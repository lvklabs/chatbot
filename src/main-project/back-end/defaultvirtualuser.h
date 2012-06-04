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

#ifndef LVK_BE_DEFAULTVIRTUALUSER_H
#define LVK_BE_DEFAULTVIRTUALUSER_H

#include <QObject>
#include <QStringList>
#include "chatvirtualuser.h"
#include "conversation.h"

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
 * \brief Default implementation of the Abstract class Virtual User
 */

class DefaultVirtualUser : public QObject, public CA::VirtualUser
{
    Q_OBJECT

public:
    DefaultVirtualUser(const QString &id, Nlp::Engine *engine = 0, QObject *parent = 0);

    ~DefaultVirtualUser();

    virtual QString getResponse(const QString &input, const CA::ContactInfo &contact);

    virtual QPixmap getAvatar();

    const Conversation &getConversationHistory() const;

    void setNlpEngine(Nlp::Engine *engine);

    void setEvasives(const QStringList &evasives);

signals:
    void newConversationEntry(const BE::Conversation::Entry &entry);

private:
    DefaultVirtualUser(DefaultVirtualUser&);
    DefaultVirtualUser& operator=(DefaultVirtualUser&);

    QString m_id;
    Nlp::Engine *m_engine;
    QStringList m_evasives;
    Conversation m_conversationHistory;
    ConversationWriter *m_convWriter;

    QReadWriteLock *m_rwLock;

    void getResponse(QString &response, bool &match, const QString &input, const QString &username);
    void logConversationEntry(const Conversation::Entry &entry);
    void logError(const QString &msg);
};

/// @}


} // namespace BE

/// @}

} // namespace Lvk

#endif // LVK_BE_DEFAULTVIRTUALUSER_H
