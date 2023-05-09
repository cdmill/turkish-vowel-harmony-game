#ifndef STARTWIDGET_H
#define STARTWIDGET_H

#include <QWidget>
#include <QTimer>
#include <vector>

using std::vector;

namespace Ui {
class StartWidget;
}

class StartWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StartWidget(QWidget *parent = nullptr);
    ~StartWidget();

public slots:
    void selectPlayerColor();

private slots:
    void startButtonPushed();
    void updateWorld();
    void backButtonPushed();
    void howToPlayButtonPushed();
    void vowelsButtonPushed();

signals:
    void showInstructions(QString playerColor);

private:
    Ui::StartWidget *ui;
    int animationIndex = 0;
    QString playerColor = "whiteRed";
    QImage startScreenImage;
    QImage howToPlay;
    QImage vowelsInfo;
    QImage vowels;
    QImage currentScreen;
    QImage crusader;
    QTimer timer;
    vector<QImage> crusaderIdleAnimation;
    void animate();
    void paintEvent(QPaintEvent *);
};

#endif // STARTWIDGET_H
