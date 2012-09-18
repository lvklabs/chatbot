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

#ifndef LVK_STATS_SECURESTATSFILE_H
#define LVK_STATS_SECURESTATSFILE_H

#include "stats/metric.h"
#include "stats/statsfile.h"
#include "stats/timeinterval.h"

#include <QString>
#include <QHash>
#include <QVector>

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
 * \brief The SecureStatsFile class provides an implementation of the StatsFile interface that
 *        loads/saves statistics from/to encrypted files.
 */
class SecureStatsFile : public StatsFile
{

public:

    /**
     * Constructs an empty SecureStatsFile object.
     */
    SecureStatsFile();

    /**
     * Constructs a SecureStatsFile object and loads statistics from \a filename.
     */
    SecureStatsFile(const QString &filename);

    /**
     * Destroys the object.
     */
    ~SecureStatsFile();

    /**
     * \copydoc StatsFile::newInterval()
     */
    virtual void newInterval();

    /**
     * \copydoc StatsFile::setMetric()
     */
    virtual void setMetric(Stats::Metric m, const QVariant &value);

    /**
     * \copydoc StatsFile::metric()
     */
    virtual void metric(Stats::Metric m, QVariant &value);

    /**
     * \copydoc StatsFile::metricHistory()
     */
    virtual void metricHistory(Stats::Metric m, Stats::History &h);

    /**
     * \copydoc StatsFile::scoreElapsedTime()
     */
    virtual int scoreElapsedTime() const;

    /**
     * \copydoc StatsFile::setScoreElapsedTime()
     */
    virtual void setScoreElapsedTime(int secs);

    /**
     * \copydoc StatsFile::scoreStartTime()
     */
    virtual QDateTime scoreStartTime() const;

    /**
     * \copydoc StatsFile::currentScore()
     */
    virtual Score currentScore() const;

    /**
     * \copydoc StatsFile::setCurrentScore()
     */
    virtual void setCurrentScore(const Score &s);

    /**
     * \copydoc StatsFile::bestScore()
     */
    virtual Score bestScore() const;

    /**
     * \copydoc StatsFile::setBestScore()
     */
    virtual void setBestScore(const Score &s);

    /**
     * \copydoc StatsFile::contacts()
     */
    virtual QSet<QString> contacts() const;

    /**
     * \copydoc StatsFile::addContact()
     */
    virtual void addContact(const QString &username);

    /**
     * \copydoc StatsFile::chatHistory()
     */
    virtual void chatHistory(Cmn::Conversation &h) const;

    /**
     * \copydoc StatsFile::appendChatEntry()
     */
    virtual void appendChatEntry(const Cmn::Conversation::Entry &entry);

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

    /**
     * \copydoc StatsFile::clear()
     */
    virtual void clear();

    /**
     * \copydoc StatsFile::isEmpty()
     */
    virtual bool isEmpty() const;

private:
    SecureStatsFile(SecureStatsFile&);
    SecureStatsFile& operator=(const SecureStatsFile&);

    typedef QHash<TimeInterval, QVector<unsigned> > IntervalStats;

    QMutex *m_mutex;
    QString m_filename;
    IntervalStats m_stats;
    TimeInterval m_curInterv;
    Score m_bestScore;
    Score m_curScore;
    QDateTime m_scoreStart;
    int m_elapsedTime;
    QSet<QString> m_contacts;
    Cmn::Conversation m_history;

    inline void serialize(QByteArray &data);
    inline bool deserialize(const QByteArray &data);
    inline void setMetric(int col, unsigned value, bool cumulative = false);
};

/// @}

} // namespace Stats

/// @}

} // namespace Lvk


#endif // LVK_STATS_SECURESTATSFILE_H

