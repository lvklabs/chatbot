#-------------------------------------------------
#
# Project created by QtCreator 2012-03-27T11:53:39
#
#-------------------------------------------------

QT       += testlib xml qt3support

QT       -= gui

TARGET = simpleAimlEngineUnitTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += \
    ../../chatbot/nlp-engine ../../chatbot/common \
    ../../third-party

HEADERS += \
    ../../chatbot/nlp-engine/aimlengine.h \
    ../../chatbot/nlp-engine/simpleaimlengine.h \
    ../../chatbot/nlp-engine/nullsanitizer.h \
    ../../chatbot/nlp-engine/defaultsanitizer.h \
    ../../chatbot/common/settings.h \
    ../../third-party/ProgramQ/aimlparser.h

SOURCES += \
    simpleaimlengineunittest.cpp \
    ../../chatbot/nlp-engine/aimlengine.cpp \
    ../../chatbot/nlp-engine/simpleaimlengine.cpp \
    ../../chatbot/nlp-engine/nullsanitizer.cpp \
    ../../chatbot/nlp-engine/defaultsanitizer.cpp \
    ../../chatbot/common/settings.cpp \
    ../../third-party/ProgramQ/aimlparser.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"
