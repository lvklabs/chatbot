
#ifndef LVK_CA_CHATCLIENT_H
#define LVK_CA_CHATCLIENT_H

#include <QObject>

class QString;

namespace Lvk
{

namespace CA
{

class VirtualUser;

/**
 * \brief Abstract interface for all chat clients
 *
 */
//  TODO rename class to Chatbot

class ChatClient : public QObject
{
    Q_OBJECT

public:
    virtual ~ChatClient();

    virtual void connectToServer(const QString &user, const QString &passwd, const QString &host) = 0;
    virtual void disconnectFromServer() = 0;
    virtual void setVirtualUser(VirtualUser *virtualUser) = 0;

signals:
    void connected();
    void disconnected();
    void error(int err);
};

} // namespace CA

} // namespace Lvk

#endif // LVK_CA_CHATCLIENT_H
