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

#include "stats/id.h"
#include "stats/csvstatsfile.h"
#include "common/csvdocument.h"

#include <QFile>
#include <QMutex>
#include <QMutexLocker>
#include <QtDebug>

#include <cassert>

#define STR_DATE_FORMAT   "yyyy-MM-dd"

enum
{
    DateCol,
    TotalColumns = 50    // Reserving lot of columns for future usage
};

//--------------------------------------------------------------------------------------------------
// Helpers
//--------------------------------------------------------------------------------------------------

// Required for QHash<QDate>
inline uint qHash(const QDate &d)
{
    if (d.isNull()) {
        return 0;
    } else {
        return d.day() | d.month() << 5 | d.year() << (5 + 4);
    }
}

//--------------------------------------------------------------------------------------------------
// CsvStatsFile
//--------------------------------------------------------------------------------------------------
//
// TODO Improve performace. Do not make so many convertions unsigned <-> QString

Lvk::Stats::CsvStatsFile::CsvStatsFile()
    : m_mutex(new QMutex()), m_colNames(TotalColumns)
{
    m_colNames[DateCol]             = "Date";
    m_colNames[LexiconSize]         = "Lexixon Size";
    m_colNames[TotalWords]          = "Total Words";
    m_colNames[TotalRules]          = "Total Rules";
    m_colNames[TotalRulePoints]     = "Total Rule Points";
    m_colNames[ConnectionTime]      = "Connection Time (secs)";
    m_colNames[HistoryTotalLines]   = "History Total Lines";
    m_colNames[HistoryChatbotLines] = "History Chatbot Lines";
    m_colNames[HistoryChatbotDiffLines] = "History Chabot Different Lines";
    m_colNames[HistoryLexiconSize]  = "History Lexicon Size";
    m_colNames[HistoryContacts]     = "History Contacts";
    m_colNames[RosterSize]          = "Roster Size";
    m_colNames[EnabledRosterSize]   = "Enabled Roster";
}

//--------------------------------------------------------------------------------------------------

Lvk::Stats::CsvStatsFile::~CsvStatsFile()
{
    delete m_mutex;
}

//--------------------------------------------------------------------------------------------------

Lvk::Stats::CsvStatsFile::CsvStatsFile(const QString &filename)
{
    load(filename);
}

//--------------------------------------------------------------------------------------------------

void Lvk::Stats::CsvStatsFile::setStat(Stats::Id id, const QVariant &value)
{
    switch (id) {
    case NullStat:
        return;
    case ConnectionTime:
        return setStat(id, value.toUInt(), true);
    default:
        return setStat(id, value.toUInt(), false);
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::Stats::CsvStatsFile::stat(Stats::Id id, QVariant &value)
{
    if (id != NullStat && static_cast<int>(id) < TotalColumns) {
        QMutexLocker locker(m_mutex);

        DailyStats::const_iterator it = m_dailyStats.find(QDate::currentDate());
        if (it != m_dailyStats.end()) {
            value = (*it)[id].toUInt();
        }
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::Stats::CsvStatsFile::history(Stats::Id id, Stats::History &history)
{
    history.clear();

    if (id != NullStat && static_cast<int>(id) < TotalColumns) {
        QMutexLocker locker(m_mutex);

        DailyStats::const_iterator it;
        for (it = m_dailyStats.begin(); it != m_dailyStats.end(); ++it) {
            QVariant value = (*it)[id].toUInt();
            history.append(it.key(), value);
        }
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::Stats::CsvStatsFile::load(const QString &filename)
{
    if (filename.isEmpty()) {
        return;
    }

    qDebug() << "CsvStatsFile: Loading stats from" << filename;

    QMutexLocker locker(m_mutex);

    m_dailyStats.clear();
    m_filename = filename;

    QFile file(filename);

    if (file.open(QFile::ReadOnly)) {
        const quint64 BUF_SIZE = 10*1024;
        char buf[BUF_SIZE];

        while (!file.atEnd()) {
            if (file.readLine(buf, BUF_SIZE) > 0) {
                Cmn::CsvRow row(QString::fromUtf8(buf));
                if (row.size() >= TotalColumns - 1) { // FIXME -1 bug in CsvRow
                    QDate date = QDate::fromString(row[DateCol], STR_DATE_FORMAT);
                    if (date.isValid()) {
                        m_dailyStats[date] = row;
                    }
                } else {
                    qWarning() << "CsvStatsFile: Invalid row format: " << buf;
                }
            } else {
                qCritical() << "CsvStatsFile: Could not read line from" << filename;
            }
        }
    } else if (file.exists()){
        qCritical() << "CsvStatsFile: Could not open" << filename;
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::Stats::CsvStatsFile::save()
{
    QMutexLocker locker(m_mutex);

    if (m_filename.isEmpty()) {
        return;
    }

    qDebug() << "CsvStatsFile: Saving stats to" << m_filename;

    QFile file(m_filename);

    if (file.open(QFile::WriteOnly)) {
        file.write(m_colNames.toString().toUtf8());
        file.write("\n");

        DailyStats::const_iterator it;
        for (it = m_dailyStats.constBegin(); it != m_dailyStats.constEnd(); ++it) {
            const Cmn::CsvRow &row = *it;
            if (row.size() >= TotalColumns - 1) { // FIXME -1 bug in CsvRow
                file.write(row.toString().toUtf8());
                file.write("\n");
            }
        }
        file.flush();
    } else {
        qCritical() << "CsvStatsFile: Could not open" << m_filename;
    }
}


//--------------------------------------------------------------------------------------------------

void Lvk::Stats::CsvStatsFile::close()
{
    QMutexLocker locker(m_mutex);

    m_filename.clear();
    m_dailyStats.clear();
}

//--------------------------------------------------------------------------------------------------

inline void Lvk::Stats::CsvStatsFile::setStat(int col, unsigned value, bool cumulative /*= false*/)
{
    assert(col < TotalColumns);
    assert(col != DateCol);

    qDebug() << "CsvStatsFile: Setting" << m_colNames[col] << ":" << value;

    QMutexLocker locker(m_mutex);

    QDate curDate = QDate::currentDate();
    DailyStats::iterator it = m_dailyStats.find(curDate);
    unsigned oldValue = 0;

    if (it != m_dailyStats.end()) {
        oldValue = (*it)[col].toUInt();
    } else {
        it = m_dailyStats.insert(curDate, Cmn::CsvRow(TotalColumns));
        (*it)[DateCol] = curDate.toString(STR_DATE_FORMAT);
    }

    (*it)[col] = QString::number(cumulative ? oldValue + value : value);
}

