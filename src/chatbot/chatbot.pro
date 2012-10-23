TARGET = chatbot

QT += \
    gui \
    xml \
    network \
    qt3support  #required for ProgramQ

TEMPLATE = app

QMAKE_CXXFLAGS += -Wall -Wextra

DEFINES +=  \
    QT_USE_FAST_CONCATENATION \
    QT_USE_FAST_OPERATOR_PLUS \
    DRAG_AND_DROP_DISABLED  # Drag and drop not working

# Icon theme can be
# - chatbot: Chatbot custom icon theme
# - gnome  : Gnome-like icon theme
ICON_THEME = chatbot

# Chatbot config options are:
# - freeling  : Enable freeling lemmatizer
# - gelf_stats: Enable Graylog statistics on remote server
# - openssl   : Enable cryptographic security with openssl
win32 {
    CONFIG  += freeling
} else:mac {
    CONFIG  +=
} else {
    CONFIG  += freeling
}

PROJECT_PATH          = $$PWD

# Paths to third party libraries
THIRD_PARTY_PATH      = $$PWD/../third-party

# ProgramQ - Mandatory
PRGRAMQ_BASE_PATH     = $$THIRD_PARTY_PATH/ProgramQ
PRGRAMQ_INCLUDE_PATH  = $$PRGRAMQ_BASE_PATH
PRGRAMQ_SRC_PATH      = $$PRGRAMQ_BASE_PATH

# QXmpp - Mandatory
QXMPP_BASE_PATH       = $$THIRD_PARTY_PATH/QXmpp
QXMPP_INCLUDE_PATH    = $$QXMPP_BASE_PATH/include
QXMPP_LIB_PATH        = $$QXMPP_BASE_PATH/lib

# Freeling - Optional
FREELING_BASE_PATH    = $$THIRD_PARTY_PATH/Freeling
FREELING_INCLUDE_PATH = $$FREELING_BASE_PATH/include
FREELING_LIB_PATH     = $$FREELING_BASE_PATH/lib

# Zlib - Optional
ZLIB_BASE_PATH        = $$THIRD_PARTY_PATH/zlib
ZLIB_INCLUDE_PATH     = $$ZLIB_BASE_PATH/include
ZLIB_LIB_PATH         = $$ZLIB_BASE_PATH/lib

# OpenSSL - Optional
OPENSSL_BASE_PATH     = $$THIRD_PARTY_PATH/openssl
OPENSSL_INCLUDE_PATH  = $$OPENSSL_BASE_PATH/include
OPENSSL_LIB_PATH      = $$OPENSSL_BASE_PATH/lib


