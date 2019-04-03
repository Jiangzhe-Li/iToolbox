#-------------------------------------------------
#
# Project created by QtCreator 2018-12-19T14:26:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SWUpgradeAssistant
TEMPLATE = app


SOURCES += main.cpp \
    ../Plugin/mainwindow.cpp

HEADERS  += \
    ../Plugin/plugininterface.h \
    ../Plugin/mainwindow.h

FORMS    += \
    ../Plugin/mainwindow.ui
