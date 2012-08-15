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

#ifndef LVK_BE_STATSHELPERS_H
#define LVK_BE_STATSHELPERS_H

#include "back-end/rule.h"
#include "back-end/conversation.h"
#include "nlp-engine/defaultsanitizer.h"

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
 * \brief The StatsHelper class provides a base class to implement helper classes to get
 *        statistics.
 *
 *
 */
class StatsHelper
{
public:

    /**
     * Constructs an empty StatsHelper
     */
    StatsHelper()
        : m_words(0), m_lines(0)
    {
    }

    /**
     * Constructs a StatsHelper and provides statistics for the given string \a s
     */
    StatsHelper(const QString &s)
        : m_words(0), m_lines(0)
    {
        count(s);
    }

    /**
     * Constructs a StatsHelper and provides statistics for the given list of strings \a l
     */
    StatsHelper(const QStringList &l)
        : m_words(0), m_lines(0)
    {
        count(l);
    }

    /**
     * Returns the total amount of lines, i.e. the amount of processed strings
     */
    unsigned lines()
    {
        return m_lines;
    }

    /**
     * Returns the total amount of words.
     */
    unsigned totalWords()
    {
        return m_words;
    }

    /**
     * Returns the lexicon size. i.e. the total amount of different words.
     */
    unsigned lexiconSize()
    {
        return m_lexicon.size();
    }

protected:

    /**
     * Counts all words in the given list of strings \a l and gets statistics
     */
    void count(const QStringList &l)
    {
        foreach (const QString &s, l) {
            count(s);
        }
    }

    /**
     * Counts all words in the given string \a s and gets statistics
     */
    void count(const QString &s)
    {
        QStringList words = s.split(QRegExp("\\s+"));

        m_lines += 1;
        m_words += words.size();
        updateLexicon(words);
    }

private:
    QSet<QString> m_lexicon;
    unsigned m_words;
    unsigned m_lines;
    Lvk::Nlp::DefaultSanitizer m_sanitizer;

    void updateLexicon(const QStringList &words)
    {
        foreach (const QString &w, words) {
            QString szw = m_sanitizer.sanitize(w).toLower();
            if (!szw.isEmpty()) {
                m_lexicon.insert(szw);
            }
        }
    }
};



/**
 * \brief The RuleStatsHelper class provides rule statistics such as total words,
 *        total rules and lexicon size.
 */
class RuleStatsHelper : public StatsHelper
{
public:

    /**
     * Constructs a RuleStatsHelper and provides statistics for the given \a root rule.
     */
    RuleStatsHelper(const Lvk::BE::Rule *root)
        : m_rules(0)
    {
        if (root) {
            rcount(root);
        }
    }

    /**
     * Returns the total amount of rules of type OrdinaryRule or EvasivesRule.
     */
    unsigned totalRules()
    {
        return m_rules;
    }

    /**
     * Returns the total amount of rule points.
     *
     * TODO:  1 point simple rules
     *        5 points rules with operators
     *       10 points rules with conditionals or variables
     */
    unsigned totalRulePoints()
    {
        return m_rules*1;
    }

protected:

    /**
     * Counts all words in the given rule \a rule and gets statistics. If the rule has children
     * it counts recursively.
     */
    void rcount(const Lvk::BE::Rule *rule)
    {
        Lvk::BE::Rule::const_iterator it;
        for (it = rule->begin(); it != rule->end(); ++it) {
            const Lvk::BE::Rule *rule = *it;
            if (rule->isComplete() && rule->type() != Lvk::BE::Rule::ContainerRule) {
                count(rule);
                m_rules++;
            }
        }
    }

    /**
     * Counts all words in the given rule \a rule and gets statistics
     */
    void count(const Lvk::BE::Rule *rule)
    {
       StatsHelper::count(rule->input());
       StatsHelper::count(rule->output());
    }

private:
    unsigned m_rules;
};



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
    unsigned chatbotLines()
    {
        return m_chatbotLinesTotal;
    }

    /**
     * Returns the total amount of different lines in history produced by the chatbot
     */
    unsigned chatbotDiffLines()
    {
        return m_chatbotLines.size();
    }

    /**
     * Returns how many different contacts contains the history.
     */
    unsigned contacts()
    {
        return m_contacts.size();
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
            m_contacts.insert(entry.from);
            m_chatbotLines.insert(entry.response);
            ++m_chatbotLinesTotal;
        }
    }

private:
    QSet<QString> m_contacts;
    QSet<QString> m_chatbotLines;
    unsigned m_chatbotLinesTotal;
};


/// @}

} // namespace BE

/// @}

} // namespace Lvk


#endif // LVK_BE_STATSHELPERS_H

