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
#include "chat-adapter/fbownmessageextension.h"
#include "common/conversation.h"
#include "common/settings.h"
#include "common/settingskeys.h"
#include "common/globalstrings.h"

#include <QDomElement>
#include <QtDebug>

#include "QXmppClient.h"
#include "QXmppMessage.h"
#include "QXmppClientExtension.h"

#define FB_CHAT_HOST        "chat.facebook.com"


//--------------------------------------------------------------------------------------------------
// Helpers
//--------------------------------------------------------------------------------------------------

namespace
{

inline Lvk::CA::ChatCorpus::CorpusEntry makeCorpusEntry(const QString &user, const QString &msg)
{
    Lvk::CA::ChatCorpus::CorpusEntry entry;

    entry.timestamp = QDateTime::currentDateTime();
    entry.username = user;
    entry.message = msg;

    return entry;
}

//--------------------------------------------------------------------------------------------------

inline Lvk::Cmn::Conversation::Entry makeEntry(const QString &from, const QString &to,
                                               const QString &msg)
{
    Lvk::Cmn::Conversation::Entry entry;

    entry.dateTime = QDateTime::currentDateTime();
    entry.from = from;
    entry.msg = msg;
    entry.to = to;

    return entry;
}

} // namespace

//--------------------------------------------------------------------------------------------------
// FbChatbot
//--------------------------------------------------------------------------------------------------

Lvk::CA::FbChatbot::FbChatbot(const QString &chatbotId, QObject *parent)
    : XmppChatbot(chatbotId, parent), m_ownMsgExtension(new CA::FbOwnMessageExtension())
{
    connect(m_ownMsgExtension,
            SIGNAL(ownMessage(QXmppMessage)),
            SLOT(onOwnMessageReceived(QXmppMessage)));

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
            CA::ChatCorpus().add(makeCorpusEntry(msg.from(), msg.body()));
        }
    }

    XmppChatbot::onMessageReceived(msg);
}

//--------------------------------------------------------------------------------------------------

void Lvk::CA::FbChatbot::onOwnMessageReceived(const QXmppMessage &msg)
{
    qDebug() << "FbChatbot OwnMessageReceived" << msg.body() << "to" << msg.to();

    CA::ChatCorpus().add(makeCorpusEntry(OWN_MESSAGE_TOKEN, msg.body()));

    emit newConversationEntry(makeEntry(OWN_MESSAGE_TOKEN, msg.to(), msg.body()));
}
