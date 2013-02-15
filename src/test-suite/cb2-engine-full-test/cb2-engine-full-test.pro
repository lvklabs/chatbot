#-------------------------------------------------
#
# Project created by QtCreator 2013-02-14T10:18:30
#
#-------------------------------------------------

QT       += testlib
QT       -= gui
TARGET = cb2EngineFullTest
CONFIG   += console freeling
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../../chatbot

HEADERS += \
    ../../chatbot/common/settings.h \
    ../../chatbot/common/settingskeys.h \
    ../../chatbot/common/logger.h \
    ../../chatbot/common/random.h \
    ../../chatbot/nlp-engine/sanitizer.h \
    ../../chatbot/nlp-engine/defaultsanitizer.h \
    ../../chatbot/nlp-engine/nullsanitizer.h \
    ../../chatbot/nlp-engine/lemmatizer.h \
    ../../chatbot/nlp-engine/nulllemmatizer.h \
    ../../chatbot/nlp-engine/rule.h \
    ../../chatbot/nlp-engine/engine.h \
    ../../chatbot/nlp-engine/lemmatizerfactory.h \
    ../../chatbot/nlp-engine/sanitizerfactory.h \
    ../../chatbot/nlp-engine/enginefactory.h \
    ../../chatbot/nlp-engine/nlpproperties.h \
    ../../chatbot/nlp-engine/cb2engine.h \
    ../../chatbot/nlp-engine/tree.h \
    ../../chatbot/nlp-engine/globaltools.h \
    ../../chatbot/nlp-engine/scoringalgorithm.h \
    ../../chatbot/nlp-engine/matchpolicy.h \
    ../../chatbot/nlp-engine/word.h \
    ../../chatbot/nlp-engine/node.h \
    ../../chatbot/nlp-engine/result.h \
    ../../chatbot/nlp-engine/freelinglemmatizer.h \
    ../../chatbot/nlp-engine/condoutput.h \
    ../../chatbot/nlp-engine/varstack.h \
    ruledef.h \

SOURCES += \
    cb2enginefulltest.cpp \
    ../../chatbot/common/settings.cpp \
    ../../chatbot/common/logger.cpp \
    ../../chatbot/common/random.cpp \
    ../../chatbot/nlp-engine/defaultsanitizer.cpp \
    ../../chatbot/nlp-engine/lemmatizerfactory.cpp \
    ../../chatbot/nlp-engine/sanitizerfactory.cpp \
    ../../chatbot/nlp-engine/enginefactory.cpp \
    ../../chatbot/nlp-engine/cb2engine.cpp \
    ../../chatbot/nlp-engine/tree.cpp \
    ../../chatbot/nlp-engine/globaltools.cpp \
    ../../chatbot/nlp-engine/scoringalgorithm.cpp \
    ../../chatbot/nlp-engine/matchpolicy.cpp \
    ../../chatbot/nlp-engine/freelinglemmatizer.cpp


DEFINES += SRCDIR=\\\"$$PWD/\\\"

include(../../chatbot/3rd-party.pri)

PROJECT_PATH = $$PWD

FL_DATA_PATH += \
    $$PROJECT_PATH/../../third-party/Freeling/data/es/

win32 {
    warning(This test needs manual setup)
    warning(Copy freeling data in the directory where the test is executed)
    # TODO copy files automatically
} else {
    copyfiles.commands = mkdir -p ./data/freeling/; cp -Rf $$FL_DATA_PATH ./data/freeling/
}

QMAKE_EXTRA_TARGETS += copyfiles
POST_TARGETDEPS += copyfiles


