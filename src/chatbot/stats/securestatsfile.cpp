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
#include "stats/securestatsfile.h"
#include "common/cipher.h"

#include <QFile>
#include <QMutex>
#include <QMutexLocker>
#include <QtDebug>
#include <QDataStream>

#include <cassert>

#define STAT_MAGIC_NUMBER            (('s'<<0) | ('t'<<8) | ('a'<<16) | ('t'<<24))
#define STAT_FILE_FORMAT_VERSION     1

#ifndef STATS_CRYPTO_KEY
# define STATS_CRYPTO_KEY { }
#endif

static const char s_statsCryptoKey[] = STATS_CRYPTO_KEY;

enum
{
    TimeIntervalCol,
    TotalColumns = 50    // Reserving lot of columns for future usage
};

//--------------------------------------------------------------------------------------------------
// Helpers
//--------------------------------------------------------------------------------------------------

namespace
{

inline bool validId(Lvk::Stats::Id id)
{
    return id != Lvk::Stats::NullStat && static_cast<int>(id) < TotalColumns;
}

} // namespace


//--------------------------------------------------------------------------------------------------
// SecureStatsFile
//--------------------------------------------------------------------------------------------------

Lvk::Stats::SecureStatsFile::SecureStatsFile()
    : m_mutex(new QMutex(QMutex::Recursive)), m_colNames(TotalColumns), m_curInterv(1)
{
    m_colNames[TimeIntervalCol]           = "Time Interval";
    m_colNames[LexiconSize]               = "Lexixon Size";
    m_colNames[TotalWords]                = "Total Words";
    m_colNames[TotalRules]                = "Total Rules";
    m_colNames[TotalRulePoints]           = "Total Rule Points";
    m_colNames[ConnectionTime]            = "Connection Time (secs)";
    m_colNames[HistoryTotalLines]         = "History Total Lines";
    m_colNames[HistoryChatbotLines]       = "History Chatbot Lines";
    m_colNames[HistoryChatbotDiffLines]   = "History Chabot Different Lines";
    m_colNames[HistoryLexiconSize]        = "History Lexicon Size";
    m_colNames[HistoryChatbotLexiconSize] = "History Chatbot Lexicon Size";
    m_colNames[HistoryImportantContacts]  = "History Important Contacts";
    m_colNames[HistoryContacts]           = "History Contacts";
    m_colNames[RosterSize]                = "Roster Size";
    m_colNames[EnabledRosterSize]         = "Enabled Roster";
}

//--------------------------------------------------------------------------------------------------

Lvk::Stats::SecureStatsFile::~SecureStatsFile()
{
    delete m_mutex;
}

//--------------------------------------------------------------------------------------------------

Lvk::Stats::SecureStatsFile::SecureStatsFile(const QString &filename)
{
    load(filename);
}

//--------------------------------------------------------------------------------------------------

void Lvk::Stats::SecureStatsFile::newInterval()
{
    ++m_curInterv;
}

//--------------------------------------------------------------------------------------------------

void Lvk::Stats::SecureStatsFile::setStat(Stats::Id id, const QVariant &value)
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

