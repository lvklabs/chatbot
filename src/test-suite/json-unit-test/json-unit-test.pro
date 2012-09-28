#-------------------------------------------------
#
# Project created by QtCreator 2012-09-28T15:53:07
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = jsonUnitTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += \
    ../../chatbot \

SOURCES += \
    tst_jsonunittest.cpp \
    ../../chatbot/common/json.cpp \

DEFINES += SRCDIR=\\\"$$PWD/\\\"
