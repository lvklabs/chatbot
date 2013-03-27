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
include($$PROJECT_PATH/crypto/crypto.pri)

HEADERS -= \
    $$PROJECT_PATH/da-clue/clueengine.h \
    $$PROJECT_PATH/da-clue/regexp.h

SOURCES -= \
    $$PROJECT_PATH/da-clue/clueengine.cpp \
    $$PROJECT_PATH/da-clue/regexp.cpp
