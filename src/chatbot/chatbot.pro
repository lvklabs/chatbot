TARGET = chatbot

QT += \
    gui \
    xml \           #required for ProgramQ
    qt3support \    #required for ProgramQ
    network         #required for QXmpp

TEMPLATE = app

PROJECT_PATH = $$PWD

QMAKE_CXXFLAGS += -Wall -Wextra

mac {
  DEFINES += MAC_OS_X
}

############################################
# Update git revision in versionrev.h
versionrev.target = common/versionrev.h
win32:versionrev.commands = $$PWD/bin/update-revision.bat $$PWD
else:versionrev.commands = $$PWD/bin/update-revision.sh
QMAKE_EXTRA_TARGETS += versionrev
PRE_TARGETDEPS += common/versionrev.h
############################################

THIRD_PARTY_PATH      = $$PWD/../third-party

PRGRAMQ_BASE_PATH     = $$THIRD_PARTY_PATH/ProgramQ
PRGRAMQ_INCLUDE_PATH  = $$PRGRAMQ_BASE_PATH
PRGRAMQ_SRC_PATH      = $$PRGRAMQ_BASE_PATH

QXMPP_BASE_PATH       = $$THIRD_PARTY_PATH/QXmpp
QXMPP_INCLUDE_PATH    = $$QXMPP_BASE_PATH/include
QXMPP_LIB_PATH        = $$QXMPP_BASE_PATH/lib

CONFIG(debug, debug|release) {
    win32 {
        QXMPP_LIBRARY_NAME = qxmpp_win32_d
    } else:mac {
        QXMPP_LIBRARY_NAME = qxmpp_mac_d
    } else {
        QXMPP_LIBRARY_NAME = qxmpp_d
    }
} else {
    win32 {
        QXMPP_LIBRARY_NAME = qxmpp_win32
    } else:mac {
        QXMPP_LIBRARY_NAME = qxmpp_mac_d
    } else {
        QXMPP_LIBRARY_NAME = qxmpp
    }
}

INCLUDEPATH += \
    front-end \
    back-end \
    nlp-engine \
    chat-adapter \
    common \
    $$THIRD_PARTY_PATH \
    $$QXMPP_INCLUDE_PATH

HEADERS += \
    front-end/mainwindow.h \
    front-end/ruletreemodel.h \
    front-end/ruleoutputwidget.h \
    front-end/ruleinputwidget.h \
    front-end/testconversationwidget.h \
    front-end/portdialog.h \
    front-end/exportdialog.h \
    front-end/importdialog.h \
    front-end/rosterwidget.h \
    front-end/autocompletetextedit.h \
    front-end/chathistorywidget.h \
    front-end/linefilteredit.h \
    back-end/appfacade.h \
    back-end/rule.h \
    back-end/defaultvirtualuser.h \
    back-end/conversation.h \
    back-end/roster.h \
    back-end/target.h \
    back-end/conversationwriter.h \
    back-end/conversationreader.h \
    back-end/chatbotrulesfile.h \
    nlp-engine/nlpengine.h \
    nlp-engine/nlprule.h \
    nlp-engine/exactmatchengine.h \
    nlp-engine/aimlengine.h \
    nlp-engine/simpleaimlengine.h \
    nlp-engine/sanitizer.h \
    nlp-engine/defaultsanitizer.h \
    nlp-engine/nullsanitizer.h \
    chat-adapter/chatvirtualuser.h \
    chat-adapter/chatbot.h \
    chat-adapter/xmmpchatbot.h \
    chat-adapter/fbchatbot.h \
    chat-adapter/gtalkchatbot.h \
    chat-adapter/contactinfo.h \
    chat-adapter/chatcorpus.h \
    common/random.h\
    common/version.h \
    common/settings.h \
    common/settingskeys.h \
    common/csvdocument.h \
    common/csvrow.h \
    common/globalstrings.h \
    common/logger.h \
    $$PRGRAMQ_INCLUDE_PATH/aimlparser.h \


SOURCES += \
    main.cpp \
    front-end/mainwindow.cpp \
    front-end/ruletreemodel.cpp \
    front-end/ruleoutputwidget.cpp \
    front-end/ruleinputwidget.cpp \
    front-end/testconversationwidget.cpp \
    front-end/portdialog.cpp \
    front-end/exportdialog.cpp \
    front-end/importdialog.cpp \
    front-end/rosterwidget.cpp \
    front-end/autocompletetextedit.cpp \
    front-end/chathistorywidget.cpp \
    front-end/linefilteredit.cpp \
    back-end/appfacade.cpp \
    back-end/rule.cpp \
    back-end/defaultvirtualuser.cpp \
    back-end/conversation.cpp \
    back-end/conversationwriter.cpp \
    back-end/conversationreader.cpp \
    back-end/chatbotrulesfile.cpp \
    nlp-engine/exactmatchengine.cpp \
    nlp-engine/aimlengine.cpp \
    nlp-engine/simpleaimlengine.cpp \
    nlp-engine/defaultsanitizer.cpp \
    nlp-engine/nullsanitizer.cpp \
    chat-adapter/xmmpchatbot.cpp \
    chat-adapter/fbchatbot.cpp \
    chat-adapter/gtalkchatbot.cpp \
    chat-adapter/chatbot.cpp \
    chat-adapter/chatcorpus.cpp \
    common/random.cpp \
    common/settings.cpp \
    common/csvdocument.cpp \
    common/csvrow.cpp \
    common/logger.cpp \
    $$PRGRAMQ_SRC_PATH/aimlparser.cpp \


FORMS += \
    front-end/mainwindow.ui \
    front-end/portdialog.ui \
    front-end/chathistorywidget.ui

RESOURCES += \
    resources.qrc

RC_FILE = \
    chatbot.rc

LIBS += -L$$QXMPP_LIB_PATH -l$$QXMPP_LIBRARY_NAME

TRANSLATIONS = \
    lang/chatbot_es_AR.ts

OTHER_FILES += \
    chatbot.rc

win32 {
    #TODO
} else {
    copylang.commands = cp -R $$PROJECT_PATH/lang .
}

QMAKE_EXTRA_TARGETS += copylang
POST_TARGETDEPS += copylang













