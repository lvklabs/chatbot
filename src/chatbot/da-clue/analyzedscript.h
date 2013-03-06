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

#ifndef LVK_CLUE_ANALYZEDSCRIPT_H
#define LVK_CLUE_ANALYZEDSCRIPT_H

#include "da-clue/genericscript.h"
#include "da-clue/analyzedline.h"

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
 * \brief The AnalyzedScript class provides a script that was analized by ClueEngine
 */
class AnalyzedScript : public GenericScript<AnalyzedLine>
{
public:

    /**
     * Constructs an empty AnalyzedScript
     */
    AnalyzedScript()
        : coverage(0) { }

    /**
     * Constructs an Script with \a filename, main \a character and script \a number
     */
    AnalyzedScript(const QString &filename, const QString &character, int number)
        : GenericScript<AnalyzedLine>(filename, character, number), coverage(0) { }


    float coverage; ///< The script overall coverage


    /**
     * Returns true if \a this instance is equal to \a other
     */
    bool operator==(const AnalyzedScript &other) const
    {
        return GenericScript<AnalyzedLine>::operator==(other) && coverage == other.coverage;
    }

    /**
     * Returns true if \a this instance is *not* equeal to \a other
     */
    bool operator!=(const AnalyzedScript &other) const
    {
        return !this->operator==(other);
    }
};


/**
 * \brief The AnalyzedList class provides a list of AnalyzedScript's
 */
typedef QList<AnalyzedScript> AnalyzedList;

/// @}

} // namespace Clue

/// @}

} // namespace Lvk


#endif // LVK_CLUE_ANALYZEDSCRIPT_H

