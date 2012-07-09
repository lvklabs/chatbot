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
    ../../chatbot/nlp-engine \
    ../../chatbot/common \
    ../../third-party

HEADERS += \
    ../../chatbot/common/csvrow.h \
    ../../chatbot/common/csvdocument.h \


SOURCES += \
    csvdocumenttest.cpp\
    ../../chatbot/common/csvrow.cpp \
    ../../chatbot/common/csvdocument.cpp \


DEFINES += SRCDIR=\\\"$$PWD/\\\"


