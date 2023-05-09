#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Model model;
    MainWindow w(model);
    w.setWindowIcon(QIcon(":/Sprites/appIcon/helmet.png"));
    w.show();
    return a.exec();
}
