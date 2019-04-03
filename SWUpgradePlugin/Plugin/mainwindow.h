#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "plugininterface.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0, PluginConfType *pluginConf=0);
    bool pluginCloseEvent();
    ~MainWindow();

protected:
    void dragEnterEvent(QDragEnterEvent *e);
    void dropEvent(QDropEvent *e);

private:
    Ui::MainWindow *ui;
    PluginConfType *pluginConf;

    void generateUpgradeCommand(void);
    void setTextBrowserInfo(QString info, QColor color);
    void writeToFile(QString data);

private slots:
    void selectPackageSlot();
};

#endif // MAINWINDOW_H
