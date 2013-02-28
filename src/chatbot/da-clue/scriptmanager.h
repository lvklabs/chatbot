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

#ifndef LVK_CLUE_SCRIPTMANAGER_H
#define LVK_CLUE_SCRIPTMANAGER_H

#include "da-clue/character.h"
#include "da-clue/script.h"
#include "da-clue/scripterror.h"

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
 * \brief The ScriptManager class manages all the scripts and characters in the system
 */
class ScriptManager
{
public:

    /**
     * Creates a ScriptManager object
     */
    ScriptManager();

    /**
     * Returns the list of available characters
     */
    QList<Clue::Character> characters();

    /**
     * Loads the scripts for the character \a c
     */
    Clue::ScriptError loadScriptsForCharacter(const Clue::Character &c);

    /**
     * Returns the list of scripts currently loaded.
     */
    const QList<Clue::Script> &scripts();

    /**
     * Clears the scripts loaded
     */
    void clear();

private:
    QList<Clue::Script> m_scripts;
};

/// @}

} // namespace Clue

/// @}

} // namespace Lvk


#endif // LVK_CLUE_SCRIPTMANAGER_H

