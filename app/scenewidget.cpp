#include "scenewidget.h"
#include <QPainter>
#include "Direction.h"
#include "wordbox.h"
#include <random>
#include "level.h"
#include "level1.h"
#include "level2.h"
#include "level3.h"
#include <QRegularExpression>

/**
 * @brief SceneWidget::SceneWidget - Constructor for the scene
 * widget
 * @param parent
 */
SceneWidget::SceneWidget(QWidget *parent) : QWidget(parent),
    world(b2Vec2(0.0f, 10.0f)),
    player(&world),
    l1(&world, &player),
    l2(&world, &player),
    l3(&world, &player),
    currentPlayerState(player.getState()),
    timer(this),
    boxTimer(this),
    image(":/Sprites/whiteBlack-crusader-run/tile000.png"),
    boxSprite(":/Sprites/boxes/box.png"),
    animationSpeed(0.1),
    animationIndex(0.0) {

    /** start creating ground **/
    // Define the ground body.
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, 20.0f);

    // Call the body factory which allocates memory for the ground body
    // from a pool and creates the ground box shape (also from a pool).
    // The body is also added to the world.
    b2Body* groundBody = world.CreateBody(&groundBodyDef);

    // Define the ground box shape.
    b2PolygonShape groundBox;

    // The extents are the half-widths of the box.
    groundBox.SetAsBox(50.0f, 3.0f);

    // Add the ground fixture to the ground body.
    groundBody->CreateFixture(&groundBox, 0.0f);
    /** end creating ground **/

    /** start creating ceiling **/
    // Define the ceiling body.
    b2BodyDef ceilingBodyDef;
    ceilingBodyDef.position.Set(0.0f, 3.0f);
    b2Body* ceilingBody = world.CreateBody(&ceilingBodyDef);

    // Define the ceiling box shape.
    b2PolygonShape ceilingBox;

    // The extents are the half-widths of the box.
    ceilingBox.SetAsBox(50.0f, 3.0f);

    // Add the ceiling fixture to the ceiling body.
    ceilingBody->CreateFixture(&ceilingBox, 0.0f);
    /** end creating ceiling **/

    printf("Init world\n");

    // Add character images
    vowels.push_back(QImage(":/Sprites/characters/i.png"));         // front high unround
    vowels.push_back(QImage(":/Sprites/characters/u-dots.png"));    // front high round
    vowels.push_back(QImage(":/Sprites/characters/e.png"));         // front low unround
    vowels.push_back(QImage(":/Sprites/characters/o-dots.png"));    // front low round
    vowels.push_back(QImage(":/Sprites/characters/i-no-dot.png"));  // back high unround
    vowels.push_back(QImage(":/Sprites/characters/u.png"));         // back high round
    vowels.push_back(QImage(":/Sprites/characters/a.png"));         // back low unround
    vowels.push_back(QImage(":/Sprites/characters/o.png"));         // back low unround

    addBox();

    connect(&timer, &QTimer::timeout, this, &SceneWidget::updateWorld);
    connect(&boxTimer, &QTimer::timeout, this, &SceneWidget::addBox);
}

/**
 * @brief SceneWidget::startUpdate - starts the game updates
 * @param playerColor
 */
void SceneWidget::startUpdate(QString playerColor) {

    // reset everything for restart pushed
    currentLevel = &l1;
    this->playerColor = playerColor;
    player.health = 100;
    player.score = 0;
    player.recover();

    // load jump animation and run animation
    jumpAnimation = loadAnimation(playerColor, "crusader-jump", 7);
    runAnimation = loadAnimation(playerColor, "crusader-run", 6);
    attackAnimation = loadAnimation(playerColor, "crusader-attack", 13);
    dieAnimation = loadAnimation(playerColor, "crusader-die", 10);
    hurtAnimation = loadAnimation(playerColor, "crusader-hurt", 3);
    timer.start(10);
    boxTimer.start(4000);
}

