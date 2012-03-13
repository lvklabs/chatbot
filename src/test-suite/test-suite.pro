#-------------------------------------------------
#
# Project created by QtCreator 2012-03-13T09:22:37
#
#-------------------------------------------------

QT       += testlib ui

TARGET = testmainwindow
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += \
    ../main-project/front-end \
    ../main-project/back-end \
    ../main-project/nlp-engine \
    ../main-project/common

HEADERS += \
    ../main-project/front-end/mainwindow.h \
    ../main-project/front-end/ruletreemodel.h \
    ../main-project/front-end/ruleoutputwidget.h \
    ../main-project/front-end/ruleinputwidget.h \
    ../main-project/front-end/testconversationwidget.h \
    ../main-project/back-end/rule.h \
    ../main-project/back-end/coreapp.h \
    ../main-project/nlp-engine/exactmatchengine.h \
    ../main-project/common/random.h

SOURCES += \
    testmainwindow.cpp\
    ../main-project/front-end/mainwindow.cpp \
    ../main-project/front-end/ruletreemodel.cpp \
    ../main-project/front-end/ruleoutputwidget.cpp \
    ../main-project/front-end/ruleinputwidget.cpp \
    ../main-project/front-end/testconversationwidget.cpp \
    ../main-project/back-end/rule.cpp \
    ../main-project/back-end/coreapp.cpp \
    ../main-project/nlp-engine/exactmatchengine.cpp \
    ../main-project/common/random.cpp

FORMS += \
    ../main-project/front-end/mainwindow.ui

DEFINES += SRCDIR=\\\"$$PWD/\\\"