CONFIG(debug, debug|release) {
    win32 {
        QXMPP_LIBS         = -lqxmpp_win32_d
        FREELING_LIBS      = -lmorfo_win32 -lfries_win32 -lomlet_win32 -lpcre_win32
        ZLIB_LIBS          = -lz_win32
        OPENSSL_LIBS       = -lcrypto_win32 -lgdi32
    } else:mac {
        QXMPP_LIBS         = -lqxmpp_mac_d
        FREELING_LIBS      = # TODO compile freeling for Mac
        ZLIB_LIBS          = # TODO compile zlib for Mac
        OPENSSL_LIBS       = # TODO compile openssl for Mac
    } else {
        QXMPP_LIBS         = -lqxmpp_d
        FREELING_LIBS      = -lmorfo -lfries -lomlet
        ZLIB_LIBS          = -lz
        OPENSSL_LIBS       = -lcrypto
    }
} else {
    win32 {
        QXMPP_LIBS         = -lqxmpp_win32
        FREELING_LIBS      = -lmorfo_win32 -lfries_win32 -lomlet_win32 -lpcre_win32
        ZLIB_LIBS          = -lz_win32
        OPENSSL_LIBS       = -lcrypto_win32 -lgdi32
    } else:mac {
        QXMPP_LIBS         = -lqxmpp_mac
        FREELING_LIBS      = # TODO compile freeling for Mac
        ZLIB_LIBS          = # TODO compile zlib for Mac
        OPENSSL_LIBS       = # TODO compile openssl for Mac
    } else {
        QXMPP_LIBS         = -lqxmpp
        FREELING_LIBS      = -lmorfo -lfries -lomlet
        ZLIB_LIBS          = -lz
        OPENSSL_LIBS       = -lcrypto
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
    front-end/scorewidget.h \
    front-end/tinyscorewidget.h \
    front-end/detailsdialog.h \
    front-end/welcomewidget.h \
    front-end/optionswindow.h \
    front-end/filedialog.h \
    front-end/mainwindowrefactor.h \
    front-end/sendscoredialog.h \
    front-end/newupdatedialog.h \
    front-end/updateexecutor.h \
    front-end/testinputtext.h \
    front-end/ruleeditwidget.h \
    front-end/connectionwidget.h \
    front-end/memberfunctor.h \
    back-end/appfacade.h \
    back-end/rule.h \
    back-end/roster.h \
    back-end/target.h \
    back-end/chatbotrulesfile.h \
    back-end/aiadapter.h \
    back-end/rloghelper.h \
    back-end/accountverifier.h \
    back-end/chattype.h \
    back-end/chatbotfactory.h \
    nlp-engine/exactmatchengine.h \
    nlp-engine/aimlengine.h \
    nlp-engine/hybridengine.h \
    nlp-engine/sanitizer.h \
    nlp-engine/defaultsanitizer.h \
    nlp-engine/nullsanitizer.h \
    nlp-engine/lemmatizer.h \
    nlp-engine/nulllemmatizer.h \
    nlp-engine/rule.h \
    nlp-engine/engine.h \
    nlp-engine/lemmatizerfactory.h \
    nlp-engine/sanitizerfactory.h \
    nlp-engine/enginefactory.h \
    nlp-engine/nlpproperties.h \
    chat-adapter/xmppchatbot.h \
    chat-adapter/historyhelper.h \
    chat-adapter/chatbot.h \
    chat-adapter/fbchatbot.h \
    chat-adapter/gtalkchatbot.h \
    chat-adapter/contactinfo.h \
    chat-adapter/chatcorpus.h \
    chat-adapter/chatbotai.h \
    chat-adapter/fbownmessageextension.h \
    common/random.h\
    common/version.h \
    common/settings.h \
    common/settingskeys.h \
    common/csvdocument.h \
    common/csvrow.h \
    common/globalstrings.h \
    common/conversation.h \
    common/conversationwriter.h \
    common/conversationreader.h \
    common/logger.h \
    common/json.h \
    common/crashhandler.h \
    crypto/cipher.h \
    crypto/keymanager.h \
    crypto/defaultkeymanager.h \
    crypto/keymanagerfactory.h \
    stats/statsmanager.h \
    stats/statsfile.h \
    stats/history.h \
    stats/timeinterval.h \
    stats/securestatsfile.h \
    stats/score.h \
    stats/metric.h \
    stats/rulestatshelper.h \
    stats/historystatshelper.h \
    stats/statshelper.h \
    da-server/remoteloggerfactory.h \
    da-server/remoteloggerkeys.h \
    da-server/remotelogger.h \
    da-server/nullremotelogger.h \
    da-server/rest.h \
    da-server/userauth.h \
    da-server/updater.h \
    da-server/updateinfo.h \
    da-server/updateversion.h \
    da-server/serverconfig.h \
    da-server/nullserverconfig.h \
    front-end/accountverifwidget.h


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
    front-end/scorewidget.cpp \
    front-end/tinyscorewidget.cpp \
    front-end/detailsdialog.cpp \
    front-end/optionswindow.cpp \
    front-end/welcomewidget.cpp \
    front-end/mainwindowrefactor.cpp \
    front-end/sendscoredialog.cpp \
    front-end/newupdatedialog.cpp \
    front-end/testinputtext.cpp \
    front-end/ruleeditwidget.cpp \
    front-end/connectionwidget.cpp \
    back-end/appfacade.cpp \
    back-end/rule.cpp \
    back-end/chatbotrulesfile.cpp \
    back-end/aiadapter.cpp \
    back-end/rloghelper.cpp \
    back-end/accountverifier.cpp \
    back-end/chatbotfactory.cpp \
    nlp-engine/exactmatchengine.cpp \
    nlp-engine/aimlengine.cpp \
    nlp-engine/hybridengine.cpp \
    nlp-engine/defaultsanitizer.cpp \
    nlp-engine/lemmatizerfactory.cpp \
    nlp-engine/sanitizerfactory.cpp \
    nlp-engine/enginefactory.cpp \
    chat-adapter/historyhelper.cpp \
    chat-adapter/xmppchatbot.cpp \
    chat-adapter/fbchatbot.cpp \
    chat-adapter/gtalkchatbot.cpp \
    chat-adapter/chatbot.cpp \
    chat-adapter/chatcorpus.cpp \
    chat-adapter/fbownmessageextension.cpp \
    common/random.cpp \
    common/settings.cpp \
    common/csvdocument.cpp \
    common/csvrow.cpp \
    common/conversation.cpp \
    common/conversationwriter.cpp \
    common/conversationreader.cpp \
    common/logger.cpp \
    common/json.cpp \
    common/crashhandler.cpp \
    crypto/cipher.cpp \
    crypto/keymanagerfactory.cpp \
    stats/statsmanager.cpp \
    stats/history.cpp \
    stats/securestatsfile.cpp \
    stats/historystatshelper.cpp \
    da-server/remoteloggerfactory.cpp \
    da-server/rest.cpp \
    da-server/userauth.cpp \
    da-server/updater.cpp \
    front-end/accountverifwidget.cpp


FORMS += \
    front-end/mainwindow.ui \
    front-end/portdialog.ui \
    front-end/chathistorywidget.ui \
    front-end/scorewidget.ui \
    front-end/tinyscorewidget.ui \
    front-end/detailsdialog.ui \
    front-end/optionswindow.ui \
    front-end/welcomewidget.ui \
    front-end/ruleeditwidget.ui \
    front-end/connectionwidget.ui \
    front-end/accountverifwidget.ui

RESOURCES += \
    res/certs.qrc \
    res/$${ICON_THEME}_theme.qrc

RC_FILE = \
    res/chatbot.rc

TRANSLATIONS = \
    lang/chatbot_es_AR.ts

OTHER_FILES += \
    res/chatbot.rc

LIBS += -L$$QXMPP_LIB_PATH $$QXMPP_LIBS


# Freeling support
freeling {
    DEFINES += PCRE_STATIC FREELING_SUPPORT
    INCLUDEPATH += $$FREELING_INCLUDE_PATH
    HEADERS += nlp-engine/freelinglemmatizer.h
    SOURCES += nlp-engine/freelinglemmatizer.cpp
    LIBS += -L$$FREELING_LIB_PATH $$FREELING_LIBS
}

# Config options required for "Dale Aceptar" contest
da_contest {
    CONFIG  += gelf_stats openssl
    DEFINES += DA_CONTEST
    INCLUDEPATH +=
    HEADERS +=
    SOURCES +=
    LIBS +=

    ### Disabled DA server for unstable releases ###
    #exists (da-server/daserverconfig.h) {
    #    DEFINES += DA_SERVER_CONFIG
    #    HEADERS += da-server/daserverconfig.h
    #}

    exists (da-server/dadevserverconfig.h) {
        DEFINES += DA_DEV_SERVER_CONFIG
        HEADERS += da-server/dadevserverconfig.h
    }

    exists (crypto/dakeymanager.h) {
        DEFINES += DA_KEY_MANAGER
        HEADERS += crypto/dakeymanager.h
        SOURCES += crypto/dakeymanager.cpp
    }

    gelf_stats {
        DEFINES += GELF_STATS_SUPPORT
        INCLUDEPATH += $$ZLIB_INCLUDE_PATH
        HEADERS += da-server/zlibhelper.h \
            da-server/graylogremotelogger.h \
            da-server/gelf.h \
            da-server/syslog.h
        SOURCES += da-server/zlibhelper.cpp \
            da-server/graylogremotelogger.cpp \
            da-server/gelf.cpp \
            da-server/syslog.cpp
        LIBS += -L$$ZLIB_LIB_PATH $$ZLIB_LIBS
    }

    openssl {
        DEFINES += OPENSSL_SUPPORT
        INCLUDEPATH += $$OPENSSL_INCLUDE_PATH
        HEADERS +=
        SOURCES +=
        LIBS += -L$$OPENSSL_LIB_PATH $$OPENSSL_LIBS
    }

    unix:!mac{
      QMAKE_LFLAGS += '-Wl,-rpath,\'\$$ORIGIN\''
      QMAKE_LFLAGS_RPATH=
    }
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
win32:versionrev.commands = $$PWD/sh/update-revision.bat $$PWD
else:versionrev.commands = $$PWD/sh/update-revision.sh

QMAKE_EXTRA_TARGETS += versionrev
PRE_TARGETDEPS += common/versionrev.h
############################################



















































