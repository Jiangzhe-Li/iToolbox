#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDragEnterEvent>

#include <QDebug>
#include <QUrl>
#include <QString>
#include <QMimeData>
#include <QByteArray>
#include <QSettings>

#include <iostream>
#include <string>
#include <QString>
#include <QColor>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QProcess>
#include <QRegExp>

MainWindow::MainWindow(QWidget *parent, PluginConfType *pluginConf) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    pluginConf(pluginConf)
{
    ui->setupUi(this);
    setAcceptDrops(true);

    ui->packagePathLineEdit->setReadOnly(true);

    QObject::connect(ui->selectPushButton,SIGNAL(clicked()),this,SLOT(selectPackageSlot()));

    if(pluginConf != NULL)
    {
        if(pluginConf->contains("Version"))
        {
            ui->buildVersionLineEdit->setText(pluginConf->value("Version"));
        }
        if(pluginConf->contains("Path"))
        {
            ui->packagePathLineEdit->setText(pluginConf->value("Path"));
        }
        if(pluginConf->contains("Status"))
        {
            if(pluginConf->value("Status") == "true")
                ui->writeToFileCheckBox->setChecked(true);
            else
                ui->writeToFileCheckBox->setChecked(false);
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dragEnterEvent(QDragEnterEvent *e)
{
    e->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *e)
{
    qDebug()<<"dropEvent";

    QList<QUrl> urls = e->mimeData()->urls();
    if(urls.isEmpty())
        return;

    QString fileFullPathName = urls.first().toLocalFile();

    qDebug()<<"you drop file: "<<fileFullPathName;

    ui->packagePathLineEdit->setText(fileFullPathName);
    generateUpgradeCommand();
}

void MainWindow::generateUpgradeCommand(void)
{
    QString buildVersion = ui->buildVersionLineEdit->text();
    QString fileFullPathName = ui->packagePathLineEdit->text();
    if(buildVersion.isEmpty())
    {
        qWarning()<<"build verison can't be empty!";
        setTextBrowserInfo("build verison can't be empty!", QColor(255,0,0));
        return;
    }
    if(fileFullPathName.isEmpty())
    {
        qWarning()<<"package path can't be empty!";
        setTextBrowserInfo("package path can't be empty!", QColor(255,0,0));
        return;
    }

    QStringList fullNameSplit = fileFullPathName.split("/");
    QString packageName = fullNameSplit.at(fullNameSplit.size() - 1);
    qDebug()<<"packageName: "<<packageName;

    QRegExp packageNameRegExp("rfsw-image_(\\d{14}).tar");

    if(!packageNameRegExp.exactMatch(packageName))
    {
        qWarning()<<packageName<<"is invalid file, please check it!";
        setTextBrowserInfo(packageName+" is invalid file, please check it!", QColor(255,0,0));
        return;
    }

    QProcess process;
    QString cmd = "zutil adler32 "+fileFullPathName;

    process.start(cmd);
    if (!process.waitForStarted())
    {
        qWarning()<<"waiting for zutil start is abnormal";
        setTextBrowserInfo("waiting for zutil start is abnormal", QColor(255,0,0));
        return;
    }
    if (!process.waitForFinished())
    {
        qWarning()<<"waiting for zutil finish is abnormal";
        setTextBrowserInfo("waiting for zutil finish is abnormal", QColor(255,0,0));
        return;
    }

    QString resultStr = QString(process.readAll());

    qDebug()<<"result: "<<resultStr;

    if(resultStr.left(17).compare("adler32 checksum:",Qt::CaseSensitive) == 0)
    {
        int pos1 = resultStr.indexOf("(",Qt::CaseSensitive);
        int pos2 = resultStr.indexOf(")",Qt::CaseSensitive);
        qDebug()<<"pos1: "<<pos1<<"pos2: "<<pos2;
        QString adler32CheckSum = resultStr.mid(pos1+1,pos2-pos1-1);
        qDebug()<<"adler32CheckSum: "<<adler32CheckSum;

        QString packageDatetimeStr = packageName;
        packageDatetimeStr.remove("rfsw-image_");
        packageDatetimeStr.remove(".tar");
        qDebug()<<"packageDatetimeStr: "<<packageDatetimeStr;

        QString savePackageCmd = "SWmanShellCommand -s /ram/"+packageName+" "+adler32CheckSum+" "+packageDatetimeStr+" 0";
        qDebug()<<"savePackageCmd: "<<savePackageCmd;
        QString activePackageCmd = "SWmanShellCommand -a "+buildVersion+" "+buildVersion+" "+packageName+" "+packageDatetimeStr;
        qDebug()<<"activePackageCmd: "<<activePackageCmd;

        setTextBrowserInfo(savePackageCmd+"\n"+activePackageCmd,QColor(0,0,0));
        if(ui->writeToFileCheckBox->isChecked())
        {
            qDebug()<<"you have selected writing to file";
            writeToFile(savePackageCmd+"\n"+activePackageCmd);
        }
    }else
    {
        qWarning()<<"zutil output is abnormal: "<<resultStr;
        setTextBrowserInfo("zutil output is abnormal: "+resultStr,QColor(255,0,0));
        return;
    }
}

void MainWindow::setTextBrowserInfo(QString info, QColor color)
{
    ui->infoTextBrowser->clear();
    ui->infoTextBrowser->setTextColor(color);
    ui->infoTextBrowser->append(info);
}

void MainWindow::selectPackageSlot()
{
    QString fileFullPathName = ui->packagePathLineEdit->text();
    QStringList splitStrList = fileFullPathName.split("/");
    QString fileLast = splitStrList.at(splitStrList.size() - 1);
    qDebug()<<"fileLast: "<<fileLast;
    QString filePath = fileFullPathName.remove(fileLast);
    QString fileName=QFileDialog::getOpenFileName(this,QString::fromLocal8Bit("Select tar package"),filePath,"rfsw-image(*.tar)");
    qDebug()<<fileName;

    ui->packagePathLineEdit->setText(fileName);
    generateUpgradeCommand();
}

void MainWindow::writeToFile(QString data)
{
    QString cmdFileName = ui->packagePathLineEdit->text()+"_upgrade_cmd.txt";
    qDebug()<<"cmdFileName"<<cmdFileName;

    QFile f(cmdFileName);
    if(!f.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qWarning("can't open file");
        setTextBrowserInfo("can't open file", QColor(255,0,0));
        return;
    }

    QTextStream txtOutput(&f);

    txtOutput << data << endl;

    f.close();
}

bool MainWindow::pluginCloseEvent()
{
    QString buildVersion = ui->buildVersionLineEdit->text();
    QString packagePath = ui->packagePathLineEdit->text();
    QString checkBoxStatus;
    if(ui->writeToFileCheckBox->isChecked())
        checkBoxStatus = "true";
    else
        checkBoxStatus = "false";

    (*pluginConf)["Version"] = buildVersion;
    (*pluginConf)["Status"] = checkBoxStatus;
    (*pluginConf)["Path"] = packagePath;

    return true;
}
