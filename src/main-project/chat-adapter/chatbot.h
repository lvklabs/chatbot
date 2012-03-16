
#ifndef LVK_CA_CHATBOT_H
#define LVK_CA_CHATBOT_H

#include <QObject>

class QString;

namespace Lvk
{

namespace CA
{

class VirtualUser;

/**
 * \brief Abstract interface for all chatbots
 *
 */

class Chatbot : public QObject
{
    Q_OBJECT

public:
    virtual ~Chatbot();

    virtual void connectToServer(const QString &user, const QString &passwd, const QString &host) = 0;
    virtual void disconnectFromServer() = 0;
    virtual void setVirtualUser(VirtualUser *virtualUser) = 0;
    virtual VirtualUser *virtualUser() = 0;

signals:
    void connected();
    void disconnected();
    void error(int err);
};

} // namespace CA

} // namespace Lvk

#endif // LVK_CA_CHATBOT_H
