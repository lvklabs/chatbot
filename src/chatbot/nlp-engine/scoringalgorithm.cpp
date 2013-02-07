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

#include "scoringalgorithm.h"

//--------------------------------------------------------------------------------------------------
// ScoringAlgorithm
//--------------------------------------------------------------------------------------------------

Lvk::Nlp::ScoringAlgorithm::ScoringAlgorithm()
{
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::ScoringAlgorithm::updateScore(int tokenIndex, float weight)
{
    for (int i = m_weights.size(); i <= tokenIndex; ++i) {
        m_weights.append(0);
    }

    m_weights[tokenIndex] = weight;

    for (int i = tokenIndex + 1; i < m_weights.size(); ++i) {
        m_weights[i] = 0;
    }
}

//--------------------------------------------------------------------------------------------------

float Lvk::Nlp::ScoringAlgorithm::currentScore()
{
    float score = 0;

    foreach (float w, m_weights) {
        score += w;
    }

    return score;
}

