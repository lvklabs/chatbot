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
 * \see Script and AnalyzedScript
 */
template<class LineType>
class GenericScript : public QList<LineType>
{
public:

    /**
     * Constructs an empty GenericScript
     */
    GenericScript()
        : number(0) { }

    /**
     * Constructs an GenericScript with \a filename, main \a character and script \a number
     */
    GenericScript(const QString &filename, const QString &character, int number)
        : filename(filename), character(character), number(number) { }

    QString filename;      ///< The filename where the script was read
    QString character;     ///< The script main character
    int number;            ///< The script number

    /**
     * Returns true if \a this instance is equeal to \a other
     */
    bool operator==(const GenericScript<LineType> &other) const
    {
        return QList<LineType>::operator==(other) &&
                filename == other.filename &&
                character == other.character &&
                number == other.number;
    }

    /**
     * Returns true if \a this instance is *not* equeal to \a other
     */
    bool operator!=(const GenericScript<LineType> &other) const
    {
        return !this->operator==(other);
    }

    /**
     * Clears the content of the script
     */
    void clear()
    {
        QList<LineType>::clear();
        filename.clear();
        character.clear();
        number = 0;
    }
};

/// @}

} // namespace Clue

/// @}

} // namespace Lvk


#endif // LVK_CLUE_GENERICSCRIPT_H

