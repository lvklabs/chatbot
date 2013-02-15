#-------------------------------------------------
#
# Project created by QtCreator 2012-03-13T09:22:37
#
#-------------------------------------------------

QT       += testlib qt3support

QT       -= gui

TARGET = cb2EngineUnitTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += \
    ../../chatbot

HEADERS += \
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
    ../../chatbot/nlp-engine/condoutput.h \
    ../../chatbot/nlp-engine/varstack.h \
    ../../chatbot/common/settings.h \
    ../../chatbot/common/random.h \
    ../../chatbot/common/logger.h \
    mocklemmatizer.h \
    ruledef.h

SOURCES += \
    testcb2engine.cpp\
    ../../chatbot/nlp-engine/defaultsanitizer.cpp \
    ../../chatbot/nlp-engine/lemmatizerfactory.cpp \
    ../../chatbot/nlp-engine/sanitizerfactory.cpp \
    ../../chatbot/nlp-engine/enginefactory.cpp \
    ../../chatbot/nlp-engine/cb2engine.cpp \
    ../../chatbot/nlp-engine/tree.cpp \
    ../../chatbot/nlp-engine/globaltools.cpp \
    ../../chatbot/nlp-engine/scoringalgorithm.cpp \
    ../../chatbot/nlp-engine/matchpolicy.cpp \
    ../../chatbot/common/settings.cpp \
    ../../chatbot/common/random.cpp \
    ../../chatbot/common/logger.cpp \
    mocklemmatizer.cpp





