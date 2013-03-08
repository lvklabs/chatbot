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
#include "da-clue/regexp.h"
#include "da-clue/analyzedscript.h"

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
 * \brief The ClueEngine class provides the engine to analyze scripts againts a set of rules.
 */
class ClueEngine
{
public:

    /**
     * Creates a ClueEngine object
     */
    ClueEngine();

    /**
     * Destroys the object
     */
    ~ClueEngine();

    /**
     * Sets \a rules as the current rules to analize scripts
     */
    void setRules(const Nlp::RuleList &rules);

    /**
     * Sets \a evasive as the response when there is no match
     */
    void setEvasive(const QString &evasive);

    /**
     * Clears the rules
     */
    void clear();

    /**
     * Analyzes \a script and returns a AnalyzedScript \a ascript with the result
     */
    void analyze(const Clue::Script & script, Clue::AnalyzedScript &ascript);

    /**
     * Analyzes \a scripts and returns a AnalyzedList \a ascripts with the result
     */
    void analyze(const Clue::ScriptList & scripts, Clue::AnalyzedList &ascripts);

private:
    ClueEngine(const ClueEngine&);
    ClueEngine & operator=(const ClueEngine&);

    Nlp::Engine *m_engine;
    Clue::RegExp m_regexp;
    QString m_evasive;
};

/// @}

} // namespace Clue

/// @}

} // namespace Lvk


#endif // LVK_CLUE_CLUEENGINE_H

