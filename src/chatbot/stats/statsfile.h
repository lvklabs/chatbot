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

#ifndef LVK_STATS_STATSFILE_H
#define LVK_STATS_STATSFILE_H

#include "stats/metric.h"
#include "stats/score.h"
#include "stats/history.h"
#include "common/conversation.h"

#include <QSet>
#include <QString>
#include <QDateTime>

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
 * \brief The StatsFile class provides the interface for all files that store statistics.
 *
 * The StatsFile defines an abstract method for each statistic we are interested in. Method names
 * starting with \a set store absolut values, whereas those starting with \a add store cumulative
 * values.
 *
 * Statistics are stored in time intervals. Time intervals don't represent a fixed amount
 * of time such us 1 hour, 12 hours or 1 day. Instead, every time newInterval()
 * is invoked a new time interval starts and all invocations to setStat() or stat() will belong
 * to that time interval.
 */
class StatsFile
{
public:

    /**
     * Destroys the object.
     */
    virtual ~StatsFile() { }

    /**
     * Starts a new time interval where the statistics will be stored.
     */
    virtual void newInterval() = 0;

    /**
     * Sets \a value as the value for the metric with the given \a m in the current date
     */
    virtual void setMetric(Metric m, const QVariant &value) = 0;

    /**
     * Returns the value of the given metric \a m for current date
     */
    virtual void metric(Metric m, QVariant &value) = 0;

    /**
     * Returns the history of values of the given metric \a m
     */
    virtual void metricHistory(Metric m, History &h) = 0;

    /**
     *
     */
    virtual int scoreElapsedTime() const = 0;

    /**
     *
     */
    virtual void setScoreElapsedTime(int secs) = 0;

    /**
     *
     */
    virtual QDateTime scoreStartTime() const = 0;

    /**
     *
     */
    virtual Score currentScore() const = 0;

    /**
     *
     */
    virtual void setCurrentScore(const Score &s) = 0;

    /**
     *
     */
    virtual Score bestScore() const = 0;

    /**
     *
     */
    virtual void setBestScore(const Score &s) = 0;

    /**
     *
     */
    virtual QSet<QString> contacts() = 0;

    /**
     *
     */
    virtual void addContact(const QString &username) = 0;

    /**
     *
     */
    virtual void chatHistory(Cmn::Conversation &h) = 0;

    /**
     *
     */
    virtual void appendChatEntry(const Cmn::Conversation::Entry &entry) = 0;

    /**
     * Loads statistics from \a filename
     */
    virtual void load(const QString &filename) = 0;

    /**
     * Save statistics to the current file.
     */
    virtual void save() = 0;

    /**
     * Closes the current file.
     */
    virtual void close() = 0;

    /**
     * Clears the statistics and removes any file created.
     */
    virtual void clear() = 0;

    /**
     * Returns true if the file is empty. Otherwise; returns false.
     */
    virtual bool isEmpty() = 0;
};

/// @}

} // namespace Stats

/// @}

} // namespace Lvk


#endif // LVK_STATS_STATSFILE_H

