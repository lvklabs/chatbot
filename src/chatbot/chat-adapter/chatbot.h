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

#ifndef LVK_CA_CHATBOT_H
#define LVK_CA_CHATBOT_H

#include <QObject>

#include "chat-adapter/contactinfo.h"

class QString;

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace CA
{

/// \ingroup Lvk
/// \addtogroup CA
/// @{

class VirtualUser;


/**
 * \brief The Chatbot class provides the abstract interface for all chatbots.
 *
 * A chatbot connects to a chat server and waits for a conversation to start. If a message arrives
 * it replies but it does not start conversations by its own.
 * Before connecting to a chat server a VirtualUser must be set with setVirtualUser().
 * The VirtualUser class provides the information to behave like a human being. Most
 * notably, the virtual user knows how to reply a contact given a sentence.
 *
 * \see VirtualUser, XmppChatbot, FbChatbot, GTalkChatbot
 */
class Chatbot : public QObject
{
    Q_OBJECT

public:

    /**
     * Destroys the chatbot and disconnects from the chat server if nescessary.
     */
    virtual ~Chatbot();

    /**
     * Connects to the default chat server using \a user and \a password.
     * Emits \a connected signal on success. Otherwise; emits \a connectionError.
     * If the connection ends prematurely, emits \a disconnected.
     */
    virtual void connectToServer(const QString &user, const QString &password) = 0;

    /**
     * Connects to the chat server at \a domain using \a user and \a password
     * Emits \a connected signal on success. Otherwise; emits \a connectionError.
     * If the connection ends prematurely, emits \a disconnected.
     */
    virtual void connectToServer(const QString &user, const QString &password,
                                 const QString &domain) = 0;

    /**
     * Disconnects from chat server.
     */
    virtual void disconnectFromServer() = 0;

    /**
     * Sets the virtual user.
     */
    virtual void setVirtualUser(VirtualUser *virtualUser) = 0;

    /**
     * Returns the virtual user.
     */
    virtual VirtualUser *virtualUser() = 0;

    /**
     * Returns the roster. i.e. the list of all chat contacts.
     */
    virtual ContactInfoList roster() const = 0;

    /**
     * Sets the black list roster \a blackList.
     * The black list roster is the list of contacts that the chatbot is forbidden to talk to.
     * By default, this list is empty.
     */
    virtual void setBlackListRoster(const ContactInfoList &blackList) = 0;

    /**
     * Returns the black list roster.
     * The black list roster is the list of contacts that the chatbot is forbidden to talk to.
     * By default, this list is empty.
     */
    virtual ContactInfoList blackListRoster() const = 0;

signals:

    /**
     * This signal is emitted after invoking connectToServer() if the connection was successful.
     */
    void connected();

    /**
     * This signal is emitted after invoking connectToServer() if the connection has ended.
     */
    void disconnected();

    /**
     * This signal es emitted after invoking connectToServer() if there was an error while trying
     * to connect to the chat server.
     */
    void error(int err);
};

/// @}

} // namespace CA

/// @}

} // namespace Lvk

#endif // LVK_CA_CHATBOT_H
