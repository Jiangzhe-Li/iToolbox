#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDir>
#include <QPluginLoader>
#include <QDebug>
#include <QObject>
#include <QMessageBox>
#include <QSettings>
#include <QMap>
#include <QStringList>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    tabWidget(new QTabWidget())
{
    ui->setupUi(this);

    QObject::connect(ui->actionAbout,SIGNAL(triggered()),this,SLOT(ActionAboutSlot()));
    QObject::connect(ui->actionPlugin_info,SIGNAL(triggered()),this,SLOT(ActionPluginInfoSlot()));

    QSettings settings("iToolbox.cfg", QSettings::IniFormat);
    QStringList pluginsconfList = settings.childGroups();
    for(int i=0;i<pluginsconfList.size();i++)
    {
        settings.beginGroup(pluginsconfList.at(i));
        PluginConfType tmpConf;
        QStringList childList = settings.childKeys();
        for(int j=0;j<childList.size();j++)
        {
            tmpConf.insert(childList.at(j), settings.value(childList.at(j)).toString());
        }
        pluginConf.insert(pluginsconfList.at(i), tmpConf);
        settings.endGroup();
    }

    int ret = loadPlugins();
    if(ret < 0)
    {
        QMessageBox::critical(this, "Error", "Load plugin failed: "+QString::number(ret));
    }
}

MainWindow::~MainWindow()
{
    delete tabWidget;
    delete ui;
}

void MainWindow::ActionAboutSlot()
{
    QMessageBox::information(this, "About", "iToolbox v2.0");
}

void MainWindow::ActionPluginInfoSlot()
{
    if(InstalledPlugins.size() <= 0)
    {
        QMessageBox::warning(this, "Warning", "No plugin installed!");
    }
    else
    {
        QString pluginInfoStr;
        for(int i=0;i<InstalledPlugins.size();i++)
        {
            pluginInfoStr += InstalledPlugins.at(i)->name()+" | "+InstalledPlugins.at(i)->version()+" | "+InstalledPlugins.at(i)->information()+"\n";
        }
        QMessageBox::information(this, "Plugin Info", pluginInfoStr);
    }
}

int MainWindow::loadPlugins()
{
    int count = 0;
    QDir pluginsDir = QDir(qApp->applicationDirPath());

    if(!pluginsDir.cd("plugins")) return -1;
    foreach (QString fileName, pluginsDir.entryList(QDir::Files))
    {
        qDebug()<<"plugin name: "<<fileName;
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = pluginLoader.instance();
        if(plugin)
        {
            PluginInterface *pluginInterface = qobject_cast<PluginInterface*>(plugin);
            if(pluginInterface)
            {
                InstalledPlugins.push_back(pluginInterface);
                tabWidget->insertTab(count, pluginInterface->pluginWidget(&pluginConf[pluginInterface->name()]), pluginInterface->name());
                count++;
            }
        }
        else
        {
            return -2;
        }
    }

    setCentralWidget(tabWidget);
    return count;
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    for(int i=0;i<InstalledPlugins.size();i++)
    {
        InstalledPlugins.at(i)->pluginCloseEvent();
    }

    QSettings settings("iToolbox.cfg", QSettings::IniFormat);

    QMap<QString, PluginConfType>::const_iterator iter = pluginConf.constBegin();
    while (iter != pluginConf.constEnd())
    {
        settings.beginGroup(iter.key());
        PluginConfType::const_iterator iter2 = iter.value().constBegin();
        while (iter2 != iter.value().constEnd())
        {
            settings.setValue(iter2.key(), iter2.value());
            iter2++;
        }
        settings.endGroup();
        iter++;
    }

    e->accept();
}
