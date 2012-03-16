#include "fbchatbot.h"

Lvk::CA::FbChatbot::FbChatbot(QObject *parent)
    : XmppChatbot(parent)
{
}

void Lvk::CA::FbChatbot::connectToServer(const QString &user, const QString &passwd)
{
    return XmppChatbot::connectToServer(user, passwd, "chat.facebook.com");
}

void Lvk::CA::FbChatbot::connectToServer(const QString &user, const QString &passwd,
                                            const QString &host)
{
    return XmppChatbot::connectToServer(user, passwd, host);
}
