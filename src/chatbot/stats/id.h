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

#ifndef LVK_STATS_ID_H
#define LVK_STATS_ID_H

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
 * \brief The Id enum provides an ID for each statistic
 */
enum Id
{
    NullStat,                   ///< The null stat
    LexiconSize,                ///< The lexicon size, i.e. the amount of different words used to define rules.
    TotalWords,                 ///< The total amount of words used to define rules.
    TotalRules,                 ///< The total amount of rules.
    TotalRulePoints,            ///< The total amount of rule points.
    ConnectionTime = 10,        ///< The total daily connection time.
    RosterSize,                 ///< The roster size, i.e. the amounts of chat contacts.
    EnabledRosterSize,          ///< The enabled roster size, i.e. the amounts of chat contacts allowed to chat
    HistoryTotalLines,          ///< The total amount of lines in the chat history.
    HistoryChatbotLines,        ///< The total amount of lines in the chat history produced by the chatbot.
    HistoryChatbotDiffLines,    ///< The total amount of different lines in the chat history produced by the chatbot
    HistoryLexiconSize,         ///< The history lexicon size, i.e. the total amount of different words used
    HistoryChatbotLexiconSize,  ///< The chatbot history lexicon size, i.e. the total amount of different words used by the chatbot
    HistoryContacts,            ///< The amount of different contacts found in the history.
    HistoryImportantContacts    ///< The amount of different contacts found in the history with long converations
};

/// @}

} // namespace Stats

/// @}

} // namespace Lvk


#endif // LVK_STATS_ID_H

