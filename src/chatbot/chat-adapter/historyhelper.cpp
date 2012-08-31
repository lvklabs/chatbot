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

#include "chat-adapter/historyhelper.h"
#include "common/settings.h"
#include "common/settingskeys.h"
#include "common/conversationreader.h"
#include "common/conversationwriter.h"

#include <QFile>
#include <QDir>
#include <QReadWriteLock>
#include <QReadLocker>
#include <QWriteLocker>
#include <QtDebug>

#define HISTORY_BASE_FILENAME       "history_"
#define HISTORY_EXT_FILENAME        "dat"


//--------------------------------------------------------------------------------------------------
// HistoryHelper
//--------------------------------------------------------------------------------------------------

Lvk::CA::HistoryHelper::HistoryHelper(const QString &chatbotId)
    : m_id(chatbotId), m_convWriter(0), m_rwLock(new QReadWriteLock())
{
    init();
}

//--------------------------------------------------------------------------------------------------

Lvk::CA::HistoryHelper::~HistoryHelper()
{
    delete m_rwLock;
    delete m_convWriter;
}

//--------------------------------------------------------------------------------------------------

void Lvk::CA::HistoryHelper::init()
{
    Lvk::Cmn::Settings settings;

    QString dataPath = settings.value(SETTING_DATA_PATH).toString();
    QString logFilename = HISTORY_BASE_FILENAME + m_id + "." + HISTORY_EXT_FILENAME;
    m_filename = dataPath + QDir::separator() + logFilename;

    if (QFile::exists(m_filename)) {
        Cmn::ConversationReader convReader(m_filename);
        if (!convReader.read(&m_conv)) {
            qWarning() << "DefaultVirtualUser: Cannot read the conversation history for chatbot id"
                       << m_id;
        }
    }

    m_convWriter = new Cmn::ConversationWriter(m_filename);
}

//--------------------------------------------------------------------------------------------------

void Lvk::CA::HistoryHelper::append(const Cmn::Conversation::Entry &entry)
{
    QWriteLocker locker(m_rwLock);

    m_conv.append(entry);

    if (!m_convWriter->write(entry)) {
        qCritical() << "HistoryHelper: Cannot write the conversation entry for chatbot id"
                    << m_id;
    }
}

//--------------------------------------------------------------------------------------------------

const Lvk::Cmn::Conversation & Lvk::CA::HistoryHelper::history() const
{
    QReadLocker locker(m_rwLock);

    return m_conv;
}

//--------------------------------------------------------------------------------------------------

void Lvk::CA::HistoryHelper::setHistory(const Cmn::Conversation &conv)
{
    QWriteLocker locker(m_rwLock);

    m_conv = conv;

    resetHistoryLog();

    if (!m_convWriter->write(conv)) {
        qCritical() << "HistoryHelper: Cannot write the conversation entry for chatbot id"
                    << m_id;
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::CA::HistoryHelper::clear()
{
    QWriteLocker locker(m_rwLock);

    m_conv = Cmn::Conversation();

    resetHistoryLog();
}

//--------------------------------------------------------------------------------------------------

void Lvk::CA::HistoryHelper::resetHistoryLog()
{
    delete m_convWriter;

    QFile::remove(m_filename);

    m_convWriter = new Cmn::ConversationWriter(m_filename);
}

