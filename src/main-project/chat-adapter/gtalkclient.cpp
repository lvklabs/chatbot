#include "gtalkclient.h"

Lvk::CA::GTalkClient::GTalkClient(QObject *parent)
    : XmppClient(parent)
{
}

void Lvk::CA::GTalkClient::connectToServer(const QString &user, const QString &passwd)
{
    return XmppClient::connectToServer(user.split("@").at(0), passwd, "gmail.com");
}

void Lvk::CA::GTalkClient::connectToServer(const QString &user, const QString &passwd,
                                          const QString &host)
{
    return XmppClient::connectToServer(user, passwd, host);
}
