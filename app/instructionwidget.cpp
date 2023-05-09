#include "instructionwidget.h"
#include "ui_instructionwidget.h"
#include <QPainter>
#include <QTimer>

/**
 * @brief InstructionWidget::InstructionWidget - Constructor
 * for widget class.
 *
 * @param parent
 */
InstructionWidget::InstructionWidget(QWidget *parent) :
    QWidget(parent),
    level1Screen(":/Sprites/levelScreens/level1.png"),
    level2Screen(":/Sprites/levelScreens/level2.png"),
    level3Screen(":/Sprites/levelScreens/level3.png"),
    backnessHarmony(":/Sprites/levelScreens/backnessHarmony.png"),
    roundingHarmony(":/Sprites/levelScreens/roundingHarmony.png"),
    dualHarmony(":/Sprites/levelScreens/harmony_backandround.png"),
    vowels(":/Sprites/vowel-image/vowels.png"),
    closeButtonIcon(":/Sprites/buttonIcons/x.png"),
    howToPlay(":/Sprites/levelScreens/howToPlayHelp"),
    ui(new Ui::InstructionWidget)
{
    ui->setupUi(this);

    instructionScreens.push_back(level1Screen);
    instructionScreens.push_back(backnessHarmony);
    instructionScreens.push_back(level2Screen);
    instructionScreens.push_back(roundingHarmony);
    instructionScreens.push_back(level3Screen);
    instructionScreens.push_back(dualHarmony);
    currentScreen = level1Screen;

    ui->closeButton->setIcon(closeButtonIcon);
    ui->closeButton->setIconSize(QSize(10,10));

    ui->continueButton->setStyleSheet(
                QString("QPushButton { "
                            "background-color: black;"
                            "border-style: outset;"
                            "border-width: 2px;"
                            "border-color: maroon;"
                            "color: rgb(251, 215, 179);"
                            "padding: 6px; }"
                        "QPushButton:pressed { "
                            "background-color: maroon;"
                            "border-style: inset; }"
                        ));
    ui->closeButton->setStyleSheet(
                QString("QPushButton { "
                            "background-color: black;"
                            "border-style: outset;"
                            "border-width: 2px;"
                            "border-color: maroon;"
                            "color: rgb(251, 215, 179);"
                            "padding: 6px; }"
                        "QPushButton:pressed { "
                            "background-color: maroon;"
                            "border-style: inset; }"
                        ));
    ui->controlsButton->setStyleSheet(
                QString("QPushButton { "
                            "background-color: black;"
                            "border-style: outset;"
                            "border-width: 2px;"
                            "border-color: maroon;"
                            "color: rgb(251, 215, 179);"
                            "padding: 6px; }"
                        "QPushButton:pressed { "
                            "background-color: maroon;"
                            "border-style: inset; }"
                        ));
    ui->levelInfoButton->setStyleSheet(
                QString("QPushButton { "
                            "background-color: black;"
                            "border-style: outset;"
                            "border-width: 2px;"
                            "border-color: maroon;"
                            "color: rgb(251, 215, 179);"
                            "padding: 6px; }"
                        "QPushButton:pressed { "
                            "background-color: maroon;"
                            "border-style: inset; }"
                        ));

    ui->continueButton->setVisible(true);
    ui->closeButton->setVisible(false);
    ui->controlsButton->setVisible(false);
    ui->levelInfoButton->setVisible(false);

    connect(ui->continueButton, &QPushButton::clicked, this, &InstructionWidget::showLevelInfoScreen);
    connect(ui->closeButton, &QPushButton::clicked, this, &InstructionWidget::closeButtonPushed);
    connect(ui->controlsButton, &QPushButton::clicked, this, &InstructionWidget::controlsButtonPushed);
    connect(ui->levelInfoButton, &QPushButton::clicked, this, &InstructionWidget::levelInfoButtonPushed);
}

/**
 * @brief InstructionWidget::~InstructionWidget - destructor
 */
InstructionWidget::~InstructionWidget()
{
    delete ui;
}

/**
 * @brief InstructionWidget::showLevelInfoScreen - shows the level screen first. When the player
 * presses 'continue', then the info for that level is shown. If the player presses 'continue' again
 * then the game starts on the respective level.
 */
void InstructionWidget::showLevelInfoScreen() {
    ui->continueButton->raise();
//    currentScreen = instructionScreens[index];
    // setup instructionScreen for next level and start current level
    if(currentScreen == backnessHarmony || currentScreen == roundingHarmony || currentScreen == dualHarmony) {
        ui->continueButton->setText("Continue");

        currentLevel++;

        if((uint)index < instructionScreens.size()-1){
            index++;
            currentScreen = instructionScreens[index];
        }
        emit startGame();
        return;
    }
    if((uint)index < instructionScreens.size()-1)
        currentScreen = instructionScreens[++index];
    ui->continueButton->setText("Start");
    update();
}

/**
 * @brief InstructionWidget::showLevelScreen - show the
 * screen that plays when a player reaches a new level.
 */
void InstructionWidget::showLevelScreen() {
    inGame = false;
    ui->continueButton->setVisible(true);
}

void InstructionWidget::reset() {
    currentScreen = level2Screen;
    currentLevel = 1;
    index = 2;
}

/**
 * @brief InstructionWidget::paintEvent - paints the instruction screen
 * screen.
 */
void InstructionWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);

    // if showing help screen during game, change opacity
    if(inGame) {
        painter.setOpacity(.9);
    }

    painter.drawImage(0, 0, currentScreen.scaled(QSize(800, 575)));

    // draw vowel table on help screen, but not on help->controls screen
    if(currentScreen == backnessHarmony) {
        painter.setOpacity(1);
        painter.drawImage(70, 70, vowels.scaled(QSize(650, 70)));
    }
    if(currentScreen == roundingHarmony || currentScreen == dualHarmony) {
        painter.setOpacity(1);
        painter.drawImage(70, 30, vowels.scaled(QSize(650, 70)));
    }
    painter.end();
}

/**
 * @brief InstructionWidget::showHelpScreen - slot to show help
 * screen when player hits help button.
 * @param level - the current level
 */
void InstructionWidget::showHelpScreen(int level) {
    inGame = true;
    currentLevel = level;
    ui->continueButton->setVisible(false);
    ui->levelInfoButton->setVisible(false);
    ui->closeButton->setVisible(true);
    ui->controlsButton->setVisible(true);

    // convert currentLevel to index {1,3,5} depending on current level in game
    currentScreen = instructionScreens[currentLevel*2-1];
    update();
}

/**
 * @brief InstructionWidget::closeButtonPushed - closes the help
 * screen
 */
void InstructionWidget::closeButtonPushed() {
    inGame = false;
    ui->closeButton->setVisible(false);
    currentScreen = instructionScreens[index];
    emit resumeGame();
}

/**
 * @brief InstructionWidget::controlsButtonPushed - displays
 * the game controls screen
 */
void InstructionWidget::controlsButtonPushed() {
    ui->controlsButton->setVisible(false);
    ui->levelInfoButton->setVisible(true);
    currentScreen = howToPlay;
    update();
}

/**
 * @brief InstructionWidget::levelInfoButtonPushed -
 * displays instructions for the current level.
 */
void InstructionWidget::levelInfoButtonPushed() {
    inGame = true;
    ui->levelInfoButton->setVisible(false);
    ui->controlsButton->setVisible(true);

    // convert currentLevel to index {1,3,5} depending on current level in game
    currentScreen = instructionScreens[currentLevel*2-1];
    update();
}
