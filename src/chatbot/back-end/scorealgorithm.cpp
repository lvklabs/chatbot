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

} // namespace


//--------------------------------------------------------------------------------------------------
// ScoreAlgorithm
//--------------------------------------------------------------------------------------------------

Lvk::BE::ScoreAlgorithm::ScoreAlgorithm()
{
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::Score Lvk::BE::ScoreAlgorithm::score()
{
    const unsigned INTERVAL_CONVERSATION_POINTS = 1000;

    unsigned trp  = uIntStat(Stats::TotalRulePoints);
    unsigned hic  = uIntStat(Stats::HistoryScoreContacts);
    unsigned hcls = uIntStat(Stats::HistoryChatbotLexiconSizeInterval);
    unsigned hcl  = uIntStat(Stats::HistoryChatbotLinesInterval);

    BE::Score score;
    score.conversations = hcls + hcl;
    score.contacts      = hic*INTERVAL_CONVERSATION_POINTS;
    score.rules         = trp;
    score.total         = score.conversations + score.contacts + score.rules;

    return score;
}

