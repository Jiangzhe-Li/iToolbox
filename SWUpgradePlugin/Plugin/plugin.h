#ifndef PLUGINA_H
#define PLUGINA_H

#include "Plugin_global.h"
#include "plugininterface.h"
#include "mainwindow.h"

class PLUGINASHARED_EXPORT Plugin
    :public QObject
    ,public PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(PluginInterface)
public:
    Plugin();
    ~Plugin();

    virtual QString name();
    virtual QString version();
    virtual QString information();
    virtual QWidget *pluginWidget(PluginConfType *pluginConf);
    virtual bool pluginCloseEvent();

private:
    MainWindow *mainWindow;
};

#endif // PLUGINA_H
