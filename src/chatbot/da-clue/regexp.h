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

#ifndef LVK_CLUE_REGEXP_H
#define LVK_CLUE_REGEXP_H

#include "nlp-engine/engine.h"

#include <QString>

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace Clue
{

/// \ingroup Lvk
/// \addtogroup Clue
/// @{

/**
 * \brief The RegExp class provides pattern matching using the NLP engine regular expressions
 */
class RegExp
{
public:

    /**
     * Creates a RegExp object
     */
    RegExp();

    /**
     * Destroys the object
     */
    ~RegExp();

    /**
     * Returns true if \a str is matched exactly by \a pattern regular expression;
     * otherwise returns false.
     */
    bool exactMatch(const QString &pattern, const QString &str);

private:
    RegExp(const RegExp&);
    RegExp & operator=(const RegExp&);

    Nlp::Engine *m_engine;
};

/// @}

} // namespace Clue

/// @}

} // namespace Lvk


#endif // LVK_CLUE_REGEXP_H

