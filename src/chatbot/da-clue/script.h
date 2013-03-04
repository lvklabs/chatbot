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

#ifndef LVK_CLUE_SCRIPT_H
#define LVK_CLUE_SCRIPT_H

#include "da-clue/genericscript.h"
#include "da-clue/scriptline.h"

#include <QList>
#include <QtDebug>

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
 * \brief The Script class provides an ordinary script
 */
class Script : public GenericScript<ScriptLine>
{
public:

    /**
     * Constructs an empty Script
     */
    Script() { }

    /**
     * Constructs an Script with \a filename, main \a character and script \a number
     */
    Script(const QString &filename, const QString &character, int number)
        : GenericScript<ScriptLine>(filename, character, number) { }

};

/**
 * \brief This function adds support to print debug information of Script objects
 */
inline QDebug& operator<<(QDebug& dbg, const Script &s)
{
    dbg.nospace() << s.filename << ":";
    foreach (const ScriptLine &line, s) {
        dbg.nospace() << line;
    }
    return dbg.space();
}


/**
 * \brief The ScriptList class provides a list of scripts
 */
typedef QList<Script> ScriptList;

/// @}

} // namespace Clue

/// @}

} // namespace Lvk


#endif // LVK_CLUE_SCRIPT_H

