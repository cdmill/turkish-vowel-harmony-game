#include "gameoverwidget.h"
#include "ui_gameoverwidget.h"
#include <QPainter>

/**
 * @brief GameOverWidget::GameOverWidget - constructor for
 * game over widget.
 *
 * @param parent
 */
GameOverWidget::GameOverWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameOverWidget),
    gameOverText(":/Sprites/gameOver.png")
{
    ui->setupUi(this);
    ui->startOverButton->setStyleSheet(
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


    connect(ui->startOverButton, &QPushButton::clicked, this, &GameOverWidget::startOverClicked);
}

/**
 * @brief GameOverWidget::~GameOverWidget - destructor
 */
GameOverWidget::~GameOverWidget()
{
    delete ui;
}

/**
 * @brief GameOverWidget::startOverClicked - send signal
 * to scene widget to restart the game
 */
void GameOverWidget::startOverClicked() {
    emit restartGame();
}

/**
 * @brief GameOverWidget::paintEvent - draws the game over
 * screen
 */
void GameOverWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.drawImage(0, 0, gameOverText.scaled(QSize(800, 600)));
    painter.end();
}
