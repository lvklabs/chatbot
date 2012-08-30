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

#include "back-end/scorealgorithm.h"
#include "stats/statsmanager.h"


//--------------------------------------------------------------------------------------------------
// Helpers
//--------------------------------------------------------------------------------------------------

namespace
{

inline unsigned uIntStat(Lvk::Stats::Id id)
{
    return Lvk::Stats::StatsManager::manager()->stat(id).toUInt();
}

//--------------------------------------------------------------------------------------------------

//inline Lvk::Stats::History history(Lvk::Stats::Id id)
//{
//    return Lvk::Stats::StatsManager::manager()->history(id);
//}

//--------------------------------------------------------------------------------------------------

//inline Lvk::Stats::History statCombinedHistory(Lvk::Stats::Id id1, Lvk::Stats::Id id2)
//{
//    return Lvk::Stats::StatsManager::manager()->combinedHistory(id1, id2);
//}

} // namespace


//--------------------------------------------------------------------------------------------------
// ScoreAlgorithm
//--------------------------------------------------------------------------------------------------

Lvk::BE::ScoreAlgorithm::ScoreAlgorithm()
{
}

Lvk::BE::Score Lvk::BE::ScoreAlgorithm::score()
{
    //    Stats::History h = statCombinedHistory(Stats::HistoryChatbotDiffLines,
    //                                           Stats::HistoryLexiconSize);

    //    unsigned maxDailyValue = 0;

    //    for (Stats::History::iterator it = h.begin(); it != h.end(); ++it) {
    //        unsigned value = it->second.toUInt();
    //        if (value > maxDailyValue) {
    //            maxDailyValue = value;
    //        }
    //    }

    const unsigned IMPORTANT_CONTACT_POINTS = 1000;

    unsigned trp  = uIntStat(Stats::TotalRulePoints);
    unsigned hic  = uIntStat(Stats::HistoryImportantContacts);
    unsigned hcls = uIntStat(Stats::HistoryChatbotLexiconSize);
    unsigned hcl  = uIntStat(Stats::HistoryChatbotLines);

    BE::Score score;
    score.conversations = hcls + hcl;
    score.contacts      = hic*IMPORTANT_CONTACT_POINTS;
    score.rules         = trp;
    score.total         = score.conversations + score.contacts + score.rules;

    return score;
}

