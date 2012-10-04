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
#include "common/conversationreader.h"
#include "common/conversationwriter.h"

#include <QFile>
#include <QDir>
#include <QReadWriteLock>
#include <QReadLocker>
#include <QWriteLocker>
#include <QtDebug>

//--------------------------------------------------------------------------------------------------
// HistoryHelper
//--------------------------------------------------------------------------------------------------

Lvk::CA::HistoryHelper::HistoryHelper()
    : m_filename(),
      m_convWriter(new Cmn::ConversationWriter()),
      m_rwLock(new QReadWriteLock())
{
}

//--------------------------------------------------------------------------------------------------

Lvk::CA::HistoryHelper::HistoryHelper(const QString &filename)
    : m_filename(filename),
      m_convWriter(new Cmn::ConversationWriter(m_filename)),
      m_rwLock(new QReadWriteLock())
{
    load();
}

//--------------------------------------------------------------------------------------------------

Lvk::CA::HistoryHelper::~HistoryHelper()
{
    delete m_rwLock;
    delete m_convWriter;
}

//--------------------------------------------------------------------------------------------------

void Lvk::CA::HistoryHelper::load()
{
    if (QFile::exists(m_filename)) {
        Cmn::ConversationReader convReader(m_filename);
        m_conv.clear();
        if (!convReader.read(&m_conv)) {
            qWarning() << "HistoryHelper: Cannot read the conversation history from file"
                       << m_filename;
        }
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::CA::HistoryHelper::setFilename(const QString &filename)
{
    QWriteLocker locker(m_rwLock);

    m_filename = filename;

    load();

    delete m_convWriter;
    m_convWriter = new Cmn::ConversationWriter(m_filename);
}

//--------------------------------------------------------------------------------------------------

QString Lvk::CA::HistoryHelper::filename() const
{
    QWriteLocker locker(m_rwLock);

    return m_filename;
}

//--------------------------------------------------------------------------------------------------

void Lvk::CA::HistoryHelper::append(const Cmn::Conversation::Entry &entry)
{
    QWriteLocker locker(m_rwLock);

    m_conv.append(entry);

    if (!m_convWriter->write(entry)) {
        qCritical() << "HistoryHelper: Cannot write the conversation entry to file" << m_filename;
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

    resetHistoryLog();

    m_conv = conv;

    if (!m_convWriter->write(conv)) {
        qCritical() << "HistoryHelper: Cannot write the conversation to file" << m_filename;
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::CA::HistoryHelper::clear()
{
    QWriteLocker locker(m_rwLock);

    resetHistoryLog();
}

//--------------------------------------------------------------------------------------------------

void Lvk::CA::HistoryHelper::resetHistoryLog()
{
    m_conv.clear();

    delete m_convWriter;

    QFile::remove(m_filename);

    m_convWriter = new Cmn::ConversationWriter(m_filename);
}

