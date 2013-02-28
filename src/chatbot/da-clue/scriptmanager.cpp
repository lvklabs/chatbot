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

#include "da-clue/scriptmanager.h"

//--------------------------------------------------------------------------------------------------
// ScriptManager
//--------------------------------------------------------------------------------------------------

Lvk::Clue::ScriptManager::ScriptManager()
{
}

//--------------------------------------------------------------------------------------------------

QList<Lvk::Clue::Character> Lvk::Clue::ScriptManager::characters()
{
    QList<Lvk::Clue::Character> chars;

    // TODO

    return chars;
}

//--------------------------------------------------------------------------------------------------

Lvk::Clue::ScriptError Lvk::Clue::ScriptManager::loadScriptsForCharacter(
        const Clue::Character &/*c*/)
{
    // TODO

    return Clue::UnknownError;
}

//--------------------------------------------------------------------------------------------------

const QList<Lvk::Clue::Script> & Lvk::Clue::ScriptManager::scripts()
{
    return m_scripts;
}

//--------------------------------------------------------------------------------------------------

void Lvk::Clue::ScriptManager::clear()
{
    m_scripts.clear();
}

