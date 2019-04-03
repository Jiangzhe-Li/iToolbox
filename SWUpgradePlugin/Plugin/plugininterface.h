#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

#include <QString>
#include <QtPlugin>
#include <QMap>

typedef QMap<QString, QString> PluginConfType;

class PluginInterface
{
public:
    virtual ~PluginInterface(){}
    virtual QString name() = 0;
    virtual QString version() = 0;
    virtual QString information() = 0;
    virtual QWidget *pluginWidget(PluginConfType *pluginConf) = 0;
    virtual bool pluginCloseEvent() = 0;
};

Q_DECLARE_INTERFACE(PluginInterface, "com.Plugin.PluginInterface/1.0")

#endif // PLUGININTERFACE_H
