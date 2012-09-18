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

#ifndef LVK_CA_FBOWNMESSAGEEXTENSION_H
#define LVK_CA_FBOWNMESSAGEEXTENSION_H

#include "QXmppClientExtension.h"

class QXmppMessage;

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
 * \brief FbOwnMessageExtension class is a QXmpp Client extension used to handle a
 *        non-standard stanza used by Facebook called "own message".
 *
 * Own messages are received whenever someone sends a message from Facebook’s web interface
 * while at the same time being logged in another client, facebook sends the following
 * non-standard IQ:
 *
   \verbatim
   <iq from="chat.facebook.com"
           to="andres.pagliano@chat.facebook.com/QXmpp_xxx"
           id="fbiq4C053818B6905"
           type="set">
       <own-message
               xmlns="http://www.facebook.com/xmpp/messages"
               to="-100003507576703@chat.facebook.com"
               self="false">
           <body>hola</body>
       </own-message>
   </iq>
   \endverbatim
*/
class FbOwnMessageExtension : public QXmppClientExtension
{
    Q_OBJECT

public:

    /**
     * Required by QXmppClientExtension interface
     */
    virtual bool handleStanza(const QDomElement &nodeRecv);

signals:

    /**
     * This signal es emited whenever an own message is received.
     */
    void ownMessage(const QXmppMessage &msg);
};

/// @}

} // namespace CA

/// @}

} // namespace Lvk


#endif // LVK_CA_FBOWNMESSAGEEXTENSION_H

