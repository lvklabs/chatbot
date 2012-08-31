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

#include "back-end/aiadapter.h"
#include "nlp-engine/engine.h"
#include "chat-adapter/contactinfo.h"
#include "common/random.h"
#include "common/globalstrings.h"

#include <QDateTime>
#include <QReadWriteLock>
#include <QReadLocker>
#include <QWriteLocker>
#include <QtDebug>


//--------------------------------------------------------------------------------------------------
// Helpers
//--------------------------------------------------------------------------------------------------

namespace
{

QString getFromString(const Lvk::CA::ContactInfo &contact)
{
    return contact.fullname.size() > 0
            ? contact.fullname + " " + USERNAME_START_TOKEN + contact.username + USERNAME_END_TOKEN
            : contact.username;
}

} // namespace

//--------------------------------------------------------------------------------------------------
// AIAdapter
//--------------------------------------------------------------------------------------------------

Lvk::BE::AIAdapter::AIAdapter(const QString &id, Nlp::Engine *engine /*= 0*/)
    : m_id(id), m_engine(engine), m_rwLock(new QReadWriteLock(QReadWriteLock::Recursive))
{
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::AIAdapter::~AIAdapter()
{
    delete m_rwLock;
}

//--------------------------------------------------------------------------------------------------

Lvk::Cmn::Conversation::Entry Lvk::BE::AIAdapter::getEntry(const QString &input,
                                                           const CA::ContactInfo &contact)
{
    QWriteLocker locker(m_rwLock);

    if (m_engine) {
        qDebug() << "AIAdapter: Getting response for input" << input
                 << "and username" << contact.username;

        quint64 ruleId = 0;
        Nlp::Engine::MatchList matches;
        QString response = m_engine->getResponse(input, contact.username, matches);
        bool matched = !response.isEmpty() && matches.size() > 0;


        if (matched) {
            qDebug() << "AIAdapter: Got response" << response;

            if (matches.size() > 0) {
                ruleId = matches.first().first;
            } else {
                qWarning() << "AIAdapter: Got response but empty match list";
            }
        } else {
            if (m_evasives.size() > 0) {
                response = m_evasives[Cmn::Random::getInt(0, m_evasives.size() - 1)];
                qDebug() << "AIAdapter: No match. Using evasive" << response;
            } else {
                response.clear();
                qDebug() << "AIAdapter: No match and no evasives found";
            }
        }

        QDateTime dateTime = QDateTime::currentDateTime();
        QString from = getFromString(contact);

        return Cmn::Conversation::Entry(dateTime, from, m_id, input, response, matched, ruleId);
    } else {
        qCritical("AIAdapter: No engine set");

        return Cmn::Conversation::Entry();
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AIAdapter::setNlpEngine(Nlp::Engine *engine)
{
    QWriteLocker locker(m_rwLock);
    m_engine = engine;
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AIAdapter::setEvasives(const QStringList &evasives)
{
    QWriteLocker locker(m_rwLock);
    m_evasives = evasives;
}




