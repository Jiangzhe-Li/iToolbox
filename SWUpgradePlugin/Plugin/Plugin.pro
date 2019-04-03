#-------------------------------------------------
#
# Project created by QtCreator 2019-03-30T13:06:10
#
#-------------------------------------------------

TARGET = Plugin
TEMPLATE = lib

DEFINES += PLUGIN_LIBRARY

SOURCES += plugin.cpp \
    mainwindow.cpp

HEADERS += plugin.h\
        Plugin_global.h \
    plugininterface.h \
    mainwindow.h

FORMS += \
    mainwindow.ui
