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

#include "stats/historystatshelper.h"
#include "common/globalstrings.h"

//--------------------------------------------------------------------------------------------------
// HistoryStatsHelper
//--------------------------------------------------------------------------------------------------

static const unsigned MAX_INACTIVITY = 60*15; // Max period of inactivity allowed. In seconds.
static const unsigned MIN_CONV_LEN = 20;      // Minimum conversation entries to add a contact

// Tracks chatbot conversations. If the conversation has at least MIN_CONV_LEN entries and
// it was not interfered by the user, adds username to the score contacts set
void Lvk::Stats::HistoryStatsHelper::trackConversation(const Lvk::Cmn::Conversation::Entry &entry)
{
    // If the conversation has been interfered by user
    bool interfered = entry.from.startsWith(OWN_MESSAGE_TOKEN);

    if (!interfered) {
        // Update conversation tracker
        ConversationTracker::iterator it = m_convTracker.find(entry.from);

        if (it != m_convTracker.end()) {
            DateCountPair &p =  it.value();

            // If inactivity period not surpassed
            if (entry.dateTime.toTime_t() - p.first.toTime_t() < MAX_INACTIVITY) {
                p = DateCountPair(entry.dateTime, p.second + 1);

                if (p.second >= MIN_CONV_LEN) {
                    m_scoreContacts.insert(entry.from);
                }
            } else {
                p = DateCountPair(entry.dateTime, 1);
            }
        } else {
            m_convTracker[entry.from] = DateCountPair(entry.dateTime, 1);
        }

        // Finally update some chatbot stats
        m_cbLines.insert(entry.response);
        updateLexicon(splitSentence(entry.response), m_cbLexicon);
        ++m_cbLinesCount;
    } else {
        // Reset conversation
        m_convTracker.remove(entry.to);
    }
}

