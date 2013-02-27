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

#ifndef LVK_NLP_MATCHPOLICY_H
#define LVK_NLP_MATCHPOLICY_H

#include <QString>

#include "nlp-engine/word.h"

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace Nlp
{

/// \ingroup Lvk
/// \addtogroup Nlp
/// @{

struct Node;


/**
 * \brief The MatchPolicy class defines the matching policy to do a search on a Tree
 *
 * Given a Node \a n and a Word \a w, returns "how well" \a w mathes \a n
 */
class MatchPolicy
{
public:

    /**
     * Returns the weight of the node \a n given the word \a w. The weight ranges from 0.0 to 1.0.
     * A zero weight means no match.
     */
    float operator()(const Node *n, const Word &w);
};

/// @}

} // namespace Nlp

/// @}

} // namespace Lvk


#endif // LVK_NLP_MATCHPOLICY_H

