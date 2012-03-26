#-------------------------------------------------
#
# Project created by QtCreator 2012-03-26T13:08:23
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = testdefaultsanitizer
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


INCLUDEPATH += \
    ../../main-project/nlp-engine \
    ../../third-party

HEADERS += \
    ../../main-project/nlp-engine/defaultsanitizer.h

SOURCES += \
    ../../main-project/nlp-engine/defaultsanitizer.cpp \
    testdefaultsanitizer.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

OTHER_FILES += \
    test_data.txt



