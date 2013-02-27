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

#ifndef LVK_NLP_VARSCOPE_H
#define LVK_NLP_VARSCOPE_H

#include <QtDebug>

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
 * \brief The VarScope class contains the scope of a variable.
 *
 * \see Variable
 */
class VarScope
{
public:

    /**
     * Constructs a VarScope object with scope [start,end]
     */
    VarScope(int start = 0, int end = 0)
        : start(start), end(end) { }

    int start; ///< The scope start
    int end;   ///< The scope end

    /**
     * Returns true if the scope is null. Otherwise; returns false.
     */
    bool isNull()
    {
        return !start && !end;
    }

    /**
     * Clears the content of the object
     */
    void clear()
    {
        start = 0;
        end = 0;
    }

    /**
     * Returns true if \a i is in range [start,end]. Otherwise; returns false.
     */
    bool contains(int i)
    {
        return start <= i && i <= end;
    }
};


/**
 * \brief This method adds support to print debug information of VarScope objects
 */
inline QDebug& operator<<(QDebug& dbg, const VarScope &s)
{
    dbg.nospace() << s.start << "," << s.end;
    return dbg.maybeSpace();
}

/// @}

} // namespace Nlp

/// @}

} // namespace Lvk

#endif // LVK_NLP_VARSCOPE_H

