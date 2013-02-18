# Chat Adapter module

QT += \
    xml \
    network \

HEADERS += \
    chat-adapter/xmppchatbot.h \
    chat-adapter/historyhelper.h \
    chat-adapter/chatbot.h \
    chat-adapter/fbchatbot.h \
    chat-adapter/gtalkchatbot.h \
    chat-adapter/contactinfo.h \
    chat-adapter/chatcorpus.h \
    chat-adapter/chatbotai.h \
    chat-adapter/fbownmessageextension.h \

SOURCES += \
    chat-adapter/historyhelper.cpp \
    chat-adapter/xmppchatbot.cpp \
    chat-adapter/fbchatbot.cpp \
    chat-adapter/gtalkchatbot.cpp \
    chat-adapter/chatbot.cpp \
    chat-adapter/chatcorpus.cpp \
    chat-adapter/fbownmessageextension.cpp \