/**
 * @brief SceneWidget::pause - pauses the game
 */
void SceneWidget::pause() {
    timer.stop();
    boxTimer.stop();
}

/**
 * @brief SceneWidget::resume - unpauses the game
 */
void SceneWidget::resume() {
    timer.start(10);
    boxTimer.start(getBoxTime());
}

/**
 * @brief SceneWidget::getBoxTime - gets the box duration
 * for the timer corresponding to the level. This makes the
 * boxes spawn more frequently in higher levels.
 * @return
 */
int SceneWidget::getBoxTime() {
    if(currentLevel == &l1)
        return 4000;
    else if(currentLevel == &l2)
        return 3000;
    else
        return 2000;
}

/**
 * @brief SceneWidget::paintEvent - paints the
 * scene
 */
void SceneWidget::paintEvent(QPaintEvent *) {
    // Create a painter
    QPainter painter(this);

    // draw level
    currentLevel->paintLevelBackground(&painter);

    // draw crusader
    player.draw(&painter, image);
    QString string("Score: ");
    string.append(QString::number(player.score));
    painter.setPen(Qt::black);

    // make the font for the score
    QFont font = painter.font();
    font.setPixelSize(18);
    font.setBold(true);
    painter.setFont(font);

    painter.drawText(175, 37, string);

    currentLevel->paintLevelGround(&painter);

    // paint boxes remove any that travel off the screen
    for(auto &[id, box] : boxes) {
        if(box.xPos() < -60)
            boxesToErase.push_back(id);
        else
            box.draw(&painter, boxSprite);
    }
    for(int id : boxesToErase) {
        boxes.erase(id);
    }

    /** start draw current word **/
    painter.setFont(QFont("ITC Benguiat Std", 45));
    //if(model->currentLevel == 3) {
    //    painter.setPen(Qt::white);
    //}
    // if the word is complete, draw the answer and draw the meaning below it
    if(isWordComplete) {
        // draw word/suffix/meaning in center of screen
        painter.drawText(QRect(0, 30, 800, 100), Qt::AlignHCenter, currentParadigm[4]);
        painter.drawText(QRect(0, 90, 800, 100), Qt::AlignHCenter, "\"" + currentParadigm[5] + "\"");

        // draw correct message below pause/help buttons
        painter.setPen(Qt::green);
        painter.setFont(QFont("ITC Benguiat Std", 25));
        painter.drawText(660, 80, "Correct!");
    }
    // if the word is incomplete, draw the word+suffix and draw the meaning below it
    else {
        painter.drawText(QRect(0, 30, 800, 100), Qt::AlignHCenter, currentParadigm[0] + currentParadigm[2]);
        painter.drawText(QRect(0, 90, 800, 100), Qt::AlignHCenter, "\"" + currentParadigm[1] + "\"");

        if(incorrectVowelHit) {
            painter.setPen(Qt::red);
            painter.setFont(QFont("ITC Benguiat Std", 25));
            painter.drawText(650, 80, "Incorrect!");
            QTimer::singleShot(600, this, &SceneWidget::turnOffIncorrectMessage);
        }
    }
    /** end draw word **/

    painter.end();
}

/**
 * @brief SceneWidget::updateWorld - handles updates for the scene,
 * checking if the player dies, and changing to a new level.
 */
void SceneWidget::updateWorld() {
    world.Step(1.0/60.0, 6, 2);
    currentLevel->landscape();
    player.update(boxes);

    // end the game if player dies
    if(player.health <= 0) {
        player.onLevel = 1;
        boxes.clear();
        emit gameOver();
        animate();
        update();
        return;
    }

    // change levels
    if(player.score >= 1 && player.onLevel < 2){
        player.onLevel = 2;
        boxTimer.stop();
        boxes.clear();
        QTimer::singleShot(2000, this, &SceneWidget::advanceLevel);
    }
    if(player.score >= 2 && player.onLevel < 3){
        player.onLevel = 3;
        QTimer::singleShot(2000, this, &SceneWidget::advanceLevel);
    }
    emit updateHealth(player.health);
    animate();
    update();
}

