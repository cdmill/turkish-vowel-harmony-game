#include "model.h"
#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <random>

/**
 * @brief Model::Model - constructor from the model
 * reads in game rules/words from txt files.
 * @param parent
 */
Model::Model(QObject *parent)
    : QObject{parent}
{
    loadWordsFromFile(":/text_files/lvl1.txt", lvl1_paradigms);
    loadWordsFromFile(":/text_files/lvl2.txt", lvl2_paradigms);
    loadWordsFromFile(":/text_files/lvl3.txt", lvl3_paradigms);
}

/**
 * @brief Model::loadWordsFromFile - load a file and saves the input as paradigms
 * @param fileName - the file to read
 * @param paradigms - the words, suffixes, correct suffix/word inflection, and meaning
 */
void Model::loadWordsFromFile(QString fileName, map<QString, vector<QStringList>>& paradigms) {
    QFile words(fileName);

    if(words.open(QIODevice::ReadOnly)) {
        qDebug() << "success: file opened";
    }
    else {
        qDebug() << "failure: file not found";
        return;
    }

    QTextStream stream(&words);
    QString line;
    QString prevWord = "";

    while (stream.readLineInto(&line)) {
        /* splitting by \t will give the following:
            index 0 -> word
            index 1 -> meaning
            index 2 -> suffix
            ındex 3 -> correct suffix inflection
            index 4 -> correct word inflection
            index 5 -> word+suffix meaning
        */
        QStringList entry = line.split("\t");
        QStringList paradigm = {entry[0], entry[1], entry[2], entry[3], entry[4], entry[5]};

        paradigms[entry[0]].push_back(paradigm);
        if(entry[0] != prevWord || keys.size() == 8) {
            keys.push_back(entry[0]);
        }
        prevWord = entry[0];
    }
    words.close();
}

/**
 * @brief Model::getNewParadigm - gets an new paradigm for the level
 * @return  - the new paradigm
 */
QStringList Model::getNewParadigm() {
    return getUnusedParadigm();
}

/**
 * @brief Model::getCurrentParadigm - returns the currently used
 * paradigm
 * @return
 */
QStringList Model::getCurrentParadigm() {
    switch (currentLevel) {
    case 1:
        return lvl1_paradigms[currentWord].at(currentSuffix);
    case 2:
        return lvl2_paradigms[currentWord].at(currentSuffix);
    case 3:
        return lvl3_paradigms[currentWord].at(currentSuffix);
    }
    QStringList emptyList;
    return emptyList;
}

/**
 * @brief Model::getUnusedParadigm - ensures that the paradigms
 * are all used before one repeats
 * @return - a paradigm that hasn't been used yet
 */
QStringList Model::getUnusedParadigm() {
    int randomSuffix;
    int randomWord;
    bool found = false;
    QStringList paradigm;

    while(!found) {
        randomWord = rand() % 8;
        randomSuffix = rand() % 4;

        QString word = keys.at(randomWord);
        if(usedSuffixes[word].count(randomSuffix) == 0) {
            paradigm = getParadigmForCurrentLevel(word, randomSuffix);
            currentWord = word;
            currentSuffix = randomSuffix;
            usedWords.insert(word);
            usedSuffixes[word].insert(randomSuffix);
            found = true;
        }
    }
    return paradigm;
}

/**
 * @brief Model::getParadigmForCurrentLevel - gets paradigm for each level
 * @param word
 * @param randomSuffix
 * @return
 */
QStringList Model::getParadigmForCurrentLevel(QString word, int randomSuffix) {
    switch (currentLevel) {
    case 1:
        return lvl1_paradigms[word].at(randomSuffix);
    case 2:
        return lvl2_paradigms[word].at(randomSuffix);
    case 3:
        return lvl3_paradigms[word].at(randomSuffix);
    }
    QStringList emptyList;
    return emptyList;
}

/**
 * @brief Model::getSuffixForCurrentLevel - gets suffix for each level
 * @return
 */
QStringList Model::getSuffixForCurrentLevel() {
    switch (currentLevel) {
    case 1:
        return lvl1_paradigms[currentWord].at(currentSuffix);
    case 2:
        return lvl2_paradigms[currentWord].at(currentSuffix);
    case 3:
        return lvl3_paradigms[currentWord].at(currentSuffix);
    }
    QStringList emptyList;
    return emptyList;
}

/**
 * @brief Model::checkCorrectness - Checks if the box the player hit
 * was had the correct vowel (matches the current paradigm)
 * @param vowel - the vowel on the box
 * @return - true if correct
 */
bool Model::checkCorrectness(int vowel) {
    QStringList suffix = getSuffixForCurrentLevel();

    if(checkLevelCorrectness(vowel, suffix)) {
        emit correctVowelHit(vowel);
        return true;
    }
    emit incorrectVowelHit();
    return false;
}

/**
 * @brief Model::startNewLevel - clears the stored values for used words/suffixes
 */
void Model::startNewLevel() {
    usedWords.clear();
    usedSuffixes.clear();
}

/**
 * @brief Model::checkLevelCorrectness - checks that the vowel
 * on the box the player hit matches the suffix on the screen
 * @param vowel - the vowel on the box
 * @param suffix - the suffix the player is trying to fill in
 * @return
 */
bool Model::checkLevelCorrectness(int vowel, QStringList& suffix) {
    // checking for 1st and 2nd levels
    if(currentLevel < 3) {
        return suffix[3].contains(vowels.at(vowel));
    }

    /** check correctness for level 3 **/
    // split double suffix into 2 parts
    QStringList rawSuffix = suffix[2].split("+");
    int size;
    QString subString;

    // check for level 3 correctness
    switch(lvl3SuffixIndex) {
    // checking first of the two suffixes
    case 0:
        // only want to check the correctness of the vowels
        // in the first part of the answer, ie the part of
        // the answer that contains the first suffix
        size = rawSuffix[1].size();
        subString = suffix[3].left(size);
        if(subString.contains(vowels.at(vowel))) {
            lvl3SuffixIndex++;
            return true;
        }
        return false;
    // checking second of the two suffixes
    case 1:
        // only want to check the correctness of the vowels
        // in the second part of the answer, ie the part of
        // the answer that contains the second suffix
        size = rawSuffix[2].size();
        subString = suffix[3].right(size);
        if(subString.contains(vowels.at(vowel))) {
            lvl3SuffixIndex = 0;
            return true;
        }
    default:
        return false;
    }
}

void Model::reset() {
    currentLevel = 1;
    usedWords.clear();
    usedSuffixes.clear();
}
