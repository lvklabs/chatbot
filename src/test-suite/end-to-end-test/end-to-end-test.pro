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
    ../../chatbot \
    $$THIRD_PARTY_PATH \
    $$QXMPP_INCLUDE_PATH

HEADERS += \
    testxmppclient.h \
    testmainwindow.h \
    $$PRGRAMQ_INCLUDE_PATH/aimlparser.h \
    ../../chatbot/front-end/mainwindow.h \
    ../../chatbot/front-end/ruletreemodel.h \
    ../../chatbot/front-end/ruleoutputwidget.h \
    ../../chatbot/front-end/ruleinputwidget.h \
    ../../chatbot/front-end/testconversationwidget.h \
    ../../chatbot/front-end/portdialog.h \
    ../../chatbot/front-end/exportdialog.h \
    ../../chatbot/front-end/importdialog.h \
    ../../chatbot/front-end/rosterwidget.h \
    ../../chatbot/front-end/autocompletetextedit.h \
    ../../chatbot/front-end/chathistorywidget.h \
    ../../chatbot/front-end/linefilteredit.h \
    ../../chatbot/front-end/ruletextview.h \
    ../../chatbot/front-end/scorewidget.h \
    ../../chatbot/front-end/sendscoredialog.h \
    ../../chatbot/front-end/tinyscorewidget.h \
    ../../chatbot/front-end/optionswindow.h \
    ../../chatbot/back-end/appfacade.h \
    ../../chatbot/back-end/rule.h \
    ../../chatbot/back-end/roster.h \
    ../../chatbot/back-end/target.h \
    ../../chatbot/back-end/chatbotrulesfile.h \
    ../../chatbot/back-end/rulestatshelper.h \
    ../../chatbot/back-end/historystatshelper.h \
    ../../chatbot/back-end/statshelper.h \
    ../../chatbot/back-end/score.h \
    ../../chatbot/back-end/scorealgorithm.h \
    ../../chatbot/back-end/aiadapter.h \
    ../../chatbot/nlp-engine/exactmatchengine.h \
    ../../chatbot/nlp-engine/aimlengine.h \
    ../../chatbot/nlp-engine/simpleaimlengine.h \
    ../../chatbot/nlp-engine/sanitizer.h \
    ../../chatbot/nlp-engine/defaultsanitizer.h \
    ../../chatbot/nlp-engine/nullsanitizer.h \
    ../../chatbot/nlp-engine/lemmatizer.h \
    ../../chatbot/nlp-engine/nulllemmatizer.h \
    ../../chatbot/nlp-engine/rule.h \
    ../../chatbot/nlp-engine/engine.h \
    ../../chatbot/nlp-engine/lemmatizerfactory.h \
    ../../chatbot/nlp-engine/sanitizerfactory.h \
    ../../chatbot/nlp-engine/enginefactory.h \
    ../../chatbot/chat-adapter/xmppchatbot.h \
    ../../chatbot/chat-adapter/historyhelper.h \
    ../../chatbot/chat-adapter/chatbot.h \
    ../../chatbot/chat-adapter/fbchatbot.h \
    ../../chatbot/chat-adapter/gtalkchatbot.h \
    ../../chatbot/chat-adapter/contactinfo.h \
    ../../chatbot/chat-adapter/chatcorpus.h \
    ../../chatbot/chat-adapter/chatbotai.h \
    ../../chatbot/common/random.h\
    ../../chatbot/common/version.h \
    ../../chatbot/common/settings.h \
    ../../chatbot/common/settingskeys.h \
    ../../chatbot/common/csvdocument.h \
    ../../chatbot/common/csvrow.h \
    ../../chatbot/common/globalstrings.h \
    ../../chatbot/common/conversation.h \
    ../../chatbot/common/conversationwriter.h \
    ../../chatbot/common/conversationreader.h \
    ../../chatbot/common/logger.h \
    ../../chatbot/common/remotelogger.h \
    ../../chatbot/common/nullremotelogger.h \
    ../../chatbot/common/remoteloggerfactory.h \
    ../../chatbot/stats/statsmanager.h \
    ../../chatbot/stats/statsfile.h \
    ../../chatbot/stats/securestatsfile.h \
    ../../chatbot/stats/id.h \
    ../../chatbot/stats/history.h \


