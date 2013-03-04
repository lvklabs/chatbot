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

#ifndef LVK_CLUE_ANALIZEDLINE_H
#define LVK_CLUE_ANALIZEDLINE_H

#include "da-clue/scriptline.h"
#include "nlp-engine/rule.h"

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
 * \brief The AnalizedLine class provides an analized script line by the ClueEngine
 */
class AnalizedLine : public ScriptLine
{
public:

    /**
     * Constructs an empty AnalizedLine
     */
    AnalizedLine()
        : ruleId(0), score(0) { }

    Nlp::RuleId ruleId; ///< The rule ID of the matched rule. 0 if there is no match.
    float score;        ///< The score of the matched rule. 0 if there is no match.
};

/// @}

} // namespace Clue

/// @}

} // namespace Lvk


#endif // LVK_CLUE_ANALIZEDLINE_H
