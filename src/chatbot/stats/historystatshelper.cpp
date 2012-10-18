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

    QString user = !interfered ? entry.from : entry.to;

    if (interfered) {
        qDebug() << "trackConversation: Interference with user" << user;
    }

    ConversationInfo &info = m_convTracker[user];

    // If inactivity period surpassed. i.e. new conversation
    if (entry.dateTime.toTime_t() - info.last.toTime_t() >= MAX_INACTIVITY) {
        info.entries = interfered ? 0 : 1;
        info.interfered = interfered;

        m_deadConvDiffLinesCount += m_convDiffLines[user].size();
        m_convDiffLines[user].clear();
    } else {
        if (interfered) {
            info.entries = 0;
        } else if (!m_convDiffLines[user].contains(entry.response)) {
            info.entries = info.entries + 1;
        }
        info.interfered |= interfered;
    }

    info.last = entry.dateTime;

    if (!info.interfered) {
        if (info.entries >= MIN_CONV_LEN && !m_scoreContacts.contains(user)) {
            qDebug() << "trackConversation: SCORE!!! with user" << user;
            m_scoreContacts.insert(user);
        }

        m_cbDiffLines.insert(entry.response);
        m_convDiffLines[user].insert(entry.response);
        updateLexicon(splitSentence(entry.response), m_cbLexicon);
        ++m_cbLinesCount;
    }
}

//--------------------------------------------------------------------------------------------------

unsigned Lvk::Stats::HistoryStatsHelper::liveConvDiffLinesCount() const
{
    unsigned count = 0;

    ConversationDiffLines::const_iterator it;
    for (it = m_convDiffLines.begin(); it != m_convDiffLines.end(); ++it) {
        count += it.value().size();
    }

    return count;
}

