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

INCLUDEPATH += ../../third-party/Freeling/include
LIBS += -L../../third-party/Freeling/lib -ldb_cxx -lmorfo -lfries -lomlet -lpcre -lboost_filesystem

####################################################


