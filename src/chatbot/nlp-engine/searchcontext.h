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

#ifndef LVK_NLP_SEARCHCONTEXT_H
#define LVK_NLP_SEARCHCONTEXT_H

#include "nlp-engine/scoringalgorithm.h"
#include "nlp-engine/varstack.h"

#include <QList>

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
 * \brief the SearchContext class provides a handy stack to push and pop search contexts.
 *
 * The SearchContext class is used internally by the Tree class. Before starting a DFS search
 * it pushes a context, when the search finishes pops it. During recursive searches several
 * context are pushed and poped.
 */
class SearchContext
{
public:
    void push()
    {
        m_scores.append(Nlp::ScoringAlgorithm());
        m_stacks.append(Nlp::VarStack());
    }

    void pop()
    {
        m_scores.removeLast();
        m_stacks.removeLast();
    }

    Nlp::ScoringAlgorithm & score()
    {
        return m_scores.last();
    }

    Nlp::VarStack & stack()
    {
        return m_stacks.last();
    }

    bool isEmpty() const
    {
        return m_scores.isEmpty();
    }

private:
    QList<Nlp::ScoringAlgorithm> m_scores;
    QList<Nlp::VarStack> m_stacks;
};

/// @}

} // namespace Nlp

/// @}

} // namespace Lvk


#endif // LVK_NLP_SEARCHCONTEXT_H

