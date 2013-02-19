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
 * \brief
 */
class Predicate
{
public:
    virtual ~Predicate() { }

    virtual bool eval(const Nlp::VarStack &varStack) const = 0;
};

/**
 * \brief
 */
class True : public Predicate
{
public:
    virtual bool eval(const Nlp::VarStack &/*varStack*/) const
    {
        return true;
    }
};

/**
 * \brief
 */
class False : public Predicate
{
public:
    virtual bool eval(const Nlp::VarStack &/*varStack*/) const
    {
        return false;
    }
};

/**
 * \brief
 */
template<typename T1,typename T2>
class Comparison : public Predicate
{
public:

    Comparison(const T1 &c1, const T2 &c2)
        : comp1(c1), comp2(c2) { }

    T1 comp1;
    T2 comp2;

    virtual bool eval(const Nlp::VarStack &varStack) const
    {
        return eval(comp1, comp2, varStack);
    }

    bool eval(int i, int j, const Nlp::VarStack &/*varStack*/)
    {
        return j == i;
    }

    bool eval(const QString s1, const QString s2, const Nlp::VarStack &varStack)
    {
        bool ok1, ok2;
        int i = s1.toInt(&ok1);
        int j = s2.toInt(&ok2);

        return (ok1 && ok2) ? eval(i, j, varStack) : (s1 == s2);
    }

    bool eval(const Nlp::Variable v, const QString s, const Nlp::VarStack &varStack)
    {
        return eval(varStack.value(v.name), s);
    }

    bool eval(const QString s, const Nlp::Variable v, const Nlp::VarStack &varStack)
    {
        return eval(varStack.value(v.name), s);
    }

    bool eval(const Nlp::Variable v1, const Nlp::Variable v2, const Nlp::VarStack &varStack)
    {
        return eval(varStack.value(v1.name), varStack.value(v2.name));
    }
};

/// @}

} // namespace Nlp

/// @}

} // namespace Lvk

#endif // LVK_NLP_PREDICATE_H
