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
#include "nlp-engine/defaultsanitizer.h"       // TODO use factories
#include "nlp-engine/simpleaimlengine.h"       // TODO use factories

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
 * Given a string or a list of strings. the StatsHelper class counts total words, different words
 * (i.e. lexicon size) and lines. Words are sanitized.
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
        m_sanitizer.setLogEnabled(false);
    }

    /**
     * Constructs a StatsHelper and provides statistics for the given string \a s
     */
    StatsHelper(const QString &s)
        : m_words(0), m_lines(0)
    {
        m_sanitizer.setLogEnabled(false);

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
        QStringList words = splitSentence(s);

        m_lines += 1;
        m_words += words.size();
        updateLexicon(words, m_lexicon);
    }

    /**
     * Splits sentence \a s and returns the list of words
     */
    QStringList splitSentence(const QString &s)
    {
        return s.split(QRegExp("\\s+"));
    }

    /**
     * Updates \a lexicon with the given list of words \a words. Each word is sanitized before
     * inserting.
     */
    void updateLexicon(const QStringList &words, QSet<QString> &lexicon)
    {
        foreach (const QString &w, words) {
            QString szw = m_sanitizer.sanitize(w).toLower();
            if (!szw.isEmpty()) {
                lexicon.insert(szw);
            }
        }
    }

private:
    QSet<QString> m_lexicon;
    unsigned m_words;
    unsigned m_lines;
    Lvk::Nlp::DefaultSanitizer m_sanitizer;
};



/**
 * \brief The RuleStatsHelper class provides rule statistics such as total words,
 *        total rules, lexicon size and rule points.
 */
class RuleStatsHelper : public StatsHelper
{
public:

    /**
     * Constructs a RuleStatsHelper and provides statistics for the given \a root rule.
     */
    RuleStatsHelper(const Lvk::BE::Rule *root)
        : m_rules(0), m_points(0)
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
     * For each rule we give the following points:
     * <ul>
     *   <li>1 point simple rules</li>
     *   <li>2 points rules with operators</li>
     *   <li>3 points rules with variables</li>
     *   <li>4 points rules with conditionals</li>
     * </ul>
     */
    unsigned totalRulePoints()
    {
        return m_points;
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
                m_points += points(rule);
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

    /**
     * Returns the points of the given \a rule
     */
    unsigned points(const Lvk::BE::Rule *rule)
    {
        unsigned p = 0;

        // To calculate points Only using first non-empty output

        QString output;
        foreach (const QString &o, rule->output()) {
            if (o.trimmed().size() > 0) {
                output = o;
                break;
            }
        }
        foreach (const QString &input, rule->input()) {
            p += points(input, output);
        }

        return p;
    }

    /**
     * Returns the points of the pair \a (input, output).
     * <ul>
     *   <li>1 point simple rules</li>
     *   <li>2 points rules with operators</li>
     *   <li>3 points rules with variables</li>
     *   <li>4 points rules with conditionals</li>
     * </ul>
     */
    unsigned points(const QString &input, const QString &output)
    {
        if (input.isEmpty() || output.isEmpty()) {
            return 0;
        } else if (Nlp::SimpleAimlEngine::hasVariable(input)) {
            if (Nlp::SimpleAimlEngine::hasConditional(output)) {
                return 4;
            } else if (Nlp::SimpleAimlEngine::hasVariable(output)) {
                return 3;
            } else {
                return 1;
            }
        } else if (Nlp::SimpleAimlEngine::hasKeywordOp(input)) {
            return 2;
        } else if (Nlp::SimpleAimlEngine::hasRegexOp(input)) {
            return 2;
        } else {
            return 1;
        }
    }

private:
    unsigned m_rules;
    unsigned m_points;
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

    /**
     * Returns how many different contacts with at least \a minChats chat entries contains
     * the history.
     */
    unsigned contacts(unsigned /*minChats*/)
    {
        return m_contacts.size(); // FIXME
    }

    /**
     * Returns the chatbot lexicon size. i.e. the total amount of different words used by
     * the chatbot.
     */
    unsigned chatbotLexiconSize()
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
            m_contacts.insert(entry.from);
            m_chatbotLines.insert(entry.response);
            updateLexicon(splitSentence(entry.response), m_chatbotLexicon);
            ++m_chatbotLinesTotal;
        }
    }

private:
    QSet<QString> m_contacts;
    QSet<QString> m_chatbotLines;
    QSet<QString> m_chatbotLexicon;
    unsigned m_chatbotLinesTotal;
};


/// @}

} // namespace BE

/// @}

} // namespace Lvk


#endif // LVK_BE_STATSHELPERS_H

