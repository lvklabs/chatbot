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

#ifndef LVK_CLUE_GENERICSCRIPT_H
#define LVK_CLUE_GENERICSCRIPT_H

#include <QList>
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
 * \brief The GenericScript class provides a generic class for diferent kind of sripts
 *
 * \see Script and AnalizedScript
 */
template<class LineType>
class GenericScript
{
public:

    QString filename;      ///< The filename where the script was read
    QList<LineType> lines; ///< The script lines

    /**
     * Returns true if the script is empty. Otherwise; returns false.
     */
    bool isEmpty()
    {
        return lines.isEmpty();
    }

    /**
     * Clears the content of the script
     */
    void clear()
    {
        filename.clear();
        lines.clear();
    }
};

/// @}

} // namespace Clue

/// @}

} // namespace Lvk


#endif // LVK_CLUE_GENERICSCRIPT_H

