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
        RemoveDiacritic = 0x1,

        /**
         * Remove punctuation such as \b .,;, . It also removes exclamation and interrogation marks.
         */
        RemovePunctuation = 0x2,

        /**
         * Removes duplicated characters. Currently, this option is only well defined for Spanish.
         */
        RemoveDupChars = 0x4,

        /**
         * FIXME
         */
        PreSanitizer = RemoveDupChars,

        /**
         * FIXME
         */
        PostSanitizer = RemoveDiacritic | RemovePunctuation

    };

    /**
     * Sanitization option flags
     */
    Q_DECLARE_FLAGS(Options, Option)

    /**
     * Constructs a DefaultSanitizer with all option flags enabled.
     *
     * \see Options
     */
    DefaultSanitizer();

    /**
     * Constructs a DefaultSanitizer with the options flags given.
     *
     * \see Options
     */
    DefaultSanitizer(Options options);

    /**
     * Sanitizes the string \a str
     */
    virtual QString sanitize(const QString &str) const;

private:
    Options m_options;
};

/// @}

} // namespace Nlp

/// @}

} // namespace Lvk


#endif // LVK_NLP_DEFAULTSANITIZER_H
