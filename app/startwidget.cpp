#include "startwidget.h"
#include "ui_startwidget.h"
#include "scenewidget.h"
#include <QPainter>

const int IDLE_ANIMATION_SIZE = 5;

/**
 * @brief StartWidget::StartWidget - constructor for the start
 * widget class sets up all screens and buttons.
 * @param parent
 */
StartWidget::StartWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StartWidget),
    startScreenImage(":/Sprites/startScreen/startScreenImage.png"),
    howToPlay(":/Sprites/startScreen/howToPlay.png"),
    vowelsInfo(":/Sprites/startScreen/vowelsInfo.png"),
    vowels(":/Sprites/vowel-image/vowels.png"),
    crusader(":/Sprites/" + playerColor + "-crusader-idle/tile000.png"),
    timer(this)
{
    ui->setupUi(this);
    ui->startButton->setStyleSheet(
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
    ui->changePlayer->setStyleSheet(
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
    ui->howToPlayButton->setStyleSheet(
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
    ui->backButton->setStyleSheet(
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
    ui->vowelsButton->setStyleSheet(
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
    ui->backButton->setVisible(false);
    ui->vowelsButton->setVisible(false);
    crusaderIdleAnimation = SceneWidget::loadAnimation(playerColor, "crusader-idle", IDLE_ANIMATION_SIZE);
    currentScreen = startScreenImage;
    connect(ui->startButton, &QPushButton::clicked, this, &StartWidget::startButtonPushed);
    connect(ui->changePlayer, &QPushButton::clicked, this, &StartWidget::selectPlayerColor);
    connect(ui->howToPlayButton, &QPushButton::clicked, this, &StartWidget::howToPlayButtonPushed);
    connect(ui->backButton, &QPushButton::clicked, this, &StartWidget::backButtonPushed);
    connect(ui->vowelsButton, &QPushButton::clicked, this, &StartWidget::vowelsButtonPushed);
    connect(&timer, &QTimer::timeout, this, &StartWidget::updateWorld);
    timer.start(250);
}

StartWidget::~StartWidget()
{
    delete ui;
}

/**
 * @brief StartWidget::startButtonPushed - gets the instruction
 * screen when the player preses start
 */
void StartWidget::startButtonPushed() {
    emit showInstructions(playerColor);
}

/**
 * @brief StartWidget::howToPlayButtonPushed - displays the
 * how to play screen
 */
void StartWidget::howToPlayButtonPushed() {
    currentScreen = howToPlay;
    ui->changePlayer->setVisible(false);
    ui->startButton->setVisible(false);
    ui->howToPlayButton->setVisible(false);
    ui->backButton->setVisible(true);
    ui->vowelsButton->setVisible(true);
}

/**
 * @brief StartWidget::backButtonPushed - goes back to the
 * main instruction screen
 */
void StartWidget::backButtonPushed() {
    currentScreen = startScreenImage;
    ui->changePlayer->setVisible(true);
    ui->startButton->setVisible(true);
    ui->howToPlayButton->setVisible(true);
    ui->backButton->setVisible(false);
    ui->vowelsButton->setVisible(false);
}

/**
 * @brief StartWidget::vowelsButtonPushed - shows the info screen
 * about vowels
 */
void StartWidget::vowelsButtonPushed() {
    currentScreen = vowelsInfo;
    ui->vowelsButton->setVisible(false);
}

/**
 * @brief StartWidget::paintEvent - paints the current event
 */
void StartWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.drawImage(0, 0, currentScreen.scaled(QSize(800, 575)));
    painter.drawImage(-100, 155, crusader.scaled(QSize(400, 400)));

    // draw vowel table when on the vowelsInfo screen
    if(currentScreen == vowelsInfo) {
        painter.drawImage(80, 130, vowels.scaled(QSize(650, 70)));
    }
    painter.end();
}

/**
 * @brief StartWidget::animate - animates the idle crusader
 */
void StartWidget::animate() {
    crusader = crusaderIdleAnimation[animationIndex++];
    if(animationIndex == IDLE_ANIMATION_SIZE) {
        animationIndex = 0;
    }
}

/**
 * @brief StartWidget::selectPlayerColor - changes the player's
 * color when user pushes choose player button
 */
void StartWidget::selectPlayerColor() {
    if(playerColor == "whiteRed")
        playerColor = "black";
    else if(playerColor == "black")
        playerColor = "red";
    else if(playerColor == "red")
        playerColor = "whiteBlack";
    else if(playerColor == "whiteBlack")
        playerColor = "whiteRed";
    crusaderIdleAnimation = SceneWidget::loadAnimation(playerColor, "crusader-idle", IDLE_ANIMATION_SIZE);
}

/**
 * @brief StartWidget::updateWorld - animates the crusader and
 * draws the screen
 */
void StartWidget::updateWorld() {
    animate();
    update();
}
