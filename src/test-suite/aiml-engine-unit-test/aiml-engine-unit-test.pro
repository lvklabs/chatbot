#-------------------------------------------------
#
# Project created by QtCreator 2012-03-13T09:22:37
#
#-------------------------------------------------

QT       += testlib xml qt3support

TARGET = aimlEngineUnitTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += \
    ../../main-project/nlp-engine \
    ../../third-party

HEADERS += \
    ../../main-project/nlp-engine/aimlengine.h \
    ../../main-project/nlp-engine/nullsanitizer.h \
    ../../main-project/nlp-engine/defaultsanitizer.h \
    ../../third-party/ProgramQ/aimlparser.h

SOURCES += \
    testaimlengine.cpp\
    ../../main-project/nlp-engine/aimlengine.cpp \
    ../../main-project/nlp-engine/nullsanitizer.cpp \
    ../../main-project/nlp-engine/defaultsanitizer.cpp \
    ../../third-party/ProgramQ/aimlparser.cpp


DEFINES += SRCDIR=\\\"$$PWD/\\\"

OTHER_FILES += \
    AimlParserRegression.xml \
    AimlParserRegression.aiml \
    AimlParserRegressionSkipList.txt



