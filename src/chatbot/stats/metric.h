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

#ifndef LVK_STATS_METRIC_H
#define LVK_STATS_METRIC_H

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
 * \brief The Metric enum provides an ID for each metric
 */
enum Metric
{
    NullStat,                   ///< The null stat
    RuleLexiconSize,            ///< The rule lexicon size, i.e. the amount of different words used to define rules.
    RuleWordCount,              ///< The total amount of words used to define rules.
    RuleDefCount,               ///< The total amount of rules.
    RulePoints,                 ///< The total amount of rule points.
    RegexRuleCount,             ///< The total amount of rule with regular expresion.
    KeywordRuleCount,           ///< The total amount of rule with keyword operator.
    VarRuleCount,               ///< The total amount of rule with variables.
    CondRuleCount,              ///< The total amount of rule with conditional.
    ConnectionTime = 20,        ///< The total daily connection time.
    RosterSize = 30,            ///< The roster size, i.e. the amounts of chat contacts.
    BlackRosterSize,            ///< The disable roster size, i.e. the amounts of chat contacts not allowed to chat
//    HistoryTotalLines = 40,   ///< The total amount of lines in the chat history.
//    HistoryLexiconSize,         ///< The history lexicon size, i.e. the total amount of different words used
//    HistoryChatbotLines,        ///< The total amount of lines in the chat history produced by the chatbot.
//    HistoryChatbotDiffLines,    ///< The total amount of different lines in the chat history produced by the chatbot
//    HistoryChatbotLexiconSize,  ///< The chatbot history lexicon size, i.e. the total amount of different words used by the chatbot
//    HistoryChatbotLinesInterval,///< The total amount of lines in the chat history produced by the chatbot within interval
//    HistoryChatbotDiffLinesInterval,///< The total amount of different lines in the chat history produced by the chatbot within interval
//    HistoryChatbotLexiconSizeInterval,///< The chatbot history lexicon size, i.e. the total amount of different words used by the chatbot within interval
//    HistoryContacts,            ///< The amount of different contacts found in the history.
//    HistoryScoreContacts        ///< The amount of different contacts found in the history that scores
};

/// @}

} // namespace Stats

/// @}

} // namespace Lvk


#endif // LVK_STATS_METRIC_H

