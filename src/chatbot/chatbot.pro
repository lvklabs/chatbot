TARGET = chatbot

TEMPLATE = app

QMAKE_CXXFLAGS += -Wall -Wextra

DEFINES +=  \
    ENABLE_WELCOME_WINDOW \
    QT_USE_FAST_CONCATENATION \
    QT_USE_FAST_OPERATOR_PLUS \
    #DRAG_AND_DROP_DISABLED

# Icon theme can be
# - chatbot: Chatbot custom icon theme
# - gnome  : Gnome-like icon theme
da_contest {
    ICON_THEME = chatbot
} else {
    ICON_THEME = gnome
}

# Required config options are:
# - qxmpp
# Optional config options are:
# - freeling  : Enable freeling lemmatizer
# - gelf_stats: Enable Graylog statistics on remote server
# - openssl   : Enable cryptographic security with openssl
win32 {
    CONFIG  += qxmpp freeling
} else:mac {
    CONFIG  += qxmpp
} else {
    CONFIG  += qxmpp freeling
}

PROJECT_PATH = $$PWD

TRANSLATIONS += \
    lang/chatbot2_es_AR.ts\
    lang/chatbot2_xx_XX.ts

HEADERS += \
    main/windowbootstrap.h \

SOURCES += \
    main/main.cpp \

include(common/common.pri)
include(nlp-engine/nlp-engine.pri)
include(back-end/back-end.pri)
include(front-end/front-end.pri)
include(chat-adapter/chat-adapter.pri)
include(crypto/crypto.pri)
include(stats/stats.pri)
include(da-server/da-server.pri)
da_contest:include(da-contest.pri)
include(3rd-party.pri)


### Update git revision in versionrev.h ##########################
versionrev.target = common/versionrev.h
win32:versionrev.commands = $$PWD/sh/update-revision.bat $$PWD
else:versionrev.commands = $$PWD/sh/update-revision.sh
QMAKE_EXTRA_TARGETS += versionrev
PRE_TARGETDEPS += common/versionrev.h

### Copy language files ##########################################
win32:copylang.commands = # FIXME
else:copylang.commands = cp -R $$PROJECT_PATH/lang .
QMAKE_EXTRA_TARGETS += copylang
POST_TARGETDEPS += copylang


