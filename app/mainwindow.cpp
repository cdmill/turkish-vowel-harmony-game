#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Direction.h"
#include "model.h"
#include <QGraphicsOpacityEffect>
#include <QGraphicsEffect>
#include <QWidget>
#include <QAudioOutput>
#include <QFontDatabase>

/**
 * @brief MainWindow::MainWindow - constructor for main window class
 * @param model - the game model
 * @param parent
 */
MainWindow::MainWindow(Model& model, QWidget *parent)
    : QMainWindow(parent),
      playButtonIcon(":/Sprites/buttonIcons/play.png"),
      pauseButtonIcon(":/Sprites/buttonIcons/pause.png"),
      helpButtonIcon(":/Sprites/buttonIcons/question-mark.png"),
      healthIcon(":/Sprites/health/red-cross.png"),
      model(model),
      ui(new Ui::MainWindow)
{

    QFontDatabase::addApplicationFont(":/text_files/BenguiatStd-Book.otf");

    ui->setupUi(this);
    ui->gameScreen->saveModel(&model);
    ui->gameScreen->setVisible(false);
    ui->instructionScreen->setVisible(false);
    ui->startScreen->setVisible(true);
    ui->menubar->setVisible(true);
    ui->helpButton->setVisible(false);
    ui->pauseButton->setVisible(false);
    ui->gameOverScreen->setVisible(false);

    ui->pauseButton->setIcon(pauseButtonIcon);
    ui->pauseButton->setIconSize(QSize(10,8));
    ui->helpButton->setIcon(helpButtonIcon);
    ui->helpButton->setIconSize(QSize(10,10));
    ui->healthLabel->setPixmap(healthIcon.pixmap(25,25));

    // music
    this->startMusic();


    ui->healthProgressBar->setStyleSheet(" QProgressBar { "
                                         "background-color: #00FFFFFF;"
                                         "border: 2px solid grey;"
                                         "border-radius: 0px;"
                                         "text-align: center; }"
                                         " QProgressBar::chunk { "
                                         "background-color: #3add36;"
                                         "width: 1px;}"
                                         );

    // This magic line makes the spacebar work again.
    this->setFocusPolicy(Qt::StrongFocus);

    ui->pauseButton->setStyleSheet(
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
    ui->helpButton->setStyleSheet(
                QString("QPushButton { "
                        "background-color: black;"
                        "border-style: outset;"
                        "border-width: 2px;"
                        "border-color: maroon;"
                        "color: rgb(251, 215, 179);"
                        "padding: 6px; }"
                        "QPushButton:pressed { "
                        "background-color: maroon;"
                        "border-style: inset;"
                        "font-size: 16px; }"
                        ));


    //connect(this, &MainWindow::spacePressed, ui->gameScreen, &SceneWidget::spacePressed);
    connect(this, &MainWindow::movePlayer, ui->gameScreen, &SceneWidget::movePlayer);
    connect(this, &MainWindow::playerAttack, ui->gameScreen, &SceneWidget::playerAttack);
    connect(ui->startScreen, &StartWidget::showInstructions, this, &MainWindow::showInstructionScreen);
    connect(ui->instructionScreen, &InstructionWidget::startGame, this, &MainWindow::showGameScreen);
    connect(this, &MainWindow::startGame, ui->gameScreen, &SceneWidget::startUpdate);
    // connects to pause game when menu item is selected
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::menuItemSelected);
    connect(ui->actionControls, &QAction::triggered, this, &MainWindow::menuItemSelected);
    connect(ui->actionSuffix_Instructions, &QAction::triggered, this, &MainWindow::menuItemSelected);
    connect(ui->pauseButton, &QPushButton::clicked, this, &MainWindow::pauseButtonPushed);
    connect(ui->helpButton, &QPushButton::clicked, this, &MainWindow::helpButtonPushed);
    connect(ui->instructionScreen, &InstructionWidget::resumeGame, this, &MainWindow::closeHelpScreen);
    // connects for health bar
    connect(ui->gameScreen, &SceneWidget::updateHealth, ui->healthProgressBar, &QProgressBar::setValue);
    // connects for saving a word to the scene widget (should only be used for the first word)
    connect(this, &MainWindow::saveCurrentWordToScene, ui->gameScreen, &SceneWidget::saveCurrentParadigm);
    // shows game over screen
    connect(ui->gameScreen, &SceneWidget::gameOver, this, &MainWindow::showGameOverScreen);
    connect(ui->gameOverScreen, &GameOverWidget::restartGame, this, &MainWindow::restartGame);
    // connects for level2 and level3
    connect(ui->gameScreen, &SceneWidget::startNewLevel, this, &MainWindow::startNewLevel);
    connect(ui->gameScreen, &SceneWidget::startNewLevel, &model, &Model::startNewLevel);
    connect(this, &MainWindow::showLevelScreen, ui->instructionScreen, &InstructionWidget::showLevelScreen);
}

