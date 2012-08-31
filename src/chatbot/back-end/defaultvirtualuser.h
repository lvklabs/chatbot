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
#include "chat-adapter/virtualuser.h"
#include "common/conversation.h"

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

namespace Cmn
{
    class ConversationWriter;
}

namespace BE
{

/// \ingroup Lvk
/// \addtogroup BE
/// @{

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
     * \copydoc CA::VirtualUser::getEntry()
     */
    virtual Cmn::Conversation::Entry getEntry(const QString &input,
                                              const CA::ContactInfo &contact);

    /**
     * \copydoc CA::VirtualUser::getAvatar()
     */
    virtual QPixmap getAvatar();

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

private:
    DefaultVirtualUser(DefaultVirtualUser&);
    DefaultVirtualUser& operator=(DefaultVirtualUser&);

    QString m_id;
    Nlp::Engine *m_engine;
    QStringList m_evasives;
    QReadWriteLock *m_rwLock;

};

/// @}


} // namespace BE

/// @}

} // namespace Lvk

#endif // LVK_BE_DEFAULTVIRTUALUSER_H
