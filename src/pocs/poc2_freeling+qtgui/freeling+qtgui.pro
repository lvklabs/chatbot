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

FREELING_INCLUDE_DIR = /home/andres/Downloads/FreeLing/FreeLing-2.2.2/src/include \
                       /home/andres/Downloads/FreeLing/libfries-1.2.1/src/include\
                       /home/andres/Downloads/FreeLing/libomlet-1.0.1/src/include

FREELING_LIBS_DIR = -L/home/andres/Downloads/FreeLing/FreeLing-2.2.2/src/libmorfo/.libs/ \
                    -L/home/andres/Downloads/FreeLing/libfries-1.2.1/src/libfries/.libs\
                    -L/home/andres/Downloads/FreeLing/libomlet-1.0.1/src/libomlet/.libs

FREELING_LIBS = $$FREELING_LIBS_DIR -ldb_cxx -lmorfo -lfries -lomlet -lpcre -lboost_filesystem

INCLUDEPATH += $$FREELING_INCLUDE_DIR
LIBS += $$FREELING_LIBS

####################################################


