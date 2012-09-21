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

#include "stats/metric.h"
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
#define STATS_CRYPTO_KEY    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
#endif

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

inline bool validMetric(Lvk::Stats::Metric m)
{
    return m != Lvk::Stats::NullStat && static_cast<int>(m) < TotalColumns;
}

} // namespace


//--------------------------------------------------------------------------------------------------
// SecureStatsFile
//--------------------------------------------------------------------------------------------------

Lvk::Stats::SecureStatsFile::SecureStatsFile()
    : m_mutex(new QMutex(QMutex::Recursive)), m_curInterv(1), m_elapsedTime(0)
{
}

//--------------------------------------------------------------------------------------------------

Lvk::Stats::SecureStatsFile::SecureStatsFile(const QString &filename)
    : m_mutex(new QMutex(QMutex::Recursive)), m_curInterv(1), m_elapsedTime(0)
{
    load(filename);
}

//--------------------------------------------------------------------------------------------------

Lvk::Stats::SecureStatsFile::~SecureStatsFile()
{
    delete m_mutex;
}

//--------------------------------------------------------------------------------------------------

void Lvk::Stats::SecureStatsFile::newInterval()
{
    QMutexLocker locker(m_mutex);

    ++m_curInterv;
    m_history.clear();
    m_scoreStart = QDateTime::currentDateTime();
    m_elapsedTime = 0;
}

//--------------------------------------------------------------------------------------------------

void Lvk::Stats::SecureStatsFile::setMetric(Stats::Metric m, const QVariant &value)
{
    switch (m) {
    case NullStat:
        return;
    case ConnectionTime:
        return setMetric(m, value.toUInt(), true);
    default:
        return setMetric(m, value.toUInt(), false);
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::Stats::SecureStatsFile::metric(Stats::Metric m, QVariant &value)
{
    value.clear();

    if (validMetric(m)) {
        QMutexLocker locker(m_mutex);

        IntervalStats::const_iterator it = m_stats.find(m_curInterv);
        if (it != m_stats.end()) {
            value = (*it)[m];
        }
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::Stats::SecureStatsFile::metricHistory(Stats::Metric m, Stats::History &h)
{
    h.clear();

    if (validMetric(m)) {
        QMutexLocker locker(m_mutex);

        IntervalStats::const_iterator it;
        for (it = m_stats.begin(); it != m_stats.end(); ++it) {
            QVariant value = (*it)[m];
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
        const char KEY[] = STATS_CRYPTO_KEY;
        QByteArray key(KEY, sizeof(KEY));
        QByteArray data = file.readAll();

        Cmn::Cipher().decrypt(data, key);

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
        const char KEY[] = STATS_CRYPTO_KEY;
        QByteArray key(KEY, sizeof(KEY));
        QByteArray data;

        serialize(data);

        Cmn::Cipher().encrypt(data, key);

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
    m_bestScore = Score();
    m_curScore = Score();
    m_scoreStart = QDateTime();
    m_elapsedTime = 0;
    m_contacts.clear();
    m_history.clear();
}

//--------------------------------------------------------------------------------------------------

void Lvk::Stats::SecureStatsFile::clear()
{
    QMutexLocker locker(m_mutex);

    m_stats.clear();
    m_curInterv = 1;
    m_bestScore = Score();
    m_curScore = Score();
    m_scoreStart = QDateTime();
    m_elapsedTime = 0;
    m_contacts.clear();
    m_history.clear();

    if (m_filename.size() > 0) {
        QFile::remove(m_filename);
    }
}

//--------------------------------------------------------------------------------------------------

bool Lvk::Stats::SecureStatsFile::isEmpty() const
{
    QMutexLocker locker(m_mutex);

    return m_stats.isEmpty() &&
            m_curInterv == 1 &&
            m_contacts.isEmpty() &&
            m_history.isEmpty() &&
            m_curScore.isNull() &&
            m_bestScore.isNull() &&
            m_scoreStart.isNull() &&
            m_elapsedTime == 0;
}

//--------------------------------------------------------------------------------------------------

inline void Lvk::Stats::SecureStatsFile::serialize(QByteArray &data)
{
    QDataStream ostream(&data, QIODevice::WriteOnly);
    ostream.setVersion(QDataStream::Qt_4_7);
    ostream << (quint32)STAT_MAGIC_NUMBER;
    ostream << (quint32)STAT_FILE_FORMAT_VERSION;
    ostream << m_curInterv;
    ostream << m_stats;
    ostream << m_bestScore;
    ostream << m_curScore;
    ostream << m_scoreStart;
    ostream << m_elapsedTime;
    ostream << m_contacts;
    ostream << m_history;
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
    istream >> m_bestScore;
    istream >> m_curScore;
    istream >> m_scoreStart;
    istream >> m_elapsedTime;
    istream >> m_contacts;
    istream >> m_history;

    if (istream.status() != QDataStream::Ok) {
        qCritical("SecureStatsFile: Cannot read stat file: Invalid file format");
        return false;
    }

    return true;
}

//--------------------------------------------------------------------------------------------------

inline void Lvk::Stats::SecureStatsFile::setMetric(int col, unsigned value, bool cumulative)
{
    assert(col < TotalColumns);
    assert(col != TimeIntervalCol);

    qDebug() << "SecureStatsFile: Setting col #" << col << ":" << value;

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

//--------------------------------------------------------------------------------------------------

int Lvk::Stats::SecureStatsFile::scoreElapsedTime() const
{
    return m_elapsedTime;
}

//--------------------------------------------------------------------------------------------------

void Lvk::Stats::SecureStatsFile::setScoreElapsedTime(int secs)
{
    m_elapsedTime = secs;
}

//--------------------------------------------------------------------------------------------------

QDateTime Lvk::Stats::SecureStatsFile::scoreStartTime() const
{
    QMutexLocker locker(m_mutex);

    return m_scoreStart;
}

//--------------------------------------------------------------------------------------------------

Lvk::Stats::Score Lvk::Stats::SecureStatsFile::currentScore() const
{
    QMutexLocker locker(m_mutex);

    return m_curScore;
}

//--------------------------------------------------------------------------------------------------

void Lvk::Stats::SecureStatsFile::setCurrentScore(const Score &s)
{
    QMutexLocker locker(m_mutex);

    m_curScore = s;
}

//--------------------------------------------------------------------------------------------------

Lvk::Stats::Score Lvk::Stats::SecureStatsFile::bestScore() const
{
    QMutexLocker locker(m_mutex);

    return m_bestScore;
}

//--------------------------------------------------------------------------------------------------

void Lvk::Stats::SecureStatsFile::setBestScore(const Score &s)
{
    QMutexLocker locker(m_mutex);

    m_bestScore = s;
}

//--------------------------------------------------------------------------------------------------

QSet<QString> Lvk::Stats::SecureStatsFile::contacts() const
{
    QMutexLocker locker(m_mutex);

    return m_contacts;
}

//--------------------------------------------------------------------------------------------------

void Lvk::Stats::SecureStatsFile::addContact(const QString &username)
{
    QMutexLocker locker(m_mutex);

    m_contacts.insert(username);
}

//--------------------------------------------------------------------------------------------------

void Lvk::Stats::SecureStatsFile::chatHistory(Cmn::Conversation &h) const
{
    QMutexLocker locker(m_mutex);

    h = m_history;
}

//--------------------------------------------------------------------------------------------------

void Lvk::Stats::SecureStatsFile::appendChatEntry(const Cmn::Conversation::Entry &entry)
{
    QMutexLocker locker(m_mutex);

    m_history.append(entry);
}

