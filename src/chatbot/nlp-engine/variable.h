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

#ifndef LVK_NLP_VARIABLE_H
#define LVK_NLP_VARIABLE_H

#include <QtDebug>
#include <QString>

#include "nlp-engine/varscope.h"


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
 * \brief The Variable class provides all the information of a variable
 *
 * This class is used internally by the Cb2Engine to store information about variables
 */
class Variable
{
public:
    /**
     * Constructs a Variable object with \a name and \a scope
     */
    Variable(const QString &name = "", const VarScope &scope = VarScope())
        : name(name), scope(scope) { }

    QString name;   ///< The variable name
    QString value;  ///< The variable value
    VarScope scope; ///< The variable scope

    /**
     * Returns true if the variable is null. Otherwise; returns false.
     */
    bool isNull()
    {
        return name.isNull() && scope.isNull() && value.isNull();
    }

    /**
     * Clears the content of the object
     */
    void clear()
    {
        name.clear();
        scope.clear();
        value.clear();
    }
};


/**
 * \brief This method adds support to print debug information of Variable objects
 */
inline QDebug& operator<<(QDebug& dbg, const Variable &i)
{
    dbg.nospace() << i.name << "," << i.scope << "," << i.value;
    return dbg.maybeSpace();
}

/// @}

} // namespace Nlp

/// @}

} // namespace Lvk


#endif // LVK_NLP_VARIABLE_H

