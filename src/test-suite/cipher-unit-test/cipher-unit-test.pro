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

THIRD_PARTY_PATH      = $$PWD/../../third-party
OPENSSL_BASE_PATH     = $$THIRD_PARTY_PATH/openssl
OPENSSL_INCLUDE_PATH  = $$OPENSSL_BASE_PATH/include
OPENSSL_LIB_PATH      = $$OPENSSL_BASE_PATH/lib

INCLUDEPATH += ../../chatbot/ $$OPENSSL_INCLUDE_PATH

SOURCES += \
    cipherunittest.cpp \
    ../../chatbot/crypto/cipher.cpp \
    ../../chatbot/crypto/keymanagerfactory.cpp \


DEFINES += SRCDIR=\\\"$$PWD/\\\"
DEFINES += OPENSSL_SUPPORT

LIBS += -L$$OPENSSL_LIB_PATH -lcrypto

