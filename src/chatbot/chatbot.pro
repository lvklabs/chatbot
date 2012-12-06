TARGET = chatbot

QT += \
    gui \
    xml \
    network

TEMPLATE = app

QMAKE_CXXFLAGS += -Wall -Wextra

DEFINES +=  \
    QT_USE_FAST_CONCATENATION \
    QT_USE_FAST_OPERATOR_PLUS \
    #DRAG_AND_DROP_DISABLED

# Icon theme can be
# - chatbot: Chatbot custom icon theme
# - gnome  : Gnome-like icon theme
ICON_THEME = chatbot

# Required config options are:
# - qxmpp
# - programq
# Optional config options are:
# - freeling  : Enable freeling lemmatizer
# - gelf_stats: Enable Graylog statistics on remote server
# - openssl   : Enable cryptographic security with openssl
win32 {
    CONFIG  += qxmpp programq freeling
} else:mac {
    CONFIG  += qxmpp programq
} else {
    CONFIG  += qxmpp programq freeling
}

PROJECT_PATH = $$PWD

HEADERS += \
    main/windowbootstrap.h \
    front-end/mainwindow.h \
    front-end/ruletreemodel.h \
    front-end/ruletreeview.h \
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
    front-end/uploaderprogressdialog.h \
    front-end/accountverifwidget.h\
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
    back-end/chatbottempfile.h \
    back-end/filemetadata.h \
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
    da-server/contestdata.h \
    da-server/contestdatauploader.h \
    da-server/nullcontestdatauploader.h \
    da-server/datauploaderfactory.h

SOURCES += \
    main/main.cpp \
    front-end/mainwindow.cpp \
    front-end/ruletreemodel.cpp \
    front-end/ruletreeview.cpp \
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
    front-end/uploaderprogressdialog.cpp \
    front-end/accountverifwidget.cpp \
    back-end/appfacade.cpp \
    back-end/rule.cpp \
    back-end/chatbotrulesfile.cpp \
    back-end/aiadapter.cpp \
    back-end/rloghelper.cpp \
    back-end/accountverifier.cpp \
    back-end/chatbotfactory.cpp \
    back-end/chatbottempfile.cpp \
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
    da-server/datauploaderfactory.cpp


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
    lang/chatbot2_es_AR.ts

OTHER_FILES += \
    res/chatbot.rc


# Third-party ###############################

include(3rd-party.pri)

INCLUDEPATH += $$THIRD_PARTY_PATH

qxmpp {
    INCLUDEPATH += $$QXMPP_INCLUDE_PATH
    LIBS += -L$$QXMPP_LIB_PATH $$QXMPP_LIBS
}

programq {
    QT += qt3support
    HEADERS += $$PRGRAMQ_INCLUDE_PATH/aimlparser.h
    SOURCES += $$PRGRAMQ_SRC_PATH/aimlparser.cpp
}

# Freeling support
freeling {
    DEFINES += PCRE_STATIC FREELING_SUPPORT
    INCLUDEPATH += $$FREELING_INCLUDE_PATH
    HEADERS += nlp-engine/freelinglemmatizer.h
    SOURCES += nlp-engine/freelinglemmatizer.cpp
    LIBS += -L$$FREELING_LIB_PATH $$FREELING_LIBS
}


# 'Dale Aceptar' contest ###################

da_contest {
    include(da-contest.pri)
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
