#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include "Direction.h"
#include "model.h"
#include <QMediaPlayer>
#include <QAudioOutput>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Model& model, QWidget *parent = nullptr);
    ~MainWindow();
    void keyPressEvent(QKeyEvent*);

public slots:
    void showGameScreen();
    void showInstructionScreen(QString playerColor);
    void showGameOverScreen();

signals:
    void spacePressed();
    void movePlayer(Direction);
    void startGame(QString playerColor);
    void playerAttack();
    void saveCurrentWordToScene(QStringList word);
    void showLevelScreen();

private slots:
    void menuItemSelected();
    void pauseButtonPushed();
    void helpButtonPushed();
    void closeHelpScreen();   
    void restartGame();
    void startNewLevel();
    void startMusic();
//    void showLevel3Screen();

private:
    QString playerColor = "whiteBlack";
    QIcon playButtonIcon;
    QIcon pauseButtonIcon;
    QIcon helpButtonIcon;
    QIcon healthIcon;
    QMediaPlayer music;
    QAudioOutput output;
    Model& model;
    Ui::MainWindow *ui;
    void getTurkishParadigm();
};
#endif // MAINWINDOW_H
