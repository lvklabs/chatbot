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

#ifndef LVK_BE_HISTORYSTATSHELPER_H
#define LVK_BE_HISTORYSTATSHELPER_H

#include "back-end/statshelper.h"
#include "back-end/conversation.h"

#include <QHash>
#include <functional>
#include <algorithm>

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace BE
{

/// \ingroup Lvk
/// \addtogroup BE
/// @{

/**
 * \brief The HistoryStatsHelper class provides chat history statistics such as total words,
 *        total lines and lexicon size.
 */
class HistoryStatsHelper : public StatsHelper
{
public:

    /**
     * Constructs a HistoryStatsHelper and provides statistics for the given conversation
     * \a conv.
     */
    HistoryStatsHelper(const Lvk::BE::Conversation &conv)
        : m_chatbotLinesTotal(0)
    {
        count(conv);
    }

    /**
     * Returns the total amount of lines in history produced by the chatbot
     */
    unsigned chatbotLines() const
    {
        return m_chatbotLinesTotal;
    }

    /**
     * Returns the total amount of different lines in history produced by the chatbot
     */
    unsigned chatbotDiffLines() const
    {
        return m_chatbotLines.size();
    }

    /**
     * Returns how many different contacts contains the history.
     */
    unsigned contacts() const
    {
        return m_contacts.size();
    }

    /**
     * Returns how many different contacts with at least \a minChats chat entries contains
     * the history.
     */
    unsigned contacts(unsigned minChats) const
    {
        return std::count_if(m_contacts.begin(), m_contacts.end(),
                             std::bind2nd(std::greater_equal<unsigned>(), minChats));
    }

    /**
     * Returns the chatbot lexicon size. i.e. the total amount of different words used by
     * the chatbot.
     */
    unsigned chatbotLexiconSize() const
    {
        return m_chatbotLexicon.size();
    }

protected:

    /**
     * Counts all words in the given conversation \a conv and gets statistics
     */
    void count(const Lvk::BE::Conversation &conv)
    {
        QList<Lvk::BE::Conversation::Entry>::const_iterator it;
        for (it = conv.entries().begin(); it != conv.entries().end(); ++it) {
             count(*it);
        }
    }

    /**
     * Counts all words in the given conversation \a entry and gets statistics
     */
    void count(const Lvk::BE::Conversation::Entry &entry)
    {
        StatsHelper::count(entry.msg);
        StatsHelper::count(entry.response);

        if (!entry.response.isEmpty()) {
            countContact(entry.from);
            m_chatbotLines.insert(entry.response);
            updateLexicon(splitSentence(entry.response), m_chatbotLexicon);
            ++m_chatbotLinesTotal;
        }
    }

private:
    typedef QHash<QString, unsigned> ContactsCount;

    ContactsCount m_contacts;
    QSet<QString> m_chatbotLines;
    QSet<QString> m_chatbotLexicon;
    unsigned m_chatbotLinesTotal;

    void countContact(const QString &username)
    {
        ContactsCount::iterator it = m_contacts.find(username);
        if (it != m_contacts.end()) {
            m_contacts[username] = it.value() + 1;
        } else {
            m_contacts[username] = 1;
        }
    }
};


/// @}

} // namespace BE

/// @}

} // namespace Lvk


#endif // LVK_BE_STATSHELPERS_H

