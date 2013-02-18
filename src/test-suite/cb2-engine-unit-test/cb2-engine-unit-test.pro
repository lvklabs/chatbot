#-------------------------------------------------
#
# Project created by QtCreator 2012-03-13T09:22:37
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = cb2EngineUnitTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += \
    ../../chatbot

HEADERS += \
    mocklemmatizer.h \
    ruledef.h

SOURCES += \
    testcb2engine.cpp\
    mocklemmatizer.cpp

PROJECT_PATH = ../../chatbot

include($$PROJECT_PATH/nlp-engine/nlp-engine.pri)
include($$PROJECT_PATH/common/common.pri)

