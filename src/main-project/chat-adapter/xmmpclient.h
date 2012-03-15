
#ifndef LVK_CA_XMPPCLIENT_H
#define LVK_CA_XMPPCLIENT_H

#include <QObject>

#include "QXmppClient.h"
#include "QXmppMessage.h"
#include "chatclient.h"

class QXmppVCardIq;

namespace Lvk
{

namespace CA
{

/**
 * \brief Implementation of the ChatClient interface to provide connection to XMPP chat servers
 */
//  TODO rename class to XmppChatbot

class XmppClient : public ChatClient
{
    Q_OBJECT

public:
    XmppClient(QObject *parent = 0);

    ~XmppClient();

    /**
     * \brief An enumeration for type of error.
     */
    enum Error {
        SocketError,        ///< Error due to TCP socket
        KeepAliveError,     ///< Error due to no response to a keep alive
        XmppStreamError,    ///< Error due to XML stream
        InternalError       ///< Internal error
    };

    // Lvk::CA::ChatClient interface

    virtual void connectToServer(const QString &user, const QString &passwd, const QString &host);

    virtual void disconnectFromServer();

    virtual void setVirtualUser(VirtualUser *virtualUser);

public slots:
    void messageReceived(const QXmppMessage&);

    void vCardReceived(const QXmppVCardIq&);

signals:
    void connected();

    void disconnected();

    void error(int err);

private slots:
    void emitLocalError(QXmppClient::Error);

private:
    QXmppClient *m_xmppClient;
    //QString m_nickname;
    //QXmppMessage m_queuedMessage;
    VirtualUser *m_virtualUser;

    Error convertToLocalError(QXmppClient::Error err);
};

} //namespace CA

} //namespace Lvk

#endif // LVK_CA_XMPPCLIENT_H
