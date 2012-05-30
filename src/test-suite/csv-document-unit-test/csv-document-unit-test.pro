#-------------------------------------------------
#
# Project created by QtCreator 2012-05-30T15:51:36
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = csvDocumentUnitTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


INCLUDEPATH += \
    ../../main-project/nlp-engine \
    ../../main-project/common \
    ../../third-party

HEADERS += \
    ../../main-project/common/csvrow.h \
    ../../main-project/common/csvdocument.h \


SOURCES += \
    csvdocumenttest.cpp\
    ../../main-project/common/csvrow.cpp \
    ../../main-project/common/csvdocument.cpp \


DEFINES += SRCDIR=\\\"$$PWD/\\\"


