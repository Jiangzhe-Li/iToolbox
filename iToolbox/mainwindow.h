#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "plugininterface.h"
#include <QTabWidget>
#include <QCloseEvent>
#include <QVector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *e);
    
private:
    Ui::MainWindow *ui;
    QTabWidget *tabWidget;
    QVector<PluginInterface*> InstalledPlugins;
    QMap<QString, PluginConfType> pluginConf;
    int loadPlugins();

private slots:
    void ActionAboutSlot();
    void ActionPluginInfoSlot();
};

#endif // MAINWINDOW_H
