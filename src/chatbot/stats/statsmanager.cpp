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

#include "stats/statsmanager.h"
#include "stats/securestatsfile.h"
#include "common/settings.h"
#include "common/settingskeys.h"

#include <QMutex>
#include <QMutexLocker>

#define STATS_FILE_PREFIX ""
#define STATS_FILE_EXT    ".stat"

//--------------------------------------------------------------------------------------------------
// StatsManager
//--------------------------------------------------------------------------------------------------

namespace
{

inline QString getStatsFilename(const QString &id)
{
    Lvk::Cmn::Settings settings;
    QString statsPath = settings.value(SETTING_STATS_PATH).toString();

    QString filename = STATS_FILE_PREFIX + id + STATS_FILE_EXT;

    return statsPath + "/" + filename;
}

} // namespace


//--------------------------------------------------------------------------------------------------
// StatsManager
//--------------------------------------------------------------------------------------------------

Lvk::Stats::StatsManager * Lvk::Stats::StatsManager::m_manager = 0;
QMutex *                   Lvk::Stats::StatsManager::m_mgrMutex = new QMutex();

//--------------------------------------------------------------------------------------------------

Lvk::Stats::StatsManager::StatsManager()
    : m_statsFile(new SecureStatsFile())
{
}

//--------------------------------------------------------------------------------------------------

Lvk::Stats::StatsManager * Lvk::Stats::StatsManager::manager()
{
    if (!m_manager) {
        QMutexLocker locker(m_mgrMutex);
        if (!m_manager) {
            static StatsManager manager;
            m_manager = &manager;
        }
    }

    return m_manager;
}

//--------------------------------------------------------------------------------------------------

void Lvk::Stats::StatsManager::setChatbotId(const QString &chatbotId)
{
    if (!m_chatbotId.isEmpty()) {
        if (!m_statsFile->isEmpty()) {
            m_statsFile->save();
        }
        m_statsFile->close();
        m_chatbotId.clear();
    }

    if (!chatbotId.isEmpty()) {
        m_statsFile->load(getStatsFilename(chatbotId));
        m_chatbotId = chatbotId;
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::Stats::StatsManager::setStat(Stats::Id id, const QVariant &value)
{
    m_statsFile->setStat(id, value);
}

//--------------------------------------------------------------------------------------------------

void Lvk::Stats::StatsManager::stat(Stats::Id id, QVariant &value)
{
    return m_statsFile->stat(id, value);
}

//--------------------------------------------------------------------------------------------------

void Lvk::Stats::StatsManager::history(Stats::Id id, Stats::History &h)
{
    m_statsFile->history(id, h);
}

//--------------------------------------------------------------------------------------------------

void Lvk::Stats::StatsManager::combinedHistory(Stats::Id id1, Stats::Id id2, Stats::History &h)
{
    m_statsFile->combinedHistory(id1, id2, h);
}

//--------------------------------------------------------------------------------------------------

void Lvk::Stats::StatsManager::clear()
{
    m_statsFile->clear();
}

//--------------------------------------------------------------------------------------------------

void Lvk::Stats::StatsManager::newInterval()
{
    m_statsFile->newInterval();
}

