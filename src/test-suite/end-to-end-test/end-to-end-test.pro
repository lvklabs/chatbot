#-------------------------------------------------
#
# Project created by QtCreator 2012-03-13T09:22:37
#
#-------------------------------------------------

QT       += testlib ui xml qt3support network

TARGET = endToEndTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

THIRD_PARTY_PATH      = $$PWD/../../third-party

PRGRAMQ_BASE_PATH     = $$THIRD_PARTY_PATH/ProgramQ
PRGRAMQ_INCLUDE_PATH  = $$PRGRAMQ_BASE_PATH
PRGRAMQ_SRC_PATH      = $$PRGRAMQ_BASE_PATH

QXMPP_BASE_PATH       = $$THIRD_PARTY_PATH/QXmpp
QXMPP_INCLUDE_PATH    = $$QXMPP_BASE_PATH/include
QXMPP_LIB_PATH        = $$QXMPP_BASE_PATH/lib

CONFIG(debug, debug|release) {
    win32 {
        QXMPP_LIBRARY_NAME = qxmpp_win32_d
    } else {
        QXMPP_LIBRARY_NAME = qxmpp_d
    }
} else {
    win32 {
        QXMPP_LIBRARY_NAME = qxmpp_win32
    } else {
        QXMPP_LIBRARY_NAME = qxmpp
    }
}

INCLUDEPATH += \
    ../../main-project/front-end \
    ../../main-project/back-end \
    ../../main-project/nlp-engine \
    ../../main-project/chat-adapter \
    ../../main-project/common \
    $$THIRD_PARTY_PATH \
    $$QXMPP_INCLUDE_PATH

HEADERS += \
    ../../main-project/front-end/mainwindow.h \
    ../../main-project/front-end/ruletreemodel.h \
    ../../main-project/front-end/ruleoutputwidget.h \
    ../../main-project/front-end/ruleinputwidget.h \
    ../../main-project/front-end/testconversationwidget.h \
    ../../main-project/front-end/conversationhistorywidget.h \
    ../../main-project/front-end/portdialog.h \
    ../../main-project/front-end/exportdialog.h \
    ../../main-project/front-end/importdialog.h \
    ../../main-project/front-end/rosterwidget.h \
    ../../main-project/front-end/autocompletetextedit.h \
    ../../main-project/back-end/rule.h \
    ../../main-project/back-end/coreapp.h \
    ../../main-project/back-end/defaultvirtualuser.h \
    ../../main-project/back-end/conversation.h \
    ../../main-project/back-end/conversationwriter.h \
    ../../main-project/back-end/conversationreader.h \
    ../../main-project/nlp-engine/nlpengine.h \
    ../../main-project/nlp-engine/nlprule.h \
    ../../main-project/nlp-engine/exactmatchengine.h \
    ../../main-project/nlp-engine/aimlengine.h \
    ../../main-project/nlp-engine/simpleaimlengine.h \
    ../../main-project/nlp-engine/defaultsanitizer.h \
    ../../main-project/nlp-engine/nullsanitizer.h \
    ../../main-project/chat-adapter/chatvirtualuser.h \
    ../../main-project/chat-adapter/chatbot.h \
    ../../main-project/chat-adapter/xmmpchatbot.h \
    ../../main-project/chat-adapter/fbchatbot.h \
    ../../main-project/chat-adapter/gtalkchatbot.h \
    ../../main-project/chat-adapter/chatcorpus.h \
    ../../main-project/common/random.h\
    ../../main-project/common/settings.h \
    ../../main-project/common/csvdocument.h \
    ../../main-project/common/csvrow.h \
    $$PRGRAMQ_INCLUDE_PATH/aimlparser.h \
    testxmppclient.h \
    testmainwindow.h \
    ../../main-project/back-end/contactinfo.h


SOURCES += \
    testmainwindow.cpp\
    ../../main-project/front-end/mainwindow.cpp \
    ../../main-project/front-end/ruletreemodel.cpp \
    ../../main-project/front-end/ruleoutputwidget.cpp \
    ../../main-project/front-end/ruleinputwidget.cpp \
    ../../main-project/front-end/testconversationwidget.cpp \
    ../../main-project/front-end/conversationhistorywidget.cpp \
    ../../main-project/front-end/portdialog.cpp \
    ../../main-project/front-end/exportdialog.cpp \
    ../../main-project/front-end/importdialog.cpp \
    ../../main-project/front-end/rosterwidget.cpp \
    ../../main-project/front-end/autocompletetextedit.cpp \
    ../../main-project/back-end/rule.cpp \
    ../../main-project/back-end/coreapp.cpp \
    ../../main-project/back-end/defaultvirtualuser.cpp \
    ../../main-project/back-end/conversation.cpp \
    ../../main-project/back-end/conversationwriter.cpp \
    ../../main-project/back-end/conversationreader.cpp \
    ../../main-project/nlp-engine/exactmatchengine.cpp \
    ../../main-project/nlp-engine/aimlengine.cpp \
    ../../main-project/nlp-engine/simpleaimlengine.cpp \
    ../../main-project/nlp-engine/defaultsanitizer.cpp \
    ../../main-project/nlp-engine/nullsanitizer.cpp \
    ../../main-project/chat-adapter/xmmpchatbot.cpp \
    ../../main-project/chat-adapter/fbchatbot.cpp \
    ../../main-project/chat-adapter/gtalkchatbot.cpp \
    ../../main-project/chat-adapter/chatbot.cpp \
    ../../main-project/chat-adapter/chatcorpus.cpp \
    ../../main-project/common/random.cpp \
    ../../main-project/common/settings.cpp \
    ../../main-project/common/csvdocument.cpp \
    ../../main-project/common/csvrow.cpp \
    $$PRGRAMQ_SRC_PATH/aimlparser.cpp \
    testxmppclient.cpp \
    main.cpp

FORMS += \
    ../../main-project/front-end/mainwindow.ui \
    ../../main-project/front-end/portdialog.ui

DEFINES += SRCDIR=\\\"$$PWD/\\\"

LIBS += -L$$QXMPP_LIB_PATH -l$$QXMPP_LIBRARY_NAME





