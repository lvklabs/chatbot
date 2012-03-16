
#ifndef LVK_CA_GTALKCHATBOT_H
#define LVK_CA_GTALKCHATBOT_H

#include "xmmpchatbot.h"

namespace Lvk
{

namespace CA
{

/**
 * \brief GTalk chatbot
 *
 * This is a very thin layer over the Xmpp chatbot because GTalk chat is XMPP compliant
 */

class GTalkChatbot : public XmppChatbot
{
public:
    GTalkChatbot(QObject *parent = 0);

    void connectToServer(const QString &user, const QString &passwd);

private:
    virtual void connectToServer(const QString &user, const QString &passwd, const QString &host);
};

} //namespace CA

} //namespace Lvk

#endif // LVK_CA_GTALKCHATBOT_H
