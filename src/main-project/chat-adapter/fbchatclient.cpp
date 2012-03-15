#include "fbchatclient.h"

Lvk::CA::FbChatClient::FbChatClient(QObject *parent)
    : XmppClient(parent)
{
}

void Lvk::CA::FbChatClient::connectToServer(const QString &user, const QString &passwd)
{
    return XmppClient::connectToServer(user, passwd, "chat.facebook.com");
}

void Lvk::CA::FbChatClient::connectToServer(const QString &user, const QString &passwd,
                                            const QString &host)
{
    return XmppClient::connectToServer(user, passwd, host);
}
