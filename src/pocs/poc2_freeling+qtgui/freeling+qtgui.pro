#-------------------------------------------------
#
# Project created by QtCreator 2012-02-21T10:11:12
#
#-------------------------------------------------

QT       += core gui

TARGET = freeling+qtgui
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    lemmatizer.cpp

HEADERS  += mainwindow.h \
    lemmatizer.h

FORMS    += mainwindow.ui


####################################################
# Freeling

DEFINES += PCRE_STATIC
INCLUDEPATH += ../../third-party/Freeling/include
LIBS += -L../../third-party/Freeling/lib

win32 {
    LIBS += -lmorfo_win32 -lfries_win32 -lomlet_win32 -lpcre_win32
} else {
    LIBS += -lmorfo -lfries -lomlet
}

####################################################


