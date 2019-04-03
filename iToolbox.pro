#-------------------------------------------------
#
# Project created by QtCreator 2019-04-01T15:49:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = iToolbox
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    plugininterface.h

FORMS    += mainwindow.ui

RC_FILE += Icon.rc
