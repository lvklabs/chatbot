#-------------------------------------------------
#
# Project created by QtCreator 2013-03-05T16:10:15
#
#-------------------------------------------------

QT       += xml testlib

QT       -= gui

TARGET = clueEngineTest
CONFIG   += console freeling
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += \
    ../../chatbot

SOURCES += clueenginetest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

PROJECT_PATH = ../../chatbot

include($$PROJECT_PATH/da-clue/da-clue.pri)
include($$PROJECT_PATH/common/common.pri)
include($$PROJECT_PATH/nlp-engine/nlp-engine.pri)
include($$PROJECT_PATH/3rd-party.pri)

FL_DATA_PATH += \
    ../../../third-party/Freeling/data/es/

win32 {
    warning(This test needs manual setup)
    warning(Copy freeling data in the directory where the test is executed)
    # TODO copy files automatically
} else {
    copyfiles.commands = mkdir -p ./data/freeling/; cp -Rf $$FL_DATA_PATH ./data/freeling/
}

QMAKE_EXTRA_TARGETS += copyfiles
POST_TARGETDEPS += copyfiles

OTHER_FILES += \
    ../../chatbot/nlp-engine/nlp-engine.pri



