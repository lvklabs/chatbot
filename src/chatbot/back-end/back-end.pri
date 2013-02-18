# $$PROJECT_PATH/back-end Module

isEmpty(PROJECT_PATH) {
    PROJECT_PATH = .
}

HEADERS += \
    $$PROJECT_PATH/back-end/appfacade.h \
    $$PROJECT_PATH/back-end/rule.h \
    $$PROJECT_PATH/back-end/roster.h \
    $$PROJECT_PATH/back-end/target.h \
    $$PROJECT_PATH/back-end/chatbotrulesfile.h \
    $$PROJECT_PATH/back-end/aiadapter.h \
    $$PROJECT_PATH/back-end/rloghelper.h \
    $$PROJECT_PATH/back-end/accountverifier.h \
    $$PROJECT_PATH/back-end/chattype.h \
    $$PROJECT_PATH/back-end/chatbotfactory.h \
    $$PROJECT_PATH/back-end/chatbottempfile.h \
    $$PROJECT_PATH/back-end/filemetadata.h \

SOURCES += \
    $$PROJECT_PATH/back-end/appfacade.cpp \
    $$PROJECT_PATH/back-end/rule.cpp \
    $$PROJECT_PATH/back-end/chatbotrulesfile.cpp \
    $$PROJECT_PATH/back-end/aiadapter.cpp \
    $$PROJECT_PATH/back-end/rloghelper.cpp \
    $$PROJECT_PATH/back-end/accountverifier.cpp \
    $$PROJECT_PATH/back-end/chatbotfactory.cpp \
    $$PROJECT_PATH/back-end/chatbottempfile.cpp \
