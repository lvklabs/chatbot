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

#ifndef LVK_CLUE_CLUEENGINE_H
#define LVK_CLUE_CLUEENGINE_H

#include "nlp-engine/rule.h"
#include "nlp-engine/engine.h"
#include "da-clue/script.h"

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace Clue
{

/// \ingroup Lvk
/// \addtogroup Clue
/// @{

class AnalizedScript;

/**
 * \brief The ClueEngine class provides the engine to analize scripts againts a set of rules.
 */
class ClueEngine
{
public:

    /**
     * Creates a ClueEngine object
     */
    ClueEngine();

    /**
     * Sets \a rules as the current rules to analize scripts
     */
    void setRules(const Nlp::RuleList &rules);

    /**
     * Clears the rules
     */
    void clear();

    /**
     * Analizes \a script and returns a AnalizedScript \a ascript object with the result
     */
    void analize(const Clue::Script & script, Clue::AnalizedScript &ascript);

};

/// @}

} // namespace Clue

/// @}

} // namespace Lvk


#endif // LVK_CLUE_CLUEENGINE_H

