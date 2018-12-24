#-------------------------------------------------
#
# Project created by QtCreator 2018-12-19T11:49:20
#
#-------------------------------------------------

QT += core gui widgets

TARGET = orson
TEMPLATE = app
CONFIG += c++1z
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        src/main.cpp \
        src/mainwindow.cpp \
        src/package.cpp \
        src/packagemanager.cpp \
        src/filestreewidget.cpp

HEADERS += \
        src/mainwindow.h \
        src/package.h \
        src/packagemanager.h \
        src/filestreewidget.h

FORMS += \
        src/mainwindow.ui

LIBS += -lalpm

# Default rules for deployment.
bin.path = /usr/bin
bin.files = $${TARGET}

INSTALLS += bin