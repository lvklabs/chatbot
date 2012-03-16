
#ifndef LVK_CA_FBCHATBOT_H
#define LVK_CA_FBCHATBOT_H

#include "xmmpchatbot.h"

namespace Lvk
{

namespace CA
{

/**
 * \brief Facebook chatbot
 *
 * This is a very thin layer over the Xmpp chatbot because fb chat is XMPP compliant
 */

class FbChatbot : public XmppChatbot
{
public:
    FbChatbot(QObject *parent = 0);

    void connectToServer(const QString &user, const QString &passwd);

private:
    virtual void connectToServer(const QString &user, const QString &passwd, const QString &host);

};

} //namespace CA

} //namespace Lvk

#endif // LVK_CA_FBCHATBOT_H
