
#ifndef LVK_CA_GTALKCLIENT_H
#define LVK_CA_GTALKCLIENT_H

#include "xmmpclient.h"

namespace Lvk
{

namespace CA
{

/**
 * \brief GTalk chat client
 *
 * This is a very thin layer over the Xmpp chat client because GTalk chat is already XMPP compliant
 */
//  TODO rename class to GTalkChatbot

class GTalkClient : public XmppClient
{
public:
    GTalkClient(QObject *parent = 0);

    void connectToServer(const QString &user, const QString &passwd);

private:
    virtual void connectToServer(const QString &user, const QString &passwd, const QString &host);
};

} //namespace CA

} //namespace Lvk

#endif // LVK_CA_FBCHATCLIENT_H
