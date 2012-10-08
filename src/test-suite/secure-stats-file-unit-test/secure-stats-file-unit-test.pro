#-------------------------------------------------
#
# Project created by QtCreator 2012-09-06T10:32:05
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = secureStatsFileUnitTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../../chatbot/

SOURCES += \
    ../../chatbot/stats/securestatsfile.cpp \
    ../../chatbot/crypto/cipher.cpp \
    ../../chatbot/crypto/keymanagerfactory.cpp \
    ../../chatbot/common/conversation.cpp \
    securestatsfiletest.cpp



DEFINES += SRCDIR=\\\"$$PWD/\\\"


