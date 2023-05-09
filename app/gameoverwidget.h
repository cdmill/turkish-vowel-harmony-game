#ifndef GAMEOVERWIDGET_H
#define GAMEOVERWIDGET_H

#include <QWidget>

namespace Ui {
class GameOverWidget;
}

class GameOverWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GameOverWidget(QWidget *parent = nullptr);
    ~GameOverWidget();

signals:
    void restartGame();

private slots:
    void startOverClicked();

private:
    Ui::GameOverWidget *ui;
    QImage gameOverText;
    void paintEvent(QPaintEvent *);
};

#endif // GAMEOVERWIDGET_H
