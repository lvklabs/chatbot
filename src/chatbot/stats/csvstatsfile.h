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
 * \brief CsvStatsFile class 
 */

class CsvStatsFile : public StatsFile
{

public:

    CsvStatsFile();

    ~CsvStatsFile();

    CsvStatsFile(const QString &filename);

    virtual void setLexiconSize(unsigned size);

    virtual void setTotalWords(unsigned count);

    virtual void addConnectionTime(unsigned secs);

    virtual void load(const QString &filename);

    virtual void save();

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