void Lvk::Stats::SecureStatsFile::stat(Stats::Id id, QVariant &value)
{
    value.clear();

    if (validId(id)) {
        QMutexLocker locker(m_mutex);

        IntervalStats::const_iterator it = m_stats.find(m_curInterv);
        if (it != m_stats.end()) {
            value = (*it)[id];
        }
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::Stats::SecureStatsFile::history(Stats::Id id, Stats::History &h)
{
    h.clear();

    if (validId(id)) {
        QMutexLocker locker(m_mutex);

        IntervalStats::const_iterator it;
        for (it = m_stats.begin(); it != m_stats.end(); ++it) {
            QVariant value = (*it)[id];
            h.append(it.key(), value);
        }
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::Stats::SecureStatsFile::combinedHistory(Stats::Id id1, Stats::Id id2, Stats::History &h)
{
    h.clear();

    if (validId(id1) && validId(id2)) {
        QMutexLocker locker(m_mutex);

        IntervalStats::const_iterator it;
        for (it = m_stats.begin(); it != m_stats.end(); ++it) {
            QVariant value = (*it)[id1] + (*it)[id2];
            h.append(it.key(), value);
        }
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::Stats::SecureStatsFile::load(const QString &filename)
{
    if (filename.isEmpty()) {
        return;
    }

    qDebug() << "SecureStatsFile: Loading stats from" << filename;

    QMutexLocker locker(m_mutex);

    m_stats.clear();
    m_curInterv = 1;
    m_filename = filename;

    QFile file(filename);

    if (file.open(QFile::ReadOnly)) {
        QByteArray data = file.readAll();

        Cmn::Cipher().decrypt(data, QByteArray(s_statsCryptoKey, sizeof(s_statsCryptoKey)));

        if (!deserialize(data)) {
            close();
        }
    } else if (file.exists()){
        qCritical() << "SecureStatsFile: Could not open" << filename;
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::Stats::SecureStatsFile::save()
{
    QMutexLocker locker(m_mutex);

    if (m_filename.isEmpty()) {
        return;
    }

    qDebug() << "SecureStatsFile: Saving stats to" << m_filename;

    QFile file(m_filename);

    if (file.open(QFile::WriteOnly)) {
        QByteArray data;

        serialize(data);

        Cmn::Cipher().encrypt(data, QByteArray(s_statsCryptoKey, sizeof(s_statsCryptoKey)));

        file.write(data);
        file.flush();
    } else {
        qCritical() << "SecureStatsFile: Could not open" << m_filename;
    }
}


//--------------------------------------------------------------------------------------------------

void Lvk::Stats::SecureStatsFile::close()
{
    QMutexLocker locker(m_mutex);

    m_filename.clear();
    m_stats.clear();
    m_curInterv = 1;
}

//--------------------------------------------------------------------------------------------------

void Lvk::Stats::SecureStatsFile::clear()
{
    QMutexLocker locker(m_mutex);

    m_stats.clear();
    m_curInterv = 1;

    if (m_filename.size() > 0) {
        QFile::remove(m_filename);
    }
}

//--------------------------------------------------------------------------------------------------

bool Lvk::Stats::SecureStatsFile::isEmpty()
{
    QMutexLocker locker(m_mutex);

    return m_stats.isEmpty();
}

//--------------------------------------------------------------------------------------------------

inline void Lvk::Stats::SecureStatsFile::serialize(QByteArray &data)
{
    QDataStream ostream(&data, QIODevice::WriteOnly);
    ostream.setVersion(QDataStream::Qt_4_7);
    ostream << (quint32)STAT_MAGIC_NUMBER;
    ostream << (quint32)STAT_FILE_FORMAT_VERSION;
    ostream << (quint32)m_curInterv;
    ostream << m_stats;
}

//--------------------------------------------------------------------------------------------------

inline bool Lvk::Stats::SecureStatsFile::deserialize(const QByteArray &data)
{
    QDataStream istream(data);
    quint32 magicNumber;
    quint32 version;

    istream >> magicNumber;

    if (magicNumber != STAT_MAGIC_NUMBER) {
        qCritical("SecureStatsFile: Cannot read stats: Invalid magic number");
        return false;
    }

    istream >> version;

    if (version > STAT_FILE_FORMAT_VERSION) {
        qCritical("Cannot read rules: Invalid format version");
        return false;
    }

    istream >> m_curInterv;
    istream >> m_stats;

    if (istream.status() != QDataStream::Ok) {
        qCritical("SecureStatsFile: Cannot read stat file: Invalid file format");
        return false;
    }

    return true;
}

//--------------------------------------------------------------------------------------------------

inline void Lvk::Stats::SecureStatsFile::setStat(int col, unsigned value, bool cumulative)
{
    assert(col < TotalColumns);
    assert(col != TimeIntervalCol);

    qDebug() << "SecureStatsFile: Setting" << m_colNames[col] << ":" << value;

    QMutexLocker locker(m_mutex);

    IntervalStats::iterator it = m_stats.find(m_curInterv);
    unsigned oldValue = 0;

    if (it != m_stats.end()) {
        oldValue = (*it)[col];
    } else {
        it = m_stats.insert(m_curInterv, QVector<unsigned>(TotalColumns, 0));
        (*it)[TimeIntervalCol] = m_curInterv;
    }

    (*it)[col] = cumulative ? oldValue + value : value;
}




