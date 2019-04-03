#include "plugin.h"

Q_EXPORT_PLUGIN2("Plugin", Plugin)

Plugin::Plugin()
{
}

Plugin::~Plugin()
{
}

QString Plugin::name()
{
    return "SWUpgrade";
}

QString Plugin::version()
{
    return "v1.0";
}

QString Plugin::information()
{
    return "it's a software upgrade plugin";
}

QWidget *Plugin::pluginWidget(PluginConfType *pluginConf)
{
    mainWindow = new MainWindow(0, pluginConf);
    return  mainWindow;
}

bool Plugin::pluginCloseEvent()
{
    if(mainWindow->pluginCloseEvent())
    {
        delete mainWindow;
        return true;
    }
    else
        return false;
}
