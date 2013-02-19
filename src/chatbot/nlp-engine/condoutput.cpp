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

#include "nlp-engine/condoutput.h"
#include "nlp-engine/varstack.h"
#include "nlp-engine/predicate.h"

//--------------------------------------------------------------------------------------------------
// CondOutput
//--------------------------------------------------------------------------------------------------

Lvk::Nlp::CondOutput::CondOutput(const QString &rawOutput)
{
    // TODO parse conditionals
    m_outputs.append(rawOutput);
    m_predicates.append(QSharedPointer<Nlp::Predicate>(new True()));

}

//--------------------------------------------------------------------------------------------------

bool Lvk::Nlp::CondOutput::eval(const Nlp::VarStack &varStack, QString &output) const
{
    for (int i = 0; i < m_predicates.size(); ++i) {
        if (m_predicates[i]->eval(varStack)) {
            output = m_outputs[i];
            return true;
        }
    }
    return false;
}

