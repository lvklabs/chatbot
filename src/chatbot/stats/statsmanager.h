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

#include "stats/score.h"
#include "stats/metric.h"
#include "stats/history.h"
#include "stats/historystatshelper.h"
#include "stats/rulestatshelper.h"
#include "common/conversation.h"

#include <QObject>
#include <QString>
#include <QTimer>

class StatsManagerUnitTest;
class QMutex;
class QDateTime;

namespace Lvk
{

namespace BE
{
    class Rule;
}

/// \addtogroup Lvk
/// @{

namespace Stats
{

class StatsFile;

/// \ingroup Lvk
/// \addtogroup Stats
/// @{

/**
 * \brief The StatsManager class provides a singleton class used to get metrics and scores
 *        for the current chatbot
 *
 * The StatsManager class provides the current and best scores for the current chatbot as well
 * as the score timer.
 */
class StatsManager : public QObject
{
    Q_OBJECT

    friend class ::StatsManagerUnitTest;

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
     * \copydoc StatsFile::metric()
     */
    void metric(Stats::Metric m, QVariant &value);

    /**
     * This is an overloaded member function defined for convenience
     *
     * \see stat(Metric, QVariant &)
     */
    inline QVariant metric(Stats::Metric m)
    {
        QVariant value;
        metric(m, value);
        return value;
    }

    /**
     * Clears stats for the current chatbot ID
     */
    void clear();

    /**
     * Returns the remaining time for the current score interval
     */
    int scoreRemainingTime() const;

    /**
     * Returns the current score for the current chatbot.
     */
    Score currentScore();

    /**
     * Returns the best score for the current chatbot.
     */
    Score bestScore();

public slots:

    /**
     * Starts the score timer.
     */
    void startTicking();

    /**
     * Stops the score timer.
     */
    void stopTicking();

    /**
     * Updates the current score with a new conversation \a entry
     */
    void updateScoreWith(const Cmn::Conversation::Entry &entry);

    /**
     * Updates the current score with a new \a root rule
     */
    void updateScoreWith(const BE::Rule *root);

signals:

    /**
     * When startTicking() is invoked this signal is emitted every second with the remaining
     * seconds to the next score interval until stopTicking() is called.
     */
    void scoreRemainingTime(int secs);

private:
    StatsManager();
    StatsManager(StatsManager&);
    StatsManager& operator=(StatsManager&);

    QString m_chatbotId;
    static StatsManager *m_manager;
    static QMutex *m_mgrMutex;

    QMutex *m_scoreMutex;
    RuleStatsHelper m_ruleStats;
    HistoryStatsHelper m_histStats;

    StatsFile *m_statsFile;
    QTimer m_scoreTimer;
    int m_elapsedTime;

    void updateBestScore();

private slots:
    void onScoreTick();
};

/// @}

} // namespace Stats

/// @}

} // namespace Lvk


#endif // LVK_STATS_STATSMANAGER_H

