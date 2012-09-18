#-------------------------------------------------
#
# Project created by QtCreator 2012-09-06T12:57:20
#
#-------------------------------------------------

QT       += testlib xml qt3support gui

TARGET = statsManagerTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../../chatbot/ ../../third-party

HEADERS += \
    ../../chatbot/common/settings.h \
    ../../chatbot/stats/statsmanager.h \

SOURCES += \
    statsmanagerunittest.cpp\
    ../../chatbot/stats/statsmanager.cpp \
    ../../chatbot/stats/securestatsfile.cpp \
    ../../chatbot/common/cipher.cpp \
    ../../chatbot/common/settings.cpp \
    ../../chatbot/common/conversation.cpp \
    ../../chatbot/common/logger.cpp \
    ../../chatbot/nlp-engine/defaultsanitizer.cpp \
    ../../chatbot/nlp-engine/enginefactory.cpp \
    ../../chatbot/nlp-engine/aimlengine.cpp \
    ../../chatbot/nlp-engine/simpleaimlengine.cpp \
    ../../chatbot/back-end/rule.cpp \
    ../../third-party/ProgramQ/aimlparser.cpp \



DEFINES += SRCDIR=\\\"$$PWD/\\\"