/**
 * @brief SceneWidget::animate - runs all animations in the scene
 */
void SceneWidget::animate() {
    // check if player state changed and reset index
    if(currentPlayerState != player.getState()) {
        animationIndex = 0;
        currentPlayerState = player.getState();
    }

    if(currentPlayerState == State::running) {
        animationIndex += animationSpeed;
        // clamp
        if ((uint)animationIndex >= runAnimation.size())
            animationIndex = 0;
        image = runAnimation[(int)animationIndex];
    }
    else if(currentPlayerState == State::jumping) {
        animationIndex += animationSpeed;
        // clamp
        if ((uint)animationIndex >= jumpAnimation.size())
            animationIndex = jumpAnimation.size() - 1;
        image = jumpAnimation[(int)animationIndex];
    }
    else if(currentPlayerState == State::attacking) {
        animationIndex += animationSpeed;
        // clamp
        if ((uint)animationIndex >= attackAnimation.size()) {
            animationIndex = attackAnimation.size() - 1;
            player.stopAttack();
        }
        image = attackAnimation[(int)animationIndex];
    }
    else if(currentPlayerState == State::hurt) {
        animationIndex += animationSpeed;
        // clamp
        if ((uint)animationIndex >= hurtAnimation.size()) {
            animationIndex = hurtAnimation.size() - 1;
            player.recover();
        }
        image = hurtAnimation[(int)animationIndex];
    }
    else if(currentPlayerState == State::dying) {
        animationIndex += animationSpeed;
        // clamp
        if ((uint)animationIndex >= dieAnimation.size()) {
            animationIndex = dieAnimation.size() - 1;
            pause();
        }
        image = dieAnimation[(int)animationIndex];
    }

}
/**
 * @brief SceneWidget::loadAnimation - loads the frames of an animation
 * into a vector of QImages
 * @param filepath - the name of the resource folder in the Sprites
 * path
 * @param frameCount - the number of frames
 * @return - a vector of frames
 */
vector<QImage> SceneWidget::loadAnimation(QString playerColor, QString filepath, int frameCount) {

    vector<QImage> frames;

    // Get each framepath, create an image, and add it to the vector
    for (int i = 0; i < frameCount; i++) {
        QString framePath("");
        // account for changes in path formats for different frame numbers
        // (max frame count is 999)
        if(i < 10)
            framePath = QString (":/Sprites/" + playerColor + "-" + filepath +
                                 "/tile00" + QString::number(i) + ".png");
        else if(i < 100)
            framePath = QString(":/Sprites/" + playerColor + "-" + filepath +
                                "/tile0" + QString::number(i) + ".png");
        else
            framePath = QString(":/Sprites/" + playerColor + "-" + filepath +
                                "/tile" + QString::number(i) + ".png");
        frames.push_back(QImage(framePath));
    }
    return frames;
}

/**
 * @brief SceneWidget::movePlayer - moves player in the input
 * direction
 * @param d - the direction the player should move
 */
void SceneWidget::movePlayer(Direction d) {
    // this makes double jump look nice
    if(currentPlayerState == State::jumping)
        currentPlayerState = State::running;
    player.move(d);
}

void SceneWidget::playerAttack() {
    player.startAttack();
}
/**
 * @brief SceneWidget::addBox - create a new box and add it to the
 * map
 */
void SceneWidget::addBox() {
    int vowel = getUnusedVowel();
    WordBox box(&world, currentBoxId, vowel, vowels[vowel]);
    boxes[currentBoxId] = box;
    currentBoxId++;
}

/**
 * @brief SceneWidget::getUnusedVowel - gets a vowel that
 * hasen't been used yet
 * @return
 */
