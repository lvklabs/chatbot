#include "gtalkchatbot.h"

#define GTALK_HOST    "gmail.com"

Lvk::CA::GTalkChatbot::GTalkChatbot(QObject *parent)
    : XmppChatbot(parent)
{
}

void Lvk::CA::GTalkChatbot::connectToServer(const QString &user, const QString &passwd)
{
    return XmppChatbot::connectToServer(user.split("@").at(0), passwd, GTALK_HOST);
}

void Lvk::CA::GTalkChatbot::connectToServer(const QString &user, const QString &passwd,
                                          const QString &host)
{
    return XmppChatbot::connectToServer(user, passwd, host);
}
