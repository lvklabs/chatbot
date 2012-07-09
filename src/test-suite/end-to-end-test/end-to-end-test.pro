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
    ../../chatbot/front-end \
    ../../chatbot/back-end \
    ../../chatbot/nlp-engine \
    ../../chatbot/chat-adapter \
    ../../chatbot/common \
    $$THIRD_PARTY_PATH \
    $$QXMPP_INCLUDE_PATH

HEADERS += \
    ../../chatbot/front-end/mainwindow.h \
    ../../chatbot/front-end/ruletreemodel.h \
    ../../chatbot/front-end/ruleoutputwidget.h \
    ../../chatbot/front-end/ruleinputwidget.h \
    ../../chatbot/front-end/testconversationwidget.h \
    ../../chatbot/front-end/conversationhistorywidget.h \
    ../../chatbot/front-end/portdialog.h \
    ../../chatbot/front-end/exportdialog.h \
    ../../chatbot/front-end/importdialog.h \
    ../../chatbot/front-end/rosterwidget.h \
    ../../chatbot/front-end/autocompletetextedit.h \
    ../../chatbot/back-end/rule.h \
    ../../chatbot/back-end/appfacade.h \
    ../../chatbot/back-end/defaultvirtualuser.h \
    ../../chatbot/back-end/conversation.h \
    ../../chatbot/back-end/conversationwriter.h \
    ../../chatbot/back-end/conversationreader.h \
    ../../chatbot/back-end/contactinfo.h \
    ../../chatbot/nlp-engine/nlpengine.h \
    ../../chatbot/nlp-engine/nlprule.h \
    ../../chatbot/nlp-engine/exactmatchengine.h \
    ../../chatbot/nlp-engine/aimlengine.h \
    ../../chatbot/nlp-engine/simpleaimlengine.h \
    ../../chatbot/nlp-engine/defaultsanitizer.h \
    ../../chatbot/nlp-engine/nullsanitizer.h \
    ../../chatbot/chat-adapter/chatvirtualuser.h \
    ../../chatbot/chat-adapter/chatbot.h \
    ../../chatbot/chat-adapter/xmmpchatbot.h \
    ../../chatbot/chat-adapter/fbchatbot.h \
    ../../chatbot/chat-adapter/gtalkchatbot.h \
    ../../chatbot/chat-adapter/chatcorpus.h \
    ../../chatbot/common/random.h\
    ../../chatbot/common/settings.h \
    ../../chatbot/common/csvdocument.h \
    ../../chatbot/common/csvrow.h \
    $$PRGRAMQ_INCLUDE_PATH/aimlparser.h \
    testxmppclient.h \
    testmainwindow.h \


SOURCES += \
    testmainwindow.cpp\
    ../../chatbot/front-end/mainwindow.cpp \
    ../../chatbot/front-end/ruletreemodel.cpp \
    ../../chatbot/front-end/ruleoutputwidget.cpp \
    ../../chatbot/front-end/ruleinputwidget.cpp \
    ../../chatbot/front-end/testconversationwidget.cpp \
    ../../chatbot/front-end/conversationhistorywidget.cpp \
    ../../chatbot/front-end/portdialog.cpp \
    ../../chatbot/front-end/exportdialog.cpp \
    ../../chatbot/front-end/importdialog.cpp \
    ../../chatbot/front-end/rosterwidget.cpp \
    ../../chatbot/front-end/autocompletetextedit.cpp \
    ../../chatbot/back-end/rule.cpp \
    ../../chatbot/back-end/appfacade.cpp \
    ../../chatbot/back-end/defaultvirtualuser.cpp \
    ../../chatbot/back-end/conversation.cpp \
    ../../chatbot/back-end/conversationwriter.cpp \
    ../../chatbot/back-end/conversationreader.cpp \
    ../../chatbot/nlp-engine/exactmatchengine.cpp \
    ../../chatbot/nlp-engine/aimlengine.cpp \
    ../../chatbot/nlp-engine/simpleaimlengine.cpp \
    ../../chatbot/nlp-engine/defaultsanitizer.cpp \
    ../../chatbot/nlp-engine/nullsanitizer.cpp \
    ../../chatbot/chat-adapter/xmmpchatbot.cpp \
    ../../chatbot/chat-adapter/fbchatbot.cpp \
    ../../chatbot/chat-adapter/gtalkchatbot.cpp \
    ../../chatbot/chat-adapter/chatbot.cpp \
    ../../chatbot/chat-adapter/chatcorpus.cpp \
    ../../chatbot/common/random.cpp \
    ../../chatbot/common/settings.cpp \
    ../../chatbot/common/csvdocument.cpp \
    ../../chatbot/common/csvrow.cpp \
    $$PRGRAMQ_SRC_PATH/aimlparser.cpp \
    testxmppclient.cpp \
    main.cpp

FORMS += \
    ../../chatbot/front-end/mainwindow.ui \
    ../../chatbot/front-end/portdialog.ui

DEFINES += SRCDIR=\\\"$$PWD/\\\"

LIBS += -L$$QXMPP_LIB_PATH -l$$QXMPP_LIBRARY_NAME





