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

#ifndef LVK_CA_FBCHATBOT_H
#define LVK_CA_FBCHATBOT_H

#include "chat-adapter/xmppchatbot.h"
#include "chat-adapter/chatcorpus.h"

#include <QObject>
#include <QHash>
#include <QList>

class QXmppMessage;

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace CA
{

class FbOwnMessageExtension;

/// \ingroup Lvk
/// \addtogroup CA
/// @{

/**
 * \brief The FbChatbot class provides a chatbot for Facebook Chat.
 *
 * \see XmppChatbot
 */

class FbChatbot : public XmppChatbot
{
    Q_OBJECT

public:

    /**
     * Constructs a Facebook chatbot with ID \a chatbotId parent object \a parent
     */
    FbChatbot(const QString &chatbotId, QObject *parent = 0);

    /**
     * Destroys the object
     */
    ~FbChatbot();

    /**
     * Connects to the Facebook Chat server using \a user and \a password.
     * Emits \a connected signal on success. Otherwise; emits \a connectionError.
     * If the connection ends prematurely, emits \a disconnected.
     */
    virtual void connectToServer(const QString &user, const QString &passwd);

protected slots:
    virtual void onMessageReceived(const QXmppMessage &msg);
    virtual void onOwnMessageReceived(const QXmppMessage &msg);

private:
    FbChatbot(const FbChatbot&);
    FbChatbot & operator=(const FbChatbot&);

    virtual void connectToServer(const QString &user, const QString &passwd, const QString &host);

    FbOwnMessageExtension    *m_ownMsgExtension;
};

/// @}

} // namespace CA

/// @}

} // namespace Lvk

#endif // LVK_CA_FBCHATBOT_H
