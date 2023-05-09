#ifndef INSTRUCTIONWIDGET_H
#define INSTRUCTIONWIDGET_H

#include <QWidget>
#include <vector>

using std::vector;

namespace Ui {
class InstructionWidget;
}

class InstructionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit InstructionWidget(QWidget *parent = nullptr);
    ~InstructionWidget();
    void reset();

public slots:
    void showLevelInfoScreen();
    void showLevelScreen();
    void showHelpScreen(int level);
    void closeButtonPushed();

signals:
    void startGame();
    void resumeGame();

private slots:
    void controlsButtonPushed();
    void levelInfoButtonPushed();

private:
    int currentLevel = 1;
    int index = 0;
    bool inGame = false;
    QImage currentScreen;
    QImage level1Screen;
    QImage level2Screen;
    QImage level3Screen;
    QImage backnessHarmony;
    QImage roundingHarmony;
    QImage dualHarmony;
    QImage vowels;
    QIcon closeButtonIcon;
    QImage howToPlay;
    vector<QImage> instructionScreens;
    void paintEvent(QPaintEvent *);
    void changeScreen();
    Ui::InstructionWidget *ui;
};

#endif // INSTRUCTIONWIDGET_H
