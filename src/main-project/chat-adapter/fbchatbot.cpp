/*
 * Copyright (C) 2012 Andres Pagliano, Gabriel Miretti, Gonzalo Buteler,
 * Nestor Bustamante, Pablo Perez de Angelis
 *
 * This file is part of LVK Botmaster.
 *
 * LVK Botmaster is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * LVK Botmaster is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LVK Botmaster.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "fbchatbot.h"

#include <QDomElement>

#include "QXmppClient.h"
#include "QXmppMessage.h"
#include "QXmppClientExtension.h"

//#include <iostream>

#define FB_CHAT_HOST       "chat.facebook.com"

//namespace
//{

//--------------------------------------------------------------------------------------------------
// FbOwnMessageExtension
//--------------------------------------------------------------------------------------------------

// Whenever someone sends a message from facebook’s web interface while at the same time being
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

        if(nodeRecv.tagName() == "iq")
        {
            QDomElement child = nodeRecv.firstChildElement();

            if (child.tagName() == "own-message") {
                QXmppMessage message;
                message.parse(child);

                //emit messageReceived(message);

                handled = true;
            }
        }

        return handled;
    }
};

//--------------------------------------------------------------------------------------------------

//} // namespace


//--------------------------------------------------------------------------------------------------
// FbChatbot
//--------------------------------------------------------------------------------------------------

Lvk::CA::FbChatbot::FbChatbot(QObject *parent)
    : XmppChatbot(parent)
{
    m_xmppClient->addExtension(new FbOwnMessageExtension());
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
