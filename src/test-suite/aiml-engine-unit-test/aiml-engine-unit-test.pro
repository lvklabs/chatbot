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
    ../../main-project/nlp-engine \
    ../../main-project/common \
    ../../third-party

HEADERS += \
    ../../main-project/nlp-engine/aimlengine.h \
    ../../main-project/nlp-engine/nullsanitizer.h \
    ../../main-project/nlp-engine/defaultsanitizer.h \
    ../../main-project/common/settings.h \
    ../../third-party/ProgramQ/aimlparser.h

SOURCES += \
    testaimlengine.cpp\
    ../../main-project/nlp-engine/aimlengine.cpp \
    ../../main-project/nlp-engine/nullsanitizer.cpp \
    ../../main-project/nlp-engine/defaultsanitizer.cpp \
    ../../main-project/common/settings.cpp \
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

