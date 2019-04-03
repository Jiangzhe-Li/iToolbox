#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.setWindowTitle(QString::fromLocal8Bit("iToolbox V2.0"));
    w.setMaximumSize(750,500);
    w.setMinimumSize(750,500);

    w.show();
    
    return a.exec();
}
