
#ifndef LVK_CA_FBCHATCLIENT_H
#define LVK_CA_FBCHATCLIENT_H

#include "xmmpclient.h"

namespace Lvk
{

namespace CA
{

/**
 * \brief Facebook chat client
 *
 * This is a very thin layer over the Xmpp chat client because fb chat is already XMPP compliant
 */
//  TODO rename class to FbChatbot

class FbChatClient : public XmppClient
{
public:
    FbChatClient(QObject *parent = 0);

    void connectToServer(const QString &user, const QString &passwd);

private:
    virtual void connectToServer(const QString &user, const QString &passwd, const QString &host);

};

} //namespace CA

} //namespace Lvk

#endif // LVK_CA_FBCHATCLIENT_H
