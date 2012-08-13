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

#ifndef LVK_STATS_CSVSTATSFILE_H
#define LVK_STATS_CSVSTATSFILE_H

#include "stats/statsfile.h"
#include "common/csvrow.h"

#include <QString>
#include <QDate>
#include <QHash>

class QMutex;

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace Stats
{

/// \ingroup Lvk
/// \addtogroup Stats
/// @{

/**
 * \brief The CsvStatsFile class provides an implementation of the StatsFile interface that
 *        loads/saves statistics from/to CSV files.
 */
class CsvStatsFile : public StatsFile
{

public:

    /**
     * Constructs an empty CsvStatsFile object.
     */
    CsvStatsFile();

    /**
     * Constructs a CsvStatsFile object and loads statistics from \a filename.
     */
    CsvStatsFile(const QString &filename);

    /**
     * Destroys the object.
     */
    ~CsvStatsFile();

    /**
     * \copydoc StatsFile::setLexiconSize()
     */
    virtual void setLexiconSize(unsigned size);

    /**
     * \copydoc StatsFile::setTotalWords()
     */
    virtual void setTotalWords(unsigned count);

    /**
     * \copydoc StatsFile::setTotalRules()
     */
    virtual void setTotalRules(unsigned count);

    /**
     * \copydoc StatsFile::setTotalRulePoints()
     */
    virtual void setTotalRulePoints(unsigned points);

    /**
     * \copydoc StatsFile::addConnectionTime()
     */
    virtual void addConnectionTime(unsigned secs);

    /**
     * \copydoc StatsFile::setHistoryLines()
     */
    virtual void setHistoryLines(unsigned count);

    /**
     * \copydoc StatsFile::setHistoryLexiconSize()
     */
    virtual void setHistoryLexiconSize(unsigned size);

    /**
     * \copydoc StatsFile::setRosterSize()
     */
    virtual void setRosterSize(unsigned size);

    /**
     * \copydoc StatsFile:: setEnabledRosterSize()
     */
    virtual void setEnabledRosterSize(unsigned size);

    /**
     * \copydoc StatsFile::load()
     */
    virtual void load(const QString &filename);

    /**
     * \copydoc StatsFile::save()
     */
    virtual void save();

    /**
     * \copydoc StatsFile::close()
     */
    virtual void close();

private:
    typedef QHash<QDate, Cmn::CsvRow> DailyStats;

    QMutex *m_mutex;
    QString m_filename;
    DailyStats m_dailyStats;

    inline void setStat(int col, unsigned value, bool cumulative = false);
    inline void setStat(int col, const QString &value, bool cumulative = false);
};

/// @}

} // namespace Stats

/// @}

} // namespace Lvk


#endif // LVK_STATS_CSVSTATSFILE_H

