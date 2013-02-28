#-------------------------------------------------
#
# Project created by QtCreator 2013-02-28T16:20:27
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = scriptManagerUnitTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += \
    ../../chatbot

SOURCES += scriptmanagerunittest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

PROJECT_PATH = ../../chatbot

include($$PROJECT_PATH/da-clue/da-clue.pri)
include($$PROJECT_PATH/common/common.pri)
