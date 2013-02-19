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

#include "nlp-engine/matchpolicy.h"
#include "nlp-engine/node.h"
#include "nlp-engine/word.h"

//--------------------------------------------------------------------------------------------------
// MatchPolicy
//--------------------------------------------------------------------------------------------------

float Lvk::Nlp::MatchPolicy::operator()(const Nlp::Node *node, const Nlp::Word &word)
{
    float weight = 0.0;

    if (node->is<Nlp::WildcardNode>()) {
        weight = 0.001;
    } else if (node->is<Nlp::VariableNode>()) {
        weight = 0.001;
    } else if (const Nlp::WordNode *wNode = node->to<Nlp::WordNode>()) {
        if (wNode->word.origWord == word.origWord) {
            weight = 1.0;
        } else if (wNode->word.lemma.size() > 0) {
            if (wNode->word.lemma == word.lemma) {
                weight = 0.5;
            }
        }
    }

    return weight;
}
