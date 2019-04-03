#include "../Plugin/mainwindow.h"
#include <QApplication>

#include <QMutex>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

using namespace std;

int main(int argc, char *argv[])
{
    PluginConfType conf;
    conf["Version"] = "V2.0.0";
    conf["Status"] = "false";
    conf["Path"] = "/var/log";

    QApplication a(argc, argv);

    MainWindow w(0, &conf);

    w.setWindowTitle(QString::fromLocal8Bit("SWUpgradeAssistant V1.0"));
    //w.setMaximumSize(785,202);
    //w.setMinimumSize(785,202);
    w.show();
    
    return a.exec();
}
