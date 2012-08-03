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

#include "back-end/defaultvirtualuser.h"
#include "nlp-engine/engine.h"
#include "common/random.h"
#include "common/settings.h"
#include "common/settingskeys.h"
#include "back-end/conversationreader.h"
#include "back-end/conversationwriter.h"

#include <QFile>
#include <QDir>
#include <QDateTime>
#include <QReadWriteLock>
#include <QReadLocker>
#include <QWriteLocker>
#include <QtDebug>

#define HISTORY_BASE_FILENAME       "history_"
#define HISTORY_EXT_FILENAME        "dat"
#define DATE_TIME_LOG_FORMAT        "dd-MM-yy hh:mm:ss"

// Note: ConversationHistoryWidget relies on these tokens.
//       If you change them, update the widget accordingly.
#define USERNAME_START_TOKEN    "<"
#define USERNAME_END_TOKEN      ">"


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
// DefaultVirtualUser
//--------------------------------------------------------------------------------------------------

Lvk::BE::DefaultVirtualUser::DefaultVirtualUser(const QString &id,
                                                Nlp::Engine *engine /*= 0*/,
                                                QObject *parent /*= 0*/)
    : QObject(parent),
      m_id(id),
      m_engine(engine),
      m_convWriter(0),
      m_rwLock(new QReadWriteLock())
{
    Lvk::Cmn::Settings settings;

    QString dataPath = settings.value(SETTING_DATA_PATH).toString();
    QString logFilename = HISTORY_BASE_FILENAME + id + "." + HISTORY_EXT_FILENAME;
    m_logFilename = dataPath + QDir::separator() + logFilename;

    if (QFile::exists(m_logFilename)) {
        BE::ConversationReader convReader(m_logFilename);
        if (!convReader.read(&m_conversationHistory)) {
            qWarning() << "DefaultVirtualUser: Cannot read the conversation history for chatbot id"
                       << id;
        }
    }

    m_convWriter = new BE::ConversationWriter(m_logFilename);
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::DefaultVirtualUser::~DefaultVirtualUser()
{
    delete m_rwLock;
    delete m_convWriter;
}

//--------------------------------------------------------------------------------------------------

QString Lvk::BE::DefaultVirtualUser::getResponse(const QString &input,
                                                 const CA::ContactInfo &contact)
{
    Conversation::Entry entry = getEntry(input, contact);

    {
        QWriteLocker locker(m_rwLock);

        m_conversationHistory.append(entry);

        if (!m_convWriter->write(entry)) {
            qCritical() << "DefaultVirtualUser: Cannot write the conversation entry for chatbot id"
                        << m_id;
        }
    }

    emit newConversationEntry(entry);

    return entry.response;
}

//--------------------------------------------------------------------------------------------------


Lvk::BE::Conversation::Entry Lvk::BE::DefaultVirtualUser::getEntry(const QString &input,
                                                                   const CA::ContactInfo &contact)
{
    QWriteLocker locker(m_rwLock);

    if (m_engine) {
        qDebug() << "DefaultVirtualUser: Getting response for input" << input
                 << "and username" << contact.username;

        quint64 ruleId = 0;
        Nlp::Engine::MatchList matches;
        QString response = m_engine->getResponse(input, contact.username, matches);
        bool matched = !response.isEmpty() && matches.size() > 0;


        if (matched) {
            qDebug() << "DefaultVirtualUser: Got response" << response;

            if (matches.size() > 0) {
                ruleId = matches.first().first; // CHECK first() or last()
            } else {
                qWarning() << "DefaultVirtualUser: Got response but empty match list";
            }
        } else {
            if (m_evasives.size() > 0) {
                response = m_evasives[Cmn::Random::getInt(0, m_evasives.size() - 1)];
                qDebug() << "DefaultVirtualUser: No match. Using evasive" << response;
            } else {
                response.clear();
                qDebug() << "DefaultVirtualUser: No match and no evasives found";
            }
        }

        QDateTime dateTime = QDateTime::currentDateTime();
        QString from = getFromString(contact);

        return Conversation::Entry(dateTime, from, m_id, input, response, matched, ruleId);
    } else {
        qCritical("DefaultVirtualUser: No engine set");

        return Conversation::Entry();
    }
}

//--------------------------------------------------------------------------------------------------

QPixmap Lvk::BE::DefaultVirtualUser::getAvatar()
{
    return QPixmap();   // For future usage
}

//--------------------------------------------------------------------------------------------------

const Lvk::BE::Conversation & Lvk::BE::DefaultVirtualUser::chatHistory() const
{
    QReadLocker locker(m_rwLock);
    return m_conversationHistory;
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::DefaultVirtualUser::setChatHistory(const Lvk::BE::Conversation &conv)
{
    QWriteLocker locker(m_rwLock);

    m_conversationHistory = conv;

    resetHistoryLog();

    if (!m_convWriter->write(conv)) {
        qCritical() << "DefaultVirtualUser: Cannot write the conversation entry for chatbot id"
                    << m_id;
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::DefaultVirtualUser::setNlpEngine(Nlp::Engine *engine)
{
    QWriteLocker locker(m_rwLock);
    m_engine = engine;
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::DefaultVirtualUser::setEvasives(const QStringList &evasives)
{
    QWriteLocker locker(m_rwLock);
    m_evasives = evasives;
}


//--------------------------------------------------------------------------------------------------

void Lvk::BE::DefaultVirtualUser::resetHistoryLog()
{
    delete m_convWriter;

    QFile::remove(m_logFilename);

    m_convWriter = new BE::ConversationWriter(m_logFilename);
}
