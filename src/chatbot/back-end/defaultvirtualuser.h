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

#ifndef LVK_BE_DEFAULTVIRTUALUSER_H
#define LVK_BE_DEFAULTVIRTUALUSER_H

#include <QObject>
#include <QStringList>
#include "chat-adapter/chatvirtualuser.h"
#include "back-end/conversation.h"

class QFile;
class QReadWriteLock;

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace Nlp
{
    class Engine;
}

namespace BE
{

/// \ingroup Lvk
/// \addtogroup BE
/// @{

class ConversationWriter;

/**
 * \brief The DefaultVirtualUser class provides the default implementation of the interface
 *        VirtualUser.
 */

class DefaultVirtualUser : public QObject, public CA::VirtualUser
{
    Q_OBJECT

public:

    /**
     * Constructs a virtual user with id \a id, NLP engine \a engine and parent object \a parent.
     */
    DefaultVirtualUser(const QString &id, Nlp::Engine *engine = 0, QObject *parent = 0);

    /**
     * Destroys the object
     */
    ~DefaultVirtualUser();

    /**
     * Returns a response for the given \a input and \a contact. If no response is found, it
     * returns an empty string.
     */
    virtual QString getResponse(const QString &input, const CA::ContactInfo &contact);

    /**
     * Returns the avatar of the virtual user.
     */
    virtual QPixmap getAvatar();

    /**
     * Returns the chat history of the current virtual user id.
     */
    const Conversation &chatHistory() const;

    /**
     * Sets the chat history of the current virtual user id.
     */
    void setChatHistory(const Conversation &conv);

    /**
     * Clears the chat history and removes any file created.
     */
    void clearHistory();

    /**
     * Sets the NLP engine that is used to get responses.
     */
    void setNlpEngine(Nlp::Engine *engine);

    /**
     * Sets the list of evasives.
     * Evasives are senteces used by the virtual user when there is no match in the NLP engine.
     * If there is no match, an evasive is chosen and returned as response.
     */
    void setEvasives(const QStringList &evasives);

signals:

    /**
     * This signal is emitted whenever the virtual user receives a chat message. \a entry
     * contains the received message, the chatbot response and other useful information.
     * \see BE::Conversation::Entry
     */
    void newConversationEntry(const BE::Conversation::Entry &entry);

private:
    DefaultVirtualUser(DefaultVirtualUser&);
    DefaultVirtualUser& operator=(DefaultVirtualUser&);

    QString m_id;
    QString m_logFilename;
    Nlp::Engine *m_engine;
    QStringList m_evasives;
    Conversation m_conversationHistory;
    ConversationWriter *m_convWriter;
    QReadWriteLock *m_rwLock;

    Conversation::Entry getEntry(const QString &input, const CA::ContactInfo &contact);
    void resetHistoryLog();
    void logConversationEntry(const Conversation::Entry &entry);
    void logError(const QString &msg);
};

/// @}


} // namespace BE

/// @}

} // namespace Lvk

#endif // LVK_BE_DEFAULTVIRTUALUSER_H
