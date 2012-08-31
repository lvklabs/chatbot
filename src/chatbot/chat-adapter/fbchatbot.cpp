/*
 * Copyright (C) 2012 Andres Pagliano, Gabriel Miretti, Gonzalo Buteler,
 * Nestor Bustamante, Pablo Perez de Angelis
 *
 * This file is part of LVK Chatbot.
 *
 * LVK Chatbot is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * LVK Chatbot is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LVK Chatbot.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "chat-adapter/fbchatbot.h"
#include "chat-adapter/chatcorpus.h"
#include "common/settings.h"
#include "common/settingskeys.h"
#include "common/globalstrings.h"

#include <QDomElement>

#include "QXmppClient.h"
#include "QXmppMessage.h"
#include "QXmppClientExtension.h"

#define FB_CHAT_HOST        "chat.facebook.com"


//--------------------------------------------------------------------------------------------------
// Helpers
//--------------------------------------------------------------------------------------------------

namespace
{

inline Lvk::CA::ChatCorpus::CorpusEntry makeEntry(const QString &user, const QString &msg)
{
    Lvk::CA::ChatCorpus::CorpusEntry entry;

    entry.timestamp = QDateTime::currentDateTime();
    entry.username = user;
    entry.message = msg;

    return entry;
}

} // namespace


namespace
{

//--------------------------------------------------------------------------------------------------
// FbOwnMessageExtension
//--------------------------------------------------------------------------------------------------
//
// FbOwnMessageExtension class is a QXmpp Client extension used to handle a non-standard stanza
// used by Facebook.
// Whenever someone sends a message from Facebook’s web interface while at the same time being
// logged in another client, facebook sends the following non-standard IQ:
//
// <iq from="chat.facebook.com"
//         to="andres.pagliano@chat.facebook.com/QXmpp_xxx"
//         id="fbiq4C053818B6905"
//         type="set">
//     <own-message
//             xmlns="http://www.facebook.com/xmpp/messages"
//             to="-100003507576703@chat.facebook.com"
//             self="false">
//         <body>hola</body>
//     </own-message>
// </iq>

class FbOwnMessageExtension : public QXmppClientExtension
{
public:

    virtual bool handleStanza(const QDomElement &nodeRecv)
    {
        bool handled = false;

        if (nodeRecv.tagName() == "iq") {
            QDomElement child = nodeRecv.firstChildElement();

            if (child.tagName() == "own-message") {
                QXmppMessage msg;
                msg.parse(child);

                handleOwnMessage(msg);

                handled = true;
            }
        }

        return handled;
    }

    void handleOwnMessage(const QXmppMessage &msg)
    {
        Lvk::CA::ChatCorpus corpus;
        corpus.add(makeEntry("Me", msg.body()));
    }
};

} // namespace


//--------------------------------------------------------------------------------------------------
// FbChatbot
//--------------------------------------------------------------------------------------------------

Lvk::CA::FbChatbot::FbChatbot(const QString &chatbotId, QObject *parent)
    : XmppChatbot(chatbotId, parent), m_ownMsgExtension(new FbOwnMessageExtension())
{
    m_xmppClient->addExtension(m_ownMsgExtension);
}

//--------------------------------------------------------------------------------------------------

Lvk::CA::FbChatbot::~FbChatbot()
{
    m_xmppClient->removeExtension(m_ownMsgExtension);
}

//--------------------------------------------------------------------------------------------------

void Lvk::CA::FbChatbot::connectToServer(const QString &user, const QString &passwd)
{
    return XmppChatbot::connectToServer(user, passwd, FB_CHAT_HOST);
}

//--------------------------------------------------------------------------------------------------

void Lvk::CA::FbChatbot::connectToServer(const QString &user, const QString &passwd,
                                         const QString &host)
{
    return XmppChatbot::connectToServer(user, passwd, host);
}

//--------------------------------------------------------------------------------------------------

void Lvk::CA::FbChatbot::onMessageReceived(const QXmppMessage &msg)
{
    if (msg.type() == QXmppMessage::Chat || msg.type() == QXmppMessage::GroupChat) {
        if (msg.body().size() > 0) {
            ChatCorpus corpus;
            corpus.add(makeEntry(msg.from(), msg.body()));
        }
    }

    XmppChatbot::onMessageReceived(msg);
}
