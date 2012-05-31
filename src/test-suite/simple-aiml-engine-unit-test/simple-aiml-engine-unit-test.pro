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
    ../../main-project/nlp-engine ../../main-project/common \
    ../../third-party

HEADERS += \
    ../../main-project/nlp-engine/aimlengine.h \
    ../../main-project/nlp-engine/simpleaimlengine.h \
    ../../main-project/nlp-engine/nullsanitizer.h \
    ../../main-project/nlp-engine/defaultsanitizer.h \
    ../../main-project/common/settings.h \
    ../../third-party/ProgramQ/aimlparser.h

SOURCES += \
    simpleaimlengineunittest.cpp \
    ../../main-project/nlp-engine/aimlengine.cpp \
    ../../main-project/nlp-engine/simpleaimlengine.cpp \
    ../../main-project/nlp-engine/nullsanitizer.cpp \
    ../../main-project/nlp-engine/defaultsanitizer.cpp \
    ../../main-project/common/settings.cpp \
    ../../third-party/ProgramQ/aimlparser.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"
