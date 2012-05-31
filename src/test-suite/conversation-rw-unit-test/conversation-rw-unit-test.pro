#-------------------------------------------------
#
# Project created by QtCreator 2012-05-31T08:47:43
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = conversationRwTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += \
    ../../main-project/back-end \
    ../../main-project/common

HEADERS += \
    ../../main-project/back-end/conversation.h \
    ../../main-project/back-end/conversationreader.h \
    ../../main-project/back-end/conversationwriter.h \
    ../../main-project/common/csvrow.h \
    ../../main-project/common/csvdocument.h


SOURCES += \
    conversationrwtest.cpp\
    ../../main-project/back-end/conversation.cpp \
    ../../main-project/back-end/conversationreader.cpp \
    ../../main-project/back-end/conversationwriter.cpp \
    ../../main-project/common/csvrow.cpp \
    ../../main-project/common/csvdocument.cpp


DEFINES += SRCDIR=\\\"$$PWD/\\\"


