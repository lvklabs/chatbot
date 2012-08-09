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
#include "stats/csvstatsfile.h"
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
    : m_statsFile(new CsvStatsFile())
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
        m_statsFile->save();
        m_statsFile->close();
        m_chatbotId.clear();
    }

    if (!chatbotId.isEmpty()) {
        m_statsFile->load(getStatsFilename(chatbotId));
        m_chatbotId = chatbotId;
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::Stats::StatsManager::setLexiconSize(unsigned size)
{
    m_statsFile->setLexiconSize(size);
}

//--------------------------------------------------------------------------------------------------

void Lvk::Stats::StatsManager::setTotalWords(unsigned count)
{
    m_statsFile->setTotalWords(count);
}

//--------------------------------------------------------------------------------------------------

void Lvk::Stats::StatsManager::addConnectionTime(unsigned secs)
{
    m_statsFile->addConnectionTime(secs);
}


