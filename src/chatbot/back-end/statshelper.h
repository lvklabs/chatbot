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

#ifndef LVK_BE_STATSHELPER_H
#define LVK_BE_STATSHELPER_H

#include "nlp-engine/defaultsanitizer.h" // TODO use factories

#include <QSet>
#include <QStringList>

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
    unsigned lines() const
    {
        return m_lines;
    }

    /**
     * Returns the total amount of words.
     */
    unsigned totalWords() const
    {
        return m_words;
    }

    /**
     * Returns the lexicon size. i.e. the total amount of different words.
     */
    unsigned lexiconSize() const
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
    QStringList splitSentence(const QString &s) const
    {
        return s.split(QRegExp("\\s+"));
    }

    /**
     * Updates \a lexicon with the given list of words \a words. Each word is sanitized before
     * inserting.
     */
    void updateLexicon(const QStringList &words, QSet<QString> &lexicon) const
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


/// @}

} // namespace BE

/// @}

} // namespace Lvk


#endif // LVK_BE_STATSHELPER_H

