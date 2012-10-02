#-------------------------------------------------
#
# Project created by QtCreator 2012-10-02T16:33:01
#
#-------------------------------------------------

QT       += xml network testlib

QT       -= gui

INCLUDEPATH += ../../chatbot

TARGET = updaterunittest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

PROJECT_PATH = $$PWD

HEADERS += \
    ../../chatbot/da-server/updater.h \
    ../../chatbot/da-server/rest.h \
    restmock.h

SOURCES += \
    updaterunittest.cpp \
    ../../chatbot/da-server/updater.cpp \
    ../../chatbot/da-server/rest.cpp \
    restmock.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

OTHER_FILES += \
    $$PROJECT_PATH/data/*.xml \

win32 {
    copyfiles.commands = @call copy $$OTHER_FILES .
} else {
    copyfiles.commands = cp -f $$OTHER_FILES .
}

QMAKE_EXTRA_TARGETS += copyfiles
POST_TARGETDEPS += copyfiles


