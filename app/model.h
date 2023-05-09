#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <map>
#include <vector>
#include <unordered_set>
#include <QStringList>

using std::map;
using std::vector;
using std::unordered_set;

class Model : public QObject
{
    Q_OBJECT

public:
    explicit Model(QObject *parent = nullptr);
    bool isRunning = false;
    int currentLevel = 0;
    QStringList getNewParadigm();
    QStringList getCurrentParadigm();
    bool checkCorrectness(int vowel);
    vector<QString> vowels {"i", "ü", "e", "ö", "ı", "u", "a", "o"};
    void reset();

public slots:
    void startNewLevel();

signals:
    void correctVowelHit(int vowel);
    void incorrectVowelHit();

private:
    QString currentWord;
    int currentSuffix;
    int lvl3SuffixIndex = 0;
    vector<QString> keys;
    unordered_set<QString> usedWords;
    map<QString, unordered_set<int>> usedSuffixes;
    map<QString, vector<QStringList>> lvl1_paradigms;
    map<QString, vector<QStringList>> lvl2_paradigms;
    map<QString, vector<QStringList>> lvl3_paradigms;
    QStringList getUnusedParadigm();
    void loadWordsFromFile(QString fileName, map<QString, vector<QStringList>> &wordList);
    QStringList getParadigmForCurrentLevel(QString word, int randomSuffix);
    QStringList getSuffixForCurrentLevel();
    bool checkLevelCorrectness(int vowel, QStringList& suffix);
};

#endif // MODEL_H
