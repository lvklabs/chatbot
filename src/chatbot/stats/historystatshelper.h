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

#ifndef LVK_STATS_HISTORYSTATSHELPER_H
#define LVK_STATS_HISTORYSTATSHELPER_H

#include "stats/statshelper.h"
#include "common/conversation.h"

#include <QHash>
#include <functional>
#include <algorithm>

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
 * \brief The HistoryStatsHelper class provides chat history statistics such as total words,
 *        total lines and lexicon size.
 */
class HistoryStatsHelper : public StatsHelper
{
public:

    /**
     * Constructs a emtpy HistoryStatsHelper
     */
    HistoryStatsHelper()
        : m_cbLinesCount(0)
    {
    }

    /**
     * Constructs a HistoryStatsHelper and provides statistics for the given conversation
     * \a conv.
     */
    HistoryStatsHelper(const Lvk::Cmn::Conversation &conv)
        : m_cbLinesCount(0)
    {
        count(conv);
    }

    /**
     * Returns the total amount of lines in history produced by the chatbot
     */
    unsigned chatbotLines() const
    {
        return m_cbLinesCount;
    }

    /**
     * Returns the total amount of different lines in history produced by the chatbot
     */
    unsigned chatbotDiffLines() const
    {
        return m_cbLines.size();
    }

    /**
     * Returns the chatbot lexicon size. i.e. the total amount of different words used by
     * the chatbot.
     */
    unsigned chatbotLexiconSize() const
    {
        return m_cbLexicon.size();
    }

    /**
     * Returns the set of contacts that have a conversation that scores points
     */
    const QSet<QString> &scoreContacts() const
    {
        return m_scoreContacts;
    }

    /**
     * Updates stats with a new conversation \a conv
     */
    void update(const Cmn::Conversation &conv)
    {
        count(conv);
    }

    /**
     * Updates stats with a new \a entry
     */
    void update(const Cmn::Conversation::Entry &entry)
    {
        count(entry);
    }

    /**
     * Sets all stats to zero.
     */
    void clear()
    {
        StatsHelper::clear();
        m_convTracker.clear();
        m_cbLines.clear();
        m_cbLexicon.clear();
        m_cbLinesCount = 0;
    }

protected:

    /**
     * Counts all words in the given conversation \a conv and gets statistics
     */
    void count(const Lvk::Cmn::Conversation &conv)
    {
        QList<Lvk::Cmn::Conversation::Entry>::const_iterator it;
        for (it = conv.entries().begin(); it != conv.entries().end(); ++it) {
             count(*it);
        }
    }

    /**
     * Counts all words in the given conversation \a entry and gets statistics
     */
    void count(const Lvk::Cmn::Conversation::Entry &entry)
    {
        StatsHelper::count(entry.msg);
        StatsHelper::count(entry.response);

        if (!entry.response.isEmpty()) {
            m_cbLines.insert(entry.response);
            updateLexicon(splitSentence(entry.response), m_cbLexicon);
            ++m_cbLinesCount;
            trackConversation(entry);
        }
    }

private:
    // (last entry timestamp, #entries)
    typedef QPair<QDateTime, unsigned> DateCountPair;
    // username -> (last entry timestamp, #entries)
    typedef QHash<QString, DateCountPair> ConversationTracker;

    ConversationTracker m_convTracker;
    QSet<QString> m_scoreContacts;
    // chatbot stats
    QSet<QString> m_cbLines;
    QSet<QString> m_cbLexicon;
    unsigned m_cbLinesCount;

    // Tracks conversations and if the conversation has at least MIN_CONV_LEN entries,
    // adds username to the score contacts set
    void trackConversation(const Lvk::Cmn::Conversation::Entry &entry)
    {
        const unsigned MAX_INACTIVITY = 60*30; // Max period of inactivity allowed. In seconds.
        const unsigned MIN_CONV_LEN = 20;      // Minimum conversation lenght to add contact

        ConversationTracker::iterator it = m_convTracker.find(entry.from);

        if (it != m_convTracker.end()) {
            DateCountPair &p =  it.value();
            if (entry.dateTime.toTime_t() - p.first.toTime_t() < MAX_INACTIVITY
                    /* TODO && !interfered*/) {
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
    }
};


/// @}

} // namespace Stats

/// @}

} // namespace Lvk


#endif // LVK_STATS_STATSHELPERS_H

