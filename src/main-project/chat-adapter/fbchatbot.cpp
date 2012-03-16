#include "fbchatbot.h"

#define FB_CHAT_HOST       "chat.facebook.com"

Lvk::CA::FbChatbot::FbChatbot(QObject *parent)
    : XmppChatbot(parent)
{
}

void Lvk::CA::FbChatbot::connectToServer(const QString &user, const QString &passwd)
{
    return XmppChatbot::connectToServer(user, passwd, FB_CHAT_HOST);
}

void Lvk::CA::FbChatbot::connectToServer(const QString &user, const QString &passwd,
                                            const QString &host)
{
    return XmppChatbot::connectToServer(user, passwd, host);
}
