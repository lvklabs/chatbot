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

#ifndef LVK_NLP_COMPARISON_H
#define LVK_NLP_COMPARISON_H

#include "nlp-engine/predicate.h"

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
 * Comparison types
 */
// TODO consider using functors
enum CompType
{
    Equal,
    Less,
    Greater,
    LessOrEqual,
    GreaterOrEqual,
    NotEqual
};

/**
 * \brief The comparison class provides a Predicate that compares two values.
 *
 * Values Must be of type int, QString and Nlp::Variable
 */
template<typename T1, typename T2>
class Comparison : public Predicate
{
public:

    /**
     * Constructs a Comparison object
     */
    Comparison(const T1 &c1, const T2 &c2, CompType type)
        : m_comp1(c1), m_comp2(c2), m_type(type) { }

    /**
     * \copydoc Predicate::eval()
     */
    virtual bool eval(const Nlp::VarStack &varStack) const
    {
        return eval(m_comp1, m_comp2, varStack);
    }

private:

    T1 m_comp1;
    T2 m_comp2;
    CompType m_type;

    bool eval(const Nlp::Variable &v, const QString &s, const Nlp::VarStack &varStack) const
    {
        return eval(varStack.value(v.name), s, varStack);
    }

    bool eval(const QString &s, const Nlp::Variable &v, const Nlp::VarStack &varStack) const
    {
        return eval(varStack.value(v.name), s, varStack);
    }

    bool eval(const Nlp::Variable &v1, const Nlp::Variable &v2, const Nlp::VarStack &varStack) const
    {
        return eval(varStack.value(v1.name), varStack.value(v2.name), varStack);
    }

    bool eval(const QString &s1, const QString &s2, const Nlp::VarStack &varStack) const
    {
        bool ok1, ok2;
        int i = s1.toInt(&ok1);
        int j = s2.toInt(&ok2);

        if (ok1 && ok2) {
            return eval(i, j, varStack);
        } else {
            switch (m_type) {
            case Equal:
                return s1.compare(s2, Qt::CaseInsensitive) == 0;
            case Less:
                return s1.compare(s2, Qt::CaseInsensitive) < 0;
            case Greater:
                return s1.compare(s2, Qt::CaseInsensitive) > 0;
            case LessOrEqual:
                return s1.compare(s2, Qt::CaseInsensitive) <= 0;
            case GreaterOrEqual:
                return s1.compare(s2, Qt::CaseInsensitive) >= 0;
            case NotEqual:
                return s1.compare(s2, Qt::CaseInsensitive) != 0;
            default:
                return false;
            }
        }
   }

    bool eval(int i, int j, const Nlp::VarStack &/*varStack*/) const
    {
        switch (m_type) {
        case Equal:
            return i == j;
        case Less:
            return i < j;
        case Greater:
            return i > j;
        case LessOrEqual:
            return i <= j;
        case GreaterOrEqual:
            return i >= j;
        case NotEqual:
            return i != j;
        default:
            return false;
        }
    }
};

/// @}

} // namespace Nlp

/// @}

} // namespace Lvk


#endif // LVK_NLP_COMPARISON_H

