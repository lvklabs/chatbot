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
 * \brief The StatsManager class provides a singleton class used to log all statistics
 *
 * The StatsManager class defines a method for each statistic we are interested in. Method names
 * starting with \a set store absolut values, whereas those starting with \a add store cumulative
 * values.
 *
 * Statistics are stored per chatbot ID on a daily basis. Before starting to log statistics we must
 * set the chatbot ID with setChatbotId().
 */
class StatsManager
{
public:

    /**
     * Returns the singleton instance.
     */
    static StatsManager *manager();

    /**
     * Sets the chatbot ID of the chabot that we are logging statistics.
     */
    void setChatbotId(const QString &id);

    /**
     * \copydoc StatsFile::setLexiconSize()
     */
    void setLexiconSize(unsigned size);

    /**
     * \copydoc StatsFile::setTotalWords()
     */
    void setTotalWords(unsigned count);

    /**
     * \copydoc StatsFile::setTotalRules()
     */
    void setTotalRules(unsigned count);

    /**
     * \copydoc StatsFile::setTotalRulePoints()
     */
    void setTotalRulePoints(unsigned points);

    /**
     * \copydoc StatsFile::addConnectionTime()
     */
    void addConnectionTime(unsigned secs);

    /**
     * \copydoc StatsFile::setHistoryLines()
     */
    void setHistoryLines(unsigned count);

    /**
     * \copydoc StatsFile::setHistoryChabotLines()
     */
    void setHistoryChabotLines(unsigned count);

    /**
     * \copydoc StatsFile::setHistoryChabotDiffLines()
     */
    void setHistoryChabotDiffLines(unsigned count);

    /**
     * \copydoc StatsFile::setHistoryLexiconSize()
     */
    void setHistoryLexiconSize(unsigned size);

    /**
     * \copydoc StatsFile::setHistoryContacts()
     */
    void setHistoryContacts(unsigned count);

    /**
     * \copydoc StatsFile::setRosterSize()
     */
    void setRosterSize(unsigned size);

    /**
     * \copydoc StatsFile::setEnabledRosterSize()
     */
    void setEnabledRosterSize(unsigned size);

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

