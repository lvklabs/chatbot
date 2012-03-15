TARGET = main

QT += \
    ui \
    xml \           #required for ProgramQ
    qt3support \    #required for ProgramQ
    network         #required for QXmpp

TEMPLATE = app

THIRD_PARTY_PATH      = $$PWD/../third-party

PRGRAMQ_BASE_PATH     = $$THIRD_PARTY_PATH/ProgramQ
PRGRAMQ_INCLUDE_PATH  = $$PRGRAMQ_BASE_PATH
PRGRAMQ_SRC_PATH      = $$PRGRAMQ_BASE_PATH

QXMPP_BASE_PATH       = $$THIRD_PARTY_PATH/QXmpp
QXMPP_INCLUDE_PATH    = $$QXMPP_BASE_PATH/include
QXMPP_LIB_PATH        = $$QXMPP_BASE_PATH/lib

CONFIG(debug, debug|release) {
    QXMPP_LIBRARY_NAME = qxmpp_d
} else {
    QXMPP_LIBRARY_NAME = qxmpp
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
    back-end/rule.h \
    back-end/coreapp.h \
    nlp-engine/nlpengine.h \
    nlp-engine/nlprule.h \
    nlp-engine/exactmatchengine.h \
    nlp-engine/aimlengine.h \
    chat-adapter/xmmpclient.h \
    chat-adapter/chatclient.h \
    chat-adapter/chatvirtualuser.h \
    chat-adapter/fbchatclient.h \
    chat-adapter/gtalkclient.h \
    common/random.h\
    $$PRGRAMQ_INCLUDE_PATH/aimlparser.h

SOURCES += \
    main.cpp \
    front-end/mainwindow.cpp \
    front-end/ruletreemodel.cpp \
    front-end/ruleoutputwidget.cpp \
    front-end/ruleinputwidget.cpp \
    front-end/testconversationwidget.cpp \
    back-end/rule.cpp \
    back-end/coreapp.cpp \
    nlp-engine/exactmatchengine.cpp \
    nlp-engine/aimlengine.cpp \
    chat-adapter/xmmpclient.cpp \
    chat-adapter/fbchatclient.cpp \
    chat-adapter/gtalkclient.cpp \
    chat-adapter/chatclient.cpp \
    common/random.cpp\
    $$PRGRAMQ_SRC_PATH/aimlparser.cpp

FORMS += \
    front-end/mainwindow.ui

RESOURCES += \
    resources.qrc

TRANSLATIONS = strings_es.ts

LIBS += -L$$QXMPP_LIB_PATH -l$$QXMPP_LIBRARY_NAME









