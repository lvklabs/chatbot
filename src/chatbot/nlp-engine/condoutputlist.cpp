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

#include "nlp-engine/condoutputlist.h"
#include "nlp-engine/varstack.h"
#include "common/random.h"

//--------------------------------------------------------------------------------------------------
// CondOutputList
//--------------------------------------------------------------------------------------------------

Lvk::Nlp::CondOutputList::CondOutputList(const QStringList &outputs, bool random)
    : m_next(0)
{
    setRandomOutput(random);

    foreach (const QString &o, outputs) {
        QString rawOutput = o.trimmed();
        if (!rawOutput.isEmpty()) {
            append(Nlp::CondOutput::fromRawString(rawOutput));
        }
    }
}

//--------------------------------------------------------------------------------------------------

QString Lvk::Nlp::CondOutputList::nextValidOutput(const Nlp::VarStack &varStack) const
{
    // If random
    if (m_next == -1) {
        QList<QString> valid;
        for (int i = 0; i < size(); ++i) {
            QString output;
            if (at(i).eval(varStack, output)) {
                valid.append(output);
            }
        }
        if (!valid.isEmpty()) {
            return valid[Cmn::Random::getInt(0, valid.size() - 1)];
        }
    // if secuential
    } else {
        for (int i = 0; i < size(); ++i) {
            int j = (m_next  + i) % size();
            QString output;
            if (at(j).eval(varStack, output)) {
                m_next = j + 1;
                return output;
            }
        }
    }

    return QString();
 }

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::CondOutputList::setRandomOutput(bool random)
{
    m_next = random ? -1 : 0;
}

