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

PROJECT_PATH = $$PWD

INCLUDEPATH += \
    ../../chatbot \
    ../../third-party

HEADERS += \
    ../../chatbot/nlp-engine/aimlengine.h \
    ../../chatbot/nlp-engine/nullsanitizer.h \
    ../../chatbot/nlp-engine/defaultsanitizer.h \
    ../../chatbot/common/settings.h \
    ../../chatbot/common/logger.h \
    ../../third-party/ProgramQ/aimlparser.h

SOURCES += \
    testaimlengine.cpp\
    ../../chatbot/nlp-engine/aimlengine.cpp \
    ../../chatbot/nlp-engine/nullsanitizer.cpp \
    ../../chatbot/nlp-engine/defaultsanitizer.cpp \
    ../../chatbot/common/settings.cpp \
    ../../chatbot/common/logger.cpp \
    ../../third-party/ProgramQ/aimlparser.cpp


DEFINES += SRCDIR=\\\"$$PWD/\\\"

OTHER_FILES += \
    $$PROJECT_PATH/AimlParserRegression.xml \
    $$PROJECT_PATH/AimlParserRegression.aiml \
    $$PROJECT_PATH/AimlParserRegressionSkipList.txt

win32 {
    copyfiles.commands = @call copy $$OTHER_FILES .
} else {
    copyfiles.commands = cp $$OTHER_FILES .
}

QMAKE_EXTRA_TARGETS += copyfiles
POST_TARGETDEPS += copyfiles

