#-------------------------------------------------
#
# Project created by QtCreator 2012-09-11T17:10:07
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = cipherUnitTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../../chatbot/

SOURCES += \
    cipherunittest.cpp \
    ../../chatbot/common/cipher.cpp


DEFINES += SRCDIR=\\\"$$PWD/\\\"
DEFINES += CRYPTO_SUPPORT

LIBS += -lcrypto

