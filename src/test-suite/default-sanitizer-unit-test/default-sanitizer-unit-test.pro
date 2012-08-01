#-------------------------------------------------
#
# Project created by QtCreator 2012-03-26T13:08:23
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = defaultSanitizerUnitTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

PROJECT_PATH = $$PWD

INCLUDEPATH += \
    ../../chatbot \
    ../../third-party

HEADERS += \
    ../../chatbot/nlp-engine/defaultsanitizer.h

SOURCES += \
    ../../chatbot/nlp-engine/defaultsanitizer.cpp \
    testdefaultsanitizer.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

OTHER_FILES += \
    $$PROJECT_PATH/test_data.txt

win32 {
    copyfiles.commands = @call copy $$OTHER_FILES .
} else {
    copyfiles.commands = cp $$OTHER_FILES .
}

QMAKE_EXTRA_TARGETS += copyfiles
POST_TARGETDEPS += copyfiles
