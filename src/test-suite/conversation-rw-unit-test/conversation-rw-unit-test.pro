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
    ../../chatbot \

HEADERS += \
    ../../chatbot/common/conversation.h \
    ../../chatbot/common/conversationreader.h \
    ../../chatbot/common/conversationwriter.h \
    ../../chatbot/common/csvrow.h \
    ../../chatbot/common/csvdocument.h


SOURCES += \
    conversationrwtest.cpp\
    ../../chatbot/common/conversation.cpp \
    ../../chatbot/common/conversationreader.cpp \
    ../../chatbot/common/conversationwriter.cpp \
    ../../chatbot/common/csvrow.cpp \
    ../../chatbot/common/csvdocument.cpp


DEFINES += SRCDIR=\\\"$$PWD/\\\"


