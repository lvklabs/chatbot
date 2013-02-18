# Chat Adapter module

QT += \
    xml \
    network \

isEmpty(PROJECT_PATH) {
    PROJECT_PATH = .
}

HEADERS += \
    $$PROJECT_PATH/chat-adapter/xmppchatbot.h \
    $$PROJECT_PATH/chat-adapter/historyhelper.h \
    $$PROJECT_PATH/chat-adapter/chatbot.h \
    $$PROJECT_PATH/chat-adapter/fbchatbot.h \
    $$PROJECT_PATH/chat-adapter/gtalkchatbot.h \
    $$PROJECT_PATH/chat-adapter/contactinfo.h \
    $$PROJECT_PATH/chat-adapter/chatcorpus.h \
    $$PROJECT_PATH/chat-adapter/chatbotai.h \
    $$PROJECT_PATH/chat-adapter/fbownmessageextension.h \

SOURCES += \
    $$PROJECT_PATH/chat-adapter/historyhelper.cpp \
    $$PROJECT_PATH/chat-adapter/xmppchatbot.cpp \
    $$PROJECT_PATH/chat-adapter/fbchatbot.cpp \
    $$PROJECT_PATH/chat-adapter/gtalkchatbot.cpp \
    $$PROJECT_PATH/chat-adapter/chatbot.cpp \
    $$PROJECT_PATH/chat-adapter/chatcorpus.cpp \
    $$PROJECT_PATH/chat-adapter/fbownmessageextension.cpp \
