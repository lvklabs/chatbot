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
    ../../chatbot/back-end \
    ../../chatbot/common

HEADERS += \
    ../../chatbot/back-end/conversation.h \
    ../../chatbot/back-end/conversationreader.h \
    ../../chatbot/back-end/conversationwriter.h \
    ../../chatbot/common/csvrow.h \
    ../../chatbot/common/csvdocument.h


SOURCES += \
    conversationrwtest.cpp\
    ../../chatbot/back-end/conversation.cpp \
    ../../chatbot/back-end/conversationreader.cpp \
    ../../chatbot/back-end/conversationwriter.cpp \
    ../../chatbot/common/csvrow.cpp \
    ../../chatbot/common/csvdocument.cpp


DEFINES += SRCDIR=\\\"$$PWD/\\\"


