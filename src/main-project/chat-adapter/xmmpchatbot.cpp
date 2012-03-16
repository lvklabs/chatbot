#include "xmmpchatbot.h"
#include "chatvirtualuser.h"

#include "QXmppClient.h"
#include "QXmppMessage.h"
#include "QXmppVCardManager.h"
#include "QXmppVCardIq.h"

//#include <iostream>

#define VAR_NICKNAME    "$NICKNAME"

//--------------------------------------------------------------------------------------------------

Lvk::CA::XmppChatbot::XmppChatbot(QObject *parent)
    : m_xmppClient(new QXmppClient(parent)), m_virtualUser(0)
{
    connect(m_xmppClient, SIGNAL(messageReceived(const QXmppMessage&)),
            this, SLOT(messageReceived(const QXmppMessage&)));

    connect(&m_xmppClient->vCardManager(), SIGNAL(vCardReceived(const QXmppVCardIq&)),
            this, SLOT(vCardReceived(const QXmppVCardIq&)));

    connect(m_xmppClient, SIGNAL(connected()),    SIGNAL(connected()));
    connect(m_xmppClient, SIGNAL(disconnected()), SIGNAL(disconnected()));

    connect(m_xmppClient, SIGNAL(error(QXmppClient::Error)),
            SLOT(emitLocalError(QXmppClient::Error)));
}

//--------------------------------------------------------------------------------------------------

Lvk::CA::XmppChatbot::~XmppChatbot()
{
    delete m_virtualUser;
    delete m_xmppClient;
}

//--------------------------------------------------------------------------------------------------

void Lvk::CA::XmppChatbot::messageReceived(const QXmppMessage& message)
{
    if (message.type() != QXmppMessage::Chat) {
        return;
    }
    if (!m_virtualUser) {
        return;
    }
    if (message.body().isEmpty()) {
        return;
    }

    QString from = message.from();
    QString body = message.body();
    QString bareJid = from.split("/").at(0);

    QString response = m_virtualUser->getResponse(body, bareJid);

    //    if (body.contains(VAR_NICKNAME)) {
    //        if (m_nickname.isEmpty()) {
    //            vCardManager().requestVCard(from);
    //            m_queuedMessage = message;
    //            return;
    //        } else {
    //            body.replace(VAR_NICKNAME, m_nickname);
    //        }
    //    }

    if (!response.isEmpty()) {
        m_xmppClient->sendPacket(QXmppMessage("", from, response));
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::CA::XmppChatbot::vCardReceived(const QXmppVCardIq &/*vCard*/)
{
//    std::cout << "============ vCARD ============" << std::endl;
//    std::cout << "fullName:" << vCard.fullName().toStdString()  << std::endl;
//    std::cout << "firstName:"<< vCard.firstName().toStdString() << std::endl;
//    std::cout << "lastName:" << vCard.lastName().toStdString()  << std::endl;
//    std::cout << "nickName:" << vCard.nickName().toStdString()  << std::endl;
//    std::cout << "birthday:" << vCard.birthday().toString().toStdString() << std::endl;
//    std::cout << "email:"    << vCard.email().toStdString()     << std::endl;
//    std::cout << "==============================="              << std::endl;

//    if (!vCard.nickName().isEmpty()) {
//        m_nickname = vCard.nickName();
//    } else if (!vCard.firstName().isEmpty()) {
//        m_nickname = vCard.firstName();
//    } else if (!vCard.fullName().isEmpty()) {
//        m_nickname = vCard.fullName().split(" ").at(0);
//    } else {
//        m_nickname = "Unkown";
//    }

//    // Send queued message
//    if (m_queuedMessage.from() != "") {
//        messageReceived(m_queuedMessage);
//        m_queuedMessage = QXmppMessage("");
//    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::CA::XmppChatbot::connectToServer(const QString &user, const QString &passwd,
                                          const QString &host)
{
    m_xmppClient->connectToServer(user + "@" + host, passwd);
}

//--------------------------------------------------------------------------------------------------

void Lvk::CA::XmppChatbot::disconnectFromServer()
{
    m_xmppClient->disconnectFromServer();
}

//--------------------------------------------------------------------------------------------------

void Lvk::CA::XmppChatbot::setVirtualUser(Lvk::CA::VirtualUser *virtualUser)
{
    if (m_virtualUser != virtualUser) {
        delete m_virtualUser;
        m_virtualUser = virtualUser;
    }
}

//--------------------------------------------------------------------------------------------------

Lvk::CA::VirtualUser * Lvk::CA::XmppChatbot::virtualUser()
{
    return m_virtualUser;
}

//--------------------------------------------------------------------------------------------------

void Lvk::CA::XmppChatbot::emitLocalError(QXmppClient::Error err)
{
    emit error(convertToLocalError(err));
}

//--------------------------------------------------------------------------------------------------

Lvk::CA::XmppChatbot::Error Lvk::CA::XmppChatbot::convertToLocalError(QXmppClient::Error err)
{
    switch (err) {
    case QXmppClient::SocketError:
        return XmppChatbot::SocketError;
    case QXmppClient::KeepAliveError:
        return XmppChatbot::KeepAliveError;
    case QXmppClient::XmppStreamError:
        return XmppChatbot::XmppStreamError;
    default:
        return XmppChatbot::InternalError;
    }
}
