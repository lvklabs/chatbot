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

#ifndef LVK_CLUE_CHARACTER_H
#define LVK_CLUE_CHARACTER_H

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
 * \brief The Character class provides information about a script character
 */
class Character
{
public:

    /**
     * Constucts a Characater with \a name and \a detective flag
     */
    Character(const QString &name = "", bool detective = false)
        : name(name), detective(detective) { }

    QString name;       ///< The character name
    bool detective;     ///< True if the character is the dectective

    /**
     * Return true if \a this is equal to \a other. Otherwise; returns false
     */
    bool operator==(const Character &other)
    {
        return name == other.name && detective == other.detective;
    }

    /**
     * Return true if \a this is *not* equal to \a other. Otherwise; returns false
     */
    bool operator!=(const Character &other)
    {
        return !this->operator==(other);
    }
};

/// @}

} // namespace Clue

/// @}

} // namespace Lvk


#endif // LVK_CLUE_CHARACTER_H

