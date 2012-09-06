#-------------------------------------------------
#
# Project created by QtCreator 2012-09-06T12:57:20
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = statsManagerUnitTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../../chatbot/

HEADERS += \
    ../../chatbot/common/Settings.h \

SOURCES += \
    statsmanagerunittest.cpp\
    ../../chatbot/stats/statsmanager.cpp \
    ../../chatbot/stats/securestatsfile.cpp \
    ../../chatbot/common/cipher.cpp \
    ../../chatbot/common/Settings.cpp \


DEFINES += SRCDIR=\\\"$$PWD/\\\"
