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

#ifndef LVK_STATS_STATSMANAGER_H
#define LVK_STATS_STATSMANAGER_H

#include <QString>
#include <memory>

class QMutex;
class QDateTime;

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace Stats
{

class StatsFile;

/// \ingroup Lvk
/// \addtogroup Stats
/// @{

/**
 * \brief StatsManager class 
 */
class StatsManager
{
public:

    static StatsManager *manager();

    void setChatbotId(const QString &id);

    void setLexiconSize(unsigned size);

    void setTotalWords(unsigned count);

    void addConnectionTime(unsigned secs);

private:
    StatsManager();
    StatsManager(StatsManager&);
    StatsManager& operator=(StatsManager&);

    QString m_chatbotId;
    static StatsManager *m_manager;
    static QMutex *m_mgrMutex;

    std::auto_ptr<StatsFile> m_statsFile;
};

/// @}

} // namespace Stats

/// @}

} // namespace Lvk


#endif // LVK_STATS_STATSMANAGER_H