int SceneWidget::getUnusedVowel() {
    bool isNewVowel = false;
    int vowel;
    while(!isNewVowel) {
        vowel = rand() % 8;
        isNewVowel = ( usedVowels.count(vowel) == 0 );
        if(usedVowels.size() == 8) {
            usedVowels.clear();
        }
    }
    usedVowels.insert(vowel);
    return vowel;
}

/**
 * @brief SceneWidget::saveCurrentParadigm - sets the current
 * paradigm
 * @param paradigm
 */
void SceneWidget::saveCurrentParadigm(QStringList paradigm) {
    currentParadigm = paradigm;
}

/**
 * @brief SceneWidget::saveModel - saves a pointer to the
 * model in the scene widget and sets up connects
 * @param model
 */
void SceneWidget::saveModel(Model* model) {
    this->model = model;
    connect(model, &Model::correctVowelHit, this, &SceneWidget::fillVowelSlot);
    connect(model, &Model::incorrectVowelHit, this, &SceneWidget::printIncorrectMessage);
    player.saveModel(model);
}

/**
 * @brief SceneWidget::fillVowelSlot - fills the vowel slot with
 * the selected vowel
 * @param vowel
 */
void SceneWidget::fillVowelSlot(int vowel) {
    // replace vowel slots in the suffix
    // ie, delete the '+' and replace underscores with the correct vowel

    /** level 3 **/
    if(currentParadigm[2].count("+") > 1) {
        QString* suffix = &currentParadigm[2];
        suffix->remove(0,1);

        // only fill the vowel slots of the one suffix at a time
        // if another suffix beginning with "+" is found, break
        for(int i = 0; i < suffix->size(); i++) {
            if(suffix->at(i) == '+') {
                break;
            }
            if(suffix->at(i) == '_') {
                suffix->replace(i, 1, model->vowels.at(vowel));
            }
        }
    }
    /** level 1,2 **/
    else {
        currentParadigm[2].replace("+", "");
        currentParadigm[2].replace("_", model->vowels.at(vowel));
    }

    if(noMoreVowelSlots()) {
        isWordComplete = true;
        boxTimer.stop();
        if(!(player.score >= 1 && player.onLevel < 2))
            QTimer::singleShot(3000, this, &SceneWidget::getNewParadigm);
    }
    update();
}

/**
 * @brief SceneWidget::noMoreVowelSlots - check if there aren't
 * any vowel slots left to fill
 * @return - true if there's a slot left
 */
bool SceneWidget::noMoreVowelSlots() {
    // return true if there are no more vowel slots to fill
    // ie, no more underscores in suffix
    return !currentParadigm[2].contains("_");
}

/**
 * @brief SceneWidget::printIncorrectMessage -
 * Displays 'incorrect' on the screen
 */
void SceneWidget::printIncorrectMessage() {
    incorrectVowelHit = true;
}

/**
 * @brief SceneWidget::turnOffIncorrectMessage -
 * turn off the message that user hit wrong box
 */
void SceneWidget::turnOffIncorrectMessage() {
    incorrectVowelHit = false;
}

/**
 * @brief SceneWidget::getNewParadigm - gets an unused paradigm
 * from the model
 */
void SceneWidget::getNewParadigm() {
    isWordComplete = false;

    if(model->currentLevel == 3 && currentParadigm[2].contains("+")) {
        return;
    }

    currentParadigm = model->getNewParadigm();
    usedVowels.clear();
    boxTimer.start(4000);
    update();
}

/**
 * @brief SceneWidget::advanceLevel - advances the
 * current level
 */
void SceneWidget::advanceLevel() {
    if(player.onLevel == 2) {
        currentLevel = &l2;
    }
    else if(player.onLevel == 3) {
        currentLevel = &l3;
    }
    player.health = player.maxHealth;
    boxes.clear();
    boxTimer.stop();
    pause();
    emit startNewLevel();
}
