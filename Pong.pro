#-------------------------------------------------
#
# Project created by QtCreator 2014-11-30
#
#-------------------------------------------------

QT += core gui network widgets

TARGET = WccPong
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    gameplay.cpp \
    webcamcap.cpp \
    controlpanel.cpp

HEADERS  += mainwindow.h \
    gameplay.h \
    webcamcap.h \
    controlpanel.h

FORMS += mainwindow.ui \
    controlpanel.ui

VERSION = 1.0