SOURCES += \
    testmainwindow.cpp\
    ../../chatbot/front-end/mainwindow.cpp \
    ../../chatbot/front-end/ruletreemodel.cpp \
    ../../chatbot/front-end/ruleoutputwidget.cpp \
    ../../chatbot/front-end/ruleinputwidget.cpp \
    ../../chatbot/front-end/testconversationwidget.cpp \
    ../../chatbot/front-end/portdialog.cpp \
    ../../chatbot/front-end/exportdialog.cpp \
    ../../chatbot/front-end/importdialog.cpp \
    ../../chatbot/front-end/rosterwidget.cpp \
    ../../chatbot/front-end/autocompletetextedit.cpp \
    ../../chatbot/front-end/chathistorywidget.cpp \
    ../../chatbot/front-end/linefilteredit.cpp \
    ../../chatbot/front-end/ruletextview.cpp \
    ../../chatbot/front-end/scorewidget.cpp \
    ../../chatbot/front-end/sendscoredialog.cpp \
    ../../chatbot/front-end/tinyscorewidget.cpp \
    ../../chatbot/front-end/optionswindow.cpp \
    ../../chatbot/back-end/appfacade.cpp \
    ../../chatbot/back-end/rule.cpp \
    ../../chatbot/back-end/chatbotrulesfile.cpp \
    ../../chatbot/back-end/scorealgorithm.cpp \
    ../../chatbot/back-end/aiadapter.cpp \
    ../../chatbot/nlp-engine/exactmatchengine.cpp \
    ../../chatbot/nlp-engine/aimlengine.cpp \
    ../../chatbot/nlp-engine/simpleaimlengine.cpp \
    ../../chatbot/nlp-engine/defaultsanitizer.cpp \
    ../../chatbot/nlp-engine/lemmatizerfactory.cpp \
    ../../chatbot/nlp-engine/sanitizerfactory.cpp \
    ../../chatbot/nlp-engine/enginefactory.cpp \
    ../../chatbot/chat-adapter/historyhelper.cpp \
    ../../chatbot/chat-adapter/xmppchatbot.cpp \
    ../../chatbot/chat-adapter/fbchatbot.cpp \
    ../../chatbot/chat-adapter/gtalkchatbot.cpp \
    ../../chatbot/chat-adapter/chatbot.cpp \
    ../../chatbot/chat-adapter/chatcorpus.cpp \
    ../../chatbot/common/random.cpp \
    ../../chatbot/common/settings.cpp \
    ../../chatbot/common/csvdocument.cpp \
    ../../chatbot/common/csvrow.cpp \
    ../../chatbot/common/conversation.cpp \
    ../../chatbot/common/conversationwriter.cpp \
    ../../chatbot/common/conversationreader.cpp \
    ../../chatbot/common/logger.cpp \
    ../../chatbot/common/remoteloggerfactory.cpp \
    ../../chatbot/common/cipher.cpp \
    ../../chatbot/stats/statsmanager.cpp \
    ../../chatbot/stats/securestatsfile.cpp \
    ../../chatbot/stats/history.cpp \
    $$PRGRAMQ_SRC_PATH/aimlparser.cpp \
    testxmppclient.cpp \
    main.cpp

FORMS += \
    ../../chatbot/front-end/mainwindow.ui \
    ../../chatbot/front-end/portdialog.ui \
    ../../chatbot/front-end/chathistorywidget.ui \
    ../../chatbot/front-end/scorewidget.ui \
    ../../chatbot/front-end/tinyscorewidget.ui \
    ../../chatbot/front-end/sendscoredialog.ui \
    ../../chatbot/front-end/optionswindow.ui



DEFINES += SRCDIR=\\\"$$PWD/\\\"

LIBS += -L$$QXMPP_LIB_PATH -l$$QXMPP_LIBRARY_NAME





