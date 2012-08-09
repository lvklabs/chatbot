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

#include "stats/csvstatsfile.h"
#include "common/csvdocument.h"

#include <QFile>
#include <QMutex>
#include <QMutexLocker>
#include <QtDebug>

#define STR_DATE_FORMAT   "yyyy-MM-dd"

inline uint qHash(const QDate &d)
{
    if (d.isNull()) {
        return 0;
    } else {
        return d.day() | d.month() << 5 | d.year() << (5 + 4);
    }
}

namespace
{

enum Columns {
    DateCol,
    LexixonSizeCol,
    TotalWordsCol,
    ConnectionTimeCol = 10,
    TotalColums = 50    // Reserving lot of columns for future usage
};

} // namespace

//--------------------------------------------------------------------------------------------------
// CsvStatsFile
//--------------------------------------------------------------------------------------------------

Lvk::Stats::CsvStatsFile::CsvStatsFile()
    : m_mutex(new QMutex())
{
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

void Lvk::Stats::CsvStatsFile::setLexiconSize(unsigned size)
{
    qDebug() << "CsvStatsFile: Setting lexicon size:" << size;

    setStat(LexixonSizeCol, size);
}

//--------------------------------------------------------------------------------------------------

void Lvk::Stats::CsvStatsFile::setTotalWords(unsigned count)
{
    qDebug() << "CsvStatsFile: Setting total words:" << count;

    setStat(TotalWordsCol, count);
}

//--------------------------------------------------------------------------------------------------

void Lvk::Stats::CsvStatsFile::addConnectionTime(unsigned secs)
{
    qDebug() << "CsvStatsFile: Adding connection time:" << secs;

    setStat(ConnectionTimeCol, secs, true);
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
                if (row.size() >= TotalColums - 1) { // FIXME -1 bug in CsvRow
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
        DailyStats::const_iterator it;
        for (it = m_dailyStats.constBegin(); it != m_dailyStats.constEnd(); ++it) {
            const Cmn::CsvRow &row = *it;
            if (row.size() >= TotalColums - 1) { // FIXME -1 bug in CsvRow
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
    // If invalid column number
    if (col >= TotalColums) {
        return;
    }

    QMutexLocker locker(m_mutex);

    QDate curDate = QDate::currentDate();
    DailyStats::iterator it = m_dailyStats.find(curDate);
    unsigned oldValue = 0;

    if (it != m_dailyStats.end()) {
        oldValue = (*it)[col].toUInt();
    } else {
        it = m_dailyStats.insert(curDate, Cmn::CsvRow(TotalColums));
        (*it)[DateCol] = curDate.toString(STR_DATE_FORMAT);
    }

    (*it)[col] = QString::number(cumulative ? oldValue + value : value);
}


