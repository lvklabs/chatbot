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

#include "defaultvirtualuser.h"
#include "nlpengine.h"
#include "random.h"
#include "settings.h"
#include "settingskeys.h"
#include "conversationreader.h"
#include "conversationwriter.h"

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
            qWarning() << "DefaultVirtualUser cannot read the conversation history for chatbot id"
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
    // get response thread-safe

    QString response;
    bool matched;
    getResponse(response, matched, input, contact.username);

    // Make entry and log it

    QDateTime dateTime = QDateTime::currentDateTime();
    Conversation::Entry entry(dateTime, getFromString(contact), m_id, input, response, matched);

    {
        QWriteLocker locker(m_rwLock);

        m_conversationHistory.append(entry);

        if (!m_convWriter->write(entry)) {
            qCritical() << "DefaultVirtualUser cannot write the conversation entry for chatbot id"
                        << m_id;
        }
    }

    emit newConversationEntry(entry);

    return response;
}

//--------------------------------------------------------------------------------------------------


void Lvk::BE::DefaultVirtualUser::getResponse(QString &response, bool &matched,
                                              const QString &input, const QString &username)
{
    QWriteLocker locker(m_rwLock);

    if (m_engine) {
        Nlp::Engine::MatchList matches;
        response = m_engine->getResponse(input, username, matches);
        matched = !response.isEmpty() && matches.size() > 0;

        if (!matched) {
            if (m_evasives.size() > 0) {
                response = m_evasives[Cmn::Random::getInt(0, m_evasives.size() - 1)];
            } else {
                response.clear();
            }
        }
    } else {
        qWarning("No engine set in DefaultVirtualUser!");
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
        qCritical() << "DefaultVirtualUser cannot write the conversation entry for chatbot id"
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
