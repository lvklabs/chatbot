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

#ifndef LVK_BE_SCOREALGORITHM_H
#define LVK_BE_SCOREALGORITHM_H

#include "back-end/score.h"

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace BE
{

/// \ingroup Lvk
/// \addtogroup BE
/// @{

/**
 * \brief The ScoreAlgorithm class implements the algorithm to get the score of a chatbot.
 */
class ScoreAlgorithm
{
public:

    /**
     * Constructs a ScoreAlgorithm object.
     */
    ScoreAlgorithm();

    /**
     * Returns the current score.
     */
    Score score();
};

/// @}

} // namespace BE

/// @}

} // namespace Lvk


#endif // LVK_BE_SCOREALGORITHM_H

