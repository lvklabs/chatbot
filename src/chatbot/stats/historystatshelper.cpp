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

#include <QtDebug>

//--------------------------------------------------------------------------------------------------
// HistoryStatsHelper
//--------------------------------------------------------------------------------------------------

static const unsigned MAX_INACTIVITY = 60*15; // Max period of inactivity allowed. In seconds.
static const unsigned MIN_CONV_LEN = 20;      // Minimum conversation entries to score a contact

// Tracks chatbot conversations. If the conversation has at least MIN_CONV_LEN entries and
// it was not interfered by the user, adds username to the score contacts set
void Lvk::Stats::HistoryStatsHelper::trackConversation(const Lvk::Cmn::Conversation::Entry &entry)
{
    // If interfered by user
    bool interfered = entry.from.startsWith(OWN_MESSAGE_TOKEN);

    if (interfered) {
        qDebug() << "trackConversation: Interference with user" << entry.to;
    }

    ConversationInfo &info = m_convTracker[!interfered ? entry.from : entry.to];

    // If inactivity period surpassed. i.e. new conversation
    if (entry.dateTime.toTime_t() - info.last.toTime_t() >= MAX_INACTIVITY) {
        info.entries = interfered ? 0 : 1;
        info.interfered = interfered;
    } else {
        info.entries = interfered ? 0 : info.entries + 1;
        info.interfered |= interfered;
    }

    info.last = entry.dateTime;

    if (!info.interfered && info.entries >= MIN_CONV_LEN) {
        qDebug() << "trackConversation: SCORE!!! with user" << entry.from;
        m_scoreContacts.insert(entry.from);
    }

    if (!info.interfered) {
        m_cbLines.insert(entry.response);
        updateLexicon(splitSentence(entry.response), m_cbLexicon);
        ++m_cbLinesCount;
    }
}

