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

#ifndef LVK_NLP_DEFAULTSANITIZER_H
#define LVK_NLP_DEFAULTSANITIZER_H

#include "nlp-engine/sanitizer.h"

#include <QSet>
#include <QHash>

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace Nlp
{

/// \ingroup Lvk
/// \addtogroup Nlp
/// @{

/**
 * \brief Default implementation of the Sanitizer interface.
 *
 * The DefaultSanitizer class returns a string with all vowels without acute or diaeresis
 * and remove punctuation symbols such as ,;.?!
 */

class DefaultSanitizer : public Sanitizer
{
public:

    /**
     * Sanitization options
     */
    enum Option {

        /**
         * Remove diacritics. Currently, only removes vowels with acute accent or diaeresis.
         */
        RemoveDiacritic = 0x01,

        /**
         * Remove punctuation such as \b .,;, . It also removes exclamation and interrogation marks.
         */
        RemovePunctuation = 0x02,

        /**
         * Removes duplicated characters. Currently, this option is only well defined for Spanish.
         */
        RemoveDupChars = 0x04,

        /**
         * Removes all braces (curly braces, square braces and parentheses)
         */
        RemoveBraces = 0x08
    };

    /**
     * Constructs a DefaultSanitizer with all option flags enabled.
     *
     * \see Option
     */
    DefaultSanitizer();

    /**
     * Constructs a DefaultSanitizer with the options flags given.
     *
     * \see Option
     */
    DefaultSanitizer(unsigned options);

    /**
     * Sanitizes the string \a str
     */
    virtual QString sanitize(const QString &str) const;

    /**
     * Enables or disables logging. The log is enabled by default.
     */
    void setLogEnabled(bool enabled);

private:
    unsigned m_options;
    QSet<QChar> m_rSet;
    QSet<QChar> m_punctSet;
    QSet<QChar> m_bracesSet;
    QHash<QChar,QChar> m_diacMap;
    bool m_logEnabled;

    void initSets();
};

/// @}

} // namespace Nlp

/// @}

} // namespace Lvk


#endif // LVK_NLP_DEFAULTSANITIZER_H