/**
 * @brief MainWindow::~MainWindow - destructor
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief MainWindow::keyPressEvent - handles key presses from
 * the user
 * @param event
 */
void MainWindow::keyPressEvent(QKeyEvent *event){
    switch(event->key()){
        case Qt::Key_Space:
            //ui->testLabel->setText("Space was Pressed!");
            emit movePlayer(Direction::up);
            break;
        case Qt::Key_Right:
            emit movePlayer(Direction::right);
            break;
        case Qt::Key_Left:
            emit movePlayer(Direction::left);
            break;
        case Qt::Key_Down:
            emit movePlayer(Direction::down);
            break;
        case Qt::Key_F:
            emit playerAttack();
            break;
        case Qt::Key_P:
            pauseButtonPushed();
            break;
        case Qt::Key_H:
            if(ui->instructionScreen->isVisible())
                ui->instructionScreen->closeButtonPushed();
            else
                helpButtonPushed();
            break;
        default:
            event->ignore();
            //ui->testLabel->setText("Another Button was Pressed!");
        break;
    }
}

/**
 * @brief MainWindow::showInstructionScreen - displays he instruction
 * screen
 * @param playerColor - the current player color
 */
void MainWindow::showInstructionScreen(QString playerColor) {
    ui->startScreen->setVisible(false);
    ui->gameScreen->setVisible(false);
    ui->instructionScreen->setVisible(true);
    this->playerColor = playerColor;
}

/**
 * @brief MainWindow::showGameScreen - allows user to restart the
 * game
 */
void MainWindow::showGameScreen() {
    ui->startScreen->setVisible(false);
    ui->instructionScreen->setVisible(false);
    ui->gameScreen->setVisible(true);
    ui->pauseButton->setVisible(true);
    ui->helpButton->setVisible(true);
    model.isRunning = true;
    model.currentLevel++;
    getTurkishParadigm();

    if(model.currentLevel == 1) {
        emit startGame(playerColor);
    }
    else {
        ui->gameScreen->resume();
    }
}
/**
 * @brief MainWindow::menuItemSelected - pauses
 * game when controls is selected.
 */
void MainWindow::menuItemSelected()
{
    ui->gameScreen->pause();
}

/**
 * @brief MainWindow::pauseButtonPushed - pauses the game and
 * restart the game
 */
void MainWindow::pauseButtonPushed() {
    if(model.isRunning) {
        model.isRunning = false;
        ui->gameScreen->pause();
        ui->pauseButton->setIcon(playButtonIcon);
    }
    else {
        model.isRunning = true;
        ui->gameScreen->resume();
        ui->gameScreen->setFocus();
        ui->pauseButton->setIcon(pauseButtonIcon);
    }
}

/**
 * @brief MainWindow::helpButtonPushed - shows the help screen
 * and pauses the game
 */
void MainWindow::helpButtonPushed() {
    ui->gameScreen->pause();
    ui->instructionScreen->showHelpScreen(model.currentLevel);
    ui->instructionScreen->raise();
    ui->instructionScreen->setVisible(true);
}

/**
 * @brief MainWindow::closeHelpScreen - closes the help screen
 * and restarts the game
 */
void MainWindow::closeHelpScreen() {
    ui->instructionScreen->setVisible(false);
    ui->gameScreen->setFocus();
    ui->gameScreen->resume();
}

/**
 * @brief MainWindow::getTurkishWord - gets a turkish word from
 * the model and passes it to the scene widget
 */
void MainWindow::getTurkishParadigm() {
    QStringList paradigm = model.getNewParadigm();
    emit saveCurrentWordToScene(paradigm);
}

/**
 * @brief MainWindow::showGameOverScreen - shows the game over screen
 * when the player dies
 */
void MainWindow::showGameOverScreen() {
    ui->gameOverScreen->setVisible(true);
}

/**
 * @brief MainWindow::restartGame - starts the game over when the user
 * clicks start over
 */
void MainWindow::restartGame() {
    ui->gameOverScreen->setVisible(false);
    ui->gameScreen->setFocus();
    ui->instructionScreen->reset();
    model.reset();
    getTurkishParadigm();
    emit startGame(playerColor);
}

void MainWindow::startNewLevel() {
//    ui->gameScreen->setVisible(false);
    ui->instructionScreen->setFocus();
    ui->instructionScreen->raise();
    ui->instructionScreen->setVisible(true);
    emit showLevelScreen();
}
/**
 * @brief MainWindow::startMusic - plays the music
 */
void MainWindow::startMusic()
{
    music.setAudioOutput(&output);
    music.setSource(QUrl("qrc:/Sprites/Music/backgroundMusic.mp3"));
    output.setVolume(50);
    music.setLoops(QMediaPlayer::Infinite);
    music.play();
}
