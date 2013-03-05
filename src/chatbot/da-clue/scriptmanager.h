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

class ScriptParser;

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
     * Returns the current character name
     */
    const QString & currentCharacter() const;

    /**
     * Sets the current character name
     */
    void setCurrentCharacter(const QString &name);

    /**
     * Loads the scripts for the current character
     */
    bool loadScripts();

    /**
     * Loads the scripts for the character with \a name
     */
    bool loadScriptsForCharacter(const QString &name);

    /**
     * Loads the scripts for the character \a c
     */
    bool loadScriptsForCharacter(const Clue::Character &c);

    /**
     * Returns the list of scripts currently loaded.
     */
    const Clue::ScriptList &scripts();

    /**
     * Returns the last error. If \a errMsg is not null, returns an string with details
     * of the error
     */
    Clue::ScriptError error(QString *errMsg = 0);

    /**
     * Imports the script pointed by filaname. The script character must match the current
     * character. Returns true on success. Otherwise; returns false.
     */
    bool import(const QString &filename);

    /**
     * Clears the scripts loaded
     */
    void clear();

private:
    QString m_curChar;
    ScriptList m_scripts;
    QString m_clueBasePath;
    QString m_charsPath;
    Clue::ScriptError m_error;
    QString m_errMsg;

    void initPaths();
    bool loadFile(const QString &filename, const QString &name);
    void resetError();
    void setError(Clue::ScriptError err, const QString &filename);
    void setParsingError(const Clue::ScriptParser &parser);
};

/// @}

} // namespace Clue

/// @}

} // namespace Lvk


#endif // LVK_CLUE_SCRIPTMANAGER_H

