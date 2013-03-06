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

#ifndef LVK_CLUE_SCRIPTLINE_H
#define LVK_CLUE_SCRIPTLINE_H

#include "da-clue/character.h"

#include <QString>
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
 * \brief The ScriptLine class provides a single line in a Clue Script
 */
class ScriptLine
{
public:

    /**
     * Line importance
     */
    enum Importance
    {
        Standard,
        Critical
    };

    /**
     * Constructs an empty ScriptLine
     */
    ScriptLine()
        : importance(Standard) { }

    /**
     * Constructs an ScriptLine with \a question expected answer \a expAnswer, and \a hint
     */
    ScriptLine(const QString &question, const QString &expAnswer, const QString &hint)
        : question(question), expAnswer(expAnswer), hint(hint), importance(Standard) { }

    /**
     * Constructs an ScriptLine with \a question expected answer \a expAnswer, forbidden answer
     * \a forbidAnswer, \a hint, and \a importance
     */
    ScriptLine(const QString &question, const QString &expAnswer, const QString &forbidAnswer,
               const QString &hint, Importance importance)
        : question(question), expAnswer(expAnswer), forbidAnswer(forbidAnswer), hint(hint),
          importance(importance) { }


    QString question;      ///< The detective question
    QString expAnswer;     ///< The suspect expected answer pattern
    QString forbidAnswer;  ///< The suspect forbidden answer pattern
    QString hint;          ///< The hint in case of rule mismatch
    Importance importance; ///< The question importance


    /**
     * Returns true if \a this instance is equeal to \a other
     */
    bool operator==(const ScriptLine &other) const
    {
        return question == other.question &&
                expAnswer == other.expAnswer &&
                forbidAnswer == other.forbidAnswer &&
                hint == other.hint &&
                importance == other.importance;
    }

    /**
     * Returns true if \a this instance is *not* equeal to \a other
     */
    bool operator!=(const ScriptLine &other) const
    {
        return !this->operator==(other);
    }
};


/**
 * \brief This function adds support to print debug information of ScriptLine objects
 */
inline QDebug& operator<<(QDebug& dbg, const ScriptLine &line)
{
    dbg.space() << line.question << line.expAnswer;
    return dbg.space();
}

/// @}

} // namespace Clue

/// @}

} // namespace Lvk


#endif // LVK_CLUE_SCRIPTLINE_H

