TARGET = chatbot

QT += \
    gui \
    xml \           #required for ProgramQ
    qt3support \    #required for ProgramQ
    network         #required for QXmpp

TEMPLATE = app

QMAKE_CXXFLAGS += -Wall -Wextra

DEFINES +=  \
    QT_USE_FAST_CONCATENATION \
    QT_USE_FAST_OPERATOR_PLUS \
    DRAG_AND_DROP_DISABLED

win32 {
    CONFIG  += freeling
} else:mac {
    CONFIG  +=
    DEFINES += MAC_OS_X
} else {
    CONFIG  += freeling
}


PROJECT_PATH          = $$PWD

THIRD_PARTY_PATH      = $$PWD/../third-party

PRGRAMQ_BASE_PATH     = $$THIRD_PARTY_PATH/ProgramQ
PRGRAMQ_INCLUDE_PATH  = $$PRGRAMQ_BASE_PATH
PRGRAMQ_SRC_PATH      = $$PRGRAMQ_BASE_PATH

QXMPP_BASE_PATH       = $$THIRD_PARTY_PATH/QXmpp
QXMPP_INCLUDE_PATH    = $$QXMPP_BASE_PATH/include
QXMPP_LIB_PATH        = $$QXMPP_BASE_PATH/lib

FREELING_BASE_PATH    = $$THIRD_PARTY_PATH/Freeling
FREELING_INCLUDE_PATH = $$FREELING_BASE_PATH/include
FREELING_LIB_PATH     = $$FREELING_BASE_PATH/lib


CONFIG(debug, debug|release) {
    win32 {
        QXMPP_LIBS         = -lqxmpp_win32_d
        FREELING_LIBS      = -lmorfo_win32 -lfries_win32 -lomlet_win32 -lpcre_win32
    } else:mac {
        QXMPP_LIBS         = -lqxmpp_mac_d
        FREELING_LIBS      = # TODO compile freeling for Mac
    } else {
        QXMPP_LIBS         = -lqxmpp_d
        FREELING_LIBS      = -lmorfo -lfries -lomlet
    }
} else {
    win32 {
        QXMPP_LIBS         = -lqxmpp_win32
        FREELING_LIBS      = -lmorfo_win32 -lfries_win32 -lomlet_win32 -lpcre_win32
    } else:mac {
        QXMPP_LIBS         = -lqxmpp_mac
        FREELING_LIBS      = # TODO compile freeling for Mac
    } else {
        QXMPP_LIBS         = -lqxmpp
        FREELING_LIBS      = -lmorfo -lfries -lomlet
    }
}

INCLUDEPATH += \
    $$THIRD_PARTY_PATH \
    $$QXMPP_INCLUDE_PATH

HEADERS += \
    $$PRGRAMQ_INCLUDE_PATH/aimlparser.h \
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
    front-end/ruletextview.h \
    back-end/appfacade.h \
    back-end/rule.h \
    back-end/defaultvirtualuser.h \
    back-end/conversation.h \
    back-end/roster.h \
    back-end/target.h \
    back-end/conversationwriter.h \
    back-end/conversationreader.h \
    back-end/chatbotrulesfile.h \
    nlp-engine/exactmatchengine.h \
    nlp-engine/aimlengine.h \
    nlp-engine/simpleaimlengine.h \
    nlp-engine/sanitizer.h \
    nlp-engine/defaultsanitizer.h \
    nlp-engine/nullsanitizer.h \
    nlp-engine/lemmatizer.h \
    nlp-engine/nulllemmatizer.h \
    nlp-engine/rule.h \
    nlp-engine/engine.h \
    nlp-engine/freelinglemmatizer.h \
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
    stats/statsmanager.h \
    stats/statsfile.h \
    stats/csvstatsfile.h


SOURCES += \
    $$PRGRAMQ_SRC_PATH/aimlparser.cpp \
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
    front-end/ruletextview.cpp \
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
    nlp-engine/freelinglemmatizer.cpp \
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
    stats/statsmanager.cpp \
    stats/csvstatsfile.cpp


FORMS += \
    front-end/mainwindow.ui \
    front-end/portdialog.ui \
    front-end/chathistorywidget.ui

RESOURCES += \
    resources.qrc

RC_FILE = \
    chatbot.rc

TRANSLATIONS = \
    lang/chatbot_es_AR.ts

OTHER_FILES += \
    doc/mainpage.dox \
    doc/modules.dox \
    chatbot.rc

LIBS += -L$$QXMPP_LIB_PATH $$QXMPP_LIBS


freeling {
    DEFINES += PCRE_STATIC FREELING_SUPPORT
    INCLUDEPATH += $$FREELING_INCLUDE_PATH
    HEADERS +=
    SOURCES +=
    LIBS += -L$$FREELING_LIB_PATH $$FREELING_LIBS
}


############################################
# Copy language files
win32:copylang.commands = # FIXME
else:copylang.commands = cp -R $$PROJECT_PATH/lang .

QMAKE_EXTRA_TARGETS += copylang
POST_TARGETDEPS += copylang
############################################


############################################
# Update git revision in versionrev.h
versionrev.target = common/versionrev.h
win32:versionrev.commands = $$PWD/bin/update-revision.bat $$PWD
else:versionrev.commands = $$PWD/bin/update-revision.sh

QMAKE_EXTRA_TARGETS += versionrev
PRE_TARGETDEPS += common/versionrev.h
############################################







































