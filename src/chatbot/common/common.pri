# Common Module

isEmpty($$PROJECT_PATH) {
    $$PROJECT_PATH = .
}

HEADERS += \
    $$PROJECT_PATH/common/random.h\
    $$PROJECT_PATH/common/version.h \
    $$PROJECT_PATH/common/settings.h \
    $$PROJECT_PATH/common/settingskeys.h \
    $$PROJECT_PATH/common/csvdocument.h \
    $$PROJECT_PATH/common/csvrow.h \
    $$PROJECT_PATH/common/globalstrings.h \
    $$PROJECT_PATH/common/conversation.h \
    $$PROJECT_PATH/common/conversationwriter.h \
    $$PROJECT_PATH/common/conversationreader.h \
    $$PROJECT_PATH/common/logger.h \
    $$PROJECT_PATH/common/json.h \
    $$PROJECT_PATH/common/crashhandler.h \

SOURCES += \
    $$PROJECT_PATH/common/random.cpp \
    $$PROJECT_PATH/common/settings.cpp \
    $$PROJECT_PATH/common/csvdocument.cpp \
    $$PROJECT_PATH/common/csvrow.cpp \
    $$PROJECT_PATH/common/conversation.cpp \
    $$PROJECT_PATH/common/conversationwriter.cpp \
    $$PROJECT_PATH/common/conversationreader.cpp \
    $$PROJECT_PATH/common/logger.cpp \
    $$PROJECT_PATH/common/json.cpp \
    $$PROJECT_PATH/common/crashhandler.cpp \
