#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setFixedSize(1300, 945);
    w.setWindowTitle("lab4Yurchik");
    w.show();
    return a.exec();
}
