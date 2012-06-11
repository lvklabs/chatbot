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

#include "fbchatbot.h"
#include "chatcorpus.h"

#include <QMutex>
#include <QMutexLocker>
#include <QDomElement>

#include "QXmppClient.h"
#include "QXmppMessage.h"
#include "QXmppClientExtension.h"

#define FB_CHAT_HOST        "chat.facebook.com"

//--------------------------------------------------------------------------------------------------
// ChatThreadContainer
//--------------------------------------------------------------------------------------------------
//
// ChatThreadContainer provides a chat thread thread-safe container to share
// conversations between the FbOwnMessageExtension and FbChatbot classess

namespace Lvk
{

namespace CA
{

class ChatThreadContainer
{
public:
    void append(const QString &threadId, const QString &user, const QString &body)
    {
        QMutexLocker locker(&m_mutex);
        m_entries[threadId].append(Lvk::CA::ChatCorpus::CorpusEntry(user, body));
    }

    void discard(const QString &threadId)
    {
        QMutexLocker locker(&m_mutex);
        m_entries.remove(threadId);
    }

    void persist(const QString &threadId)
    {
        QMutexLocker locker(&m_mutex);

        ChatCorpus corpus;
        foreach (const Lvk::CA::ChatCorpus::CorpusEntry &entry, m_entries[threadId]) {
            corpus.add(entry);
        }
        m_entries.remove(threadId);
    }

private:
    QMutex m_mutex;

    QHash< QString, QList<Lvk::CA::ChatCorpus::CorpusEntry> > m_entries;
};

} // namespace CA

} // namespace Lvk


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

    FbOwnMessageExtension(Lvk::CA::ChatThreadContainer *threadContainer)
        : m_chatThreads(threadContainer),
          SAVE_ORDER(QObject::tr("chatbot save").simplified().toLower()),
          DISCARD_ORDER(QObject::tr("chatbot discard").simplified().toLower()),
          USER_ME("Me")
    { }

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
        QString szBody = msg.body().simplified().toLower();

        if (szBody == SAVE_ORDER) {
            m_chatThreads->persist(msg.thread());
            client()->sendMessage(msg.to(), QObject::tr("[chatbot] Saved!"));
        } else if (szBody == DISCARD_ORDER) {
            m_chatThreads->discard(msg.thread());
            client()->sendMessage(msg.to(), QObject::tr("[chatbot] Discarded!"));
        } else {
            m_chatThreads->append(msg.thread(), USER_ME, msg.body());
        }
    }

private:
    Lvk::CA::ChatThreadContainer *m_chatThreads;

    const QString SAVE_ORDER;
    const QString DISCARD_ORDER;
    const QString USER_ME;
};

} // namespace


//--------------------------------------------------------------------------------------------------
// FbChatbot
//--------------------------------------------------------------------------------------------------

Lvk::CA::FbChatbot::FbChatbot(QObject *parent)
    : XmppChatbot(parent),
      m_chatThreads(new CA::ChatThreadContainer()),
      m_ownMsgExtension(new FbOwnMessageExtension(m_chatThreads))
{
    m_xmppClient->addExtension(m_ownMsgExtension);
}

//--------------------------------------------------------------------------------------------------

Lvk::CA::FbChatbot::~FbChatbot()
{
    m_xmppClient->removeExtension(m_ownMsgExtension);

    delete m_chatThreads;
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
            m_chatThreads->append(msg.thread(), msg.from(), msg.body());
        }
    }

    XmppChatbot::onMessageReceived(msg);
}
