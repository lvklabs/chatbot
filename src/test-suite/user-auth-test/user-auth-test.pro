#-------------------------------------------------
#
# Project created by QtCreator 2012-09-28T18:48:28
#
#-------------------------------------------------

QT       += network testlib

QT       -= gui

TARGET = userAuthTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += \
    ../../chatbot

HEADERS += \
    ../../chatbot/da-server/rest.h \
    ../../chatbot/da-server/userauth.h \

SOURCES += \
    userauthtest.cpp \
    ../../chatbot/common/json.cpp \
    ../../chatbot/da-server/userauth.cpp \
    ../../chatbot/da-server/rest.cpp \
    ../../chatbot/crypto/keymanagerfactory.cpp \

DEFINES += SRCDIR=\\\"$$PWD/\\\"

exists(../../chatbot/crypto/dakeymanager.cpp) {
    SOURCES += ../../chatbot/crypto/dakeymanager.cpp
    DEFINES += DA_CONTEST
}

