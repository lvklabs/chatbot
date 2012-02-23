#include "XmppClient.h"

#include "QXmppMessage.h"
#include "QXmppVCardManager.h"
#include "QXmppVCardIq.h"

#include <QFile>
#include <QTextStream>

#include <exception>
#include <iostream>

#include "aimlparser.h"
#include "RemoteLogger.h"

#define VAR_NICKNAME    "$NICKNAME"


XmppClient::XmppClient(QObject *parent)
    : QXmppClient(parent)
{
    connect(this,            SIGNAL(messageReceived(const QXmppMessage&)), SLOT(messageReceived(const QXmppMessage&)));
    connect(&vCardManager(), SIGNAL(vCardReceived(const QXmppVCardIq&)),   SLOT(vCardReceived(const QXmppVCardIq&)));


    //Create the log file
    QFile file("debug.log");
    if ( !file.open( IO_WriteOnly ) ) {
        std::cout << "FATAL: cannot write debug.log" << std::endl;
        RemoteLogger().logCritical("Cannot write debug.log");
        throw std::exception();
    }

    static QTextStream logStream;
    logStream.setDevice(&file);
    logStream.setEncoding(QTextStream::Unicode);

    m_parser = new AIMLParser(&logStream);

    //m_parser->loadVars("vars.aiml", false);
    //m_parser->loadSubstitutions("substitutions.aiml");

    if ( !m_parser->loadAiml("rules.aiml") ) {
        std::cout << "FATAL: cannot open rules.aiml" << std::endl;
        RemoteLogger().logCritical("Cannot open rules.aiml");
        throw std::exception();
    }

    RemoteLogger().logInfo("Client Connected");
}


XmppClient::~XmppClient()
{
    RemoteLogger().logInfo("Client Disconnected");
}


void XmppClient::messageReceived(const QXmppMessage& message)
{
    if (message.type() != QXmppMessage::Chat) {
        return;
    }

    QString from = message.from();
    QString msg = message.body();

    RemoteLogger().logInfo("Bot receives '" + msg + "'");

    bool srai = false;
    do {
        msg = m_parser->getResponse(msg, srai);
    } while (srai);

    if (msg.contains(VAR_NICKNAME)) {
        if (m_nickname.isEmpty()) {
            vCardManager().requestVCard(from);
            m_queuedMessage = message;
            return;
        } else {
            msg.replace(VAR_NICKNAME, m_nickname);
        }
    }

    QString response = (msg != "Internal Error!") ? msg : "Sorry, I can't understand: " + message.body();

    RemoteLogger().logInfo("Bot replies '" + response + "'");

    sendPacket(QXmppMessage("", from, response));
}


void XmppClient::vCardReceived(const QXmppVCardIq &vCard)
{
    std::cout << "============ vCARD ============" << std::endl;
    std::cout << "fullName:" << vCard.fullName().toStdString()  << std::endl;
    std::cout << "firstName:"<< vCard.firstName().toStdString() << std::endl;
    std::cout << "lastName:" << vCard.lastName().toStdString()  << std::endl;
    std::cout << "nickName:" << vCard.nickName().toStdString()  << std::endl;
    std::cout << "birthday:" << vCard.birthday().toString().toStdString() << std::endl;
    std::cout << "email:"    << vCard.email().toStdString()     << std::endl;
    std::cout << "==============================="              << std::endl;

    if (!vCard.nickName().isEmpty()) {
        m_nickname = vCard.nickName();
    } else if (!vCard.firstName().isEmpty()) {
        m_nickname = vCard.firstName();
    } else if (!vCard.fullName().isEmpty()) {
        m_nickname = vCard.fullName().split(" ").at(0);
    } else {
        m_nickname = "Unkown";
    }

    // Send queued message
    if (m_queuedMessage.from() != "") {
        messageReceived(m_queuedMessage);
        m_queuedMessage = QXmppMessage("");
    }
}
