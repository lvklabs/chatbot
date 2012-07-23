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

#include "xmmpchatbot.h"
#include "chatcorpus.h"

#include <QMutex>
#include <QHash>
#include <QList>

class QXmppMessage;
class QXmppClientExtension;

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace CA
{

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
public:

    /**
     * Constructs a Facebook chatbot with parent object \a parent
     */
    FbChatbot(QObject *parent = 0);

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
    virtual void onMessageReceived(const QXmppMessage &);

private:
    virtual void connectToServer(const QString &user, const QString &passwd, const QString &host);

    QXmppClientExtension    *m_ownMsgExtension;
};

/// @}

} // namespace CA

/// @}

} // namespace Lvk

#endif // LVK_CA_FBCHATBOT_H
