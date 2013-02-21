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
#ifndef LVK_NLP_PREDICATE_H
#define LVK_NLP_PREDICATE_H

#include <functional>
#include <QString>

#include "nlp-engine/varstack.h"

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
 * \brief The Predicate class provides the interface for all predicates
 */
class Predicate
{
public:
    /**
     * Destroys the object
     */
    virtual ~Predicate() { }

    /**
     * Evaluates the predicate using the variable stack \a varStack.
     * Returns \a true if the predicate is satisfied; \a false otherwise.
     */
    virtual bool eval(const Nlp::VarStack &varStack) const = 0;
};


/**
 * \brief The True class is a Predicate that always returns true
 */
class True : public Predicate
{
public:

    /**
     * This method always returns \a true
     */
    virtual bool eval(const Nlp::VarStack &/*varStack*/) const
    {
        return true;
    }
};


/**
 * \brief The False class is a Predicate that always returns false
 */
class False : public Predicate
{
public:

    /**
     * This method always returns \a false
     */
    virtual bool eval(const Nlp::VarStack &/*varStack*/) const
    {
        return false;
    }
};

/// @}

} // namespace Nlp

/// @}

} // namespace Lvk

#endif // LVK_NLP_PREDICATE_H
