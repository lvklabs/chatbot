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

#include "nlp-engine/varstack.h"

#define CAPTURE_SEP         " "

//--------------------------------------------------------------------------------------------------
// Helpers
//--------------------------------------------------------------------------------------------------

namespace
{

inline void removeLastWord(QString &s)
{
    int n = s.lastIndexOf(CAPTURE_SEP);

    s = (n == -1) ? QString() : s.mid(0, n);
}

} // namespace


//--------------------------------------------------------------------------------------------------
// VarStack
//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::VarStack::update(const QString &varName, int offset)
{
    // If current variable out of scope
    while (m_stack.size() > 0 && m_stack.last().scope.start >= offset) {
        m_stack.removeLast();
    }

    // Rewind
    while (m_stack.size() > 0 && m_stack.last().scope.end >= offset) {
        m_stack.last().scope.end -= 1;
        removeLastWord(m_stack.last().value); // Ugly! TODO move this out and optimize!
    }

    if (!varName.isEmpty()) {
        if (m_stack.isEmpty()) {
            m_stack.append(Nlp::Variable(varName, Nlp::VarScope(offset, offset)));
        } else {
            if (m_stack.last().name != varName) {
                m_stack.append(Nlp::Variable(varName, Nlp::VarScope(offset, offset)));
            } else {
                m_stack.last().scope.end = offset;
            }
        }
    }

    //qDebug() << "VarStack:" << m_stack;
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::VarStack::capture(const QString &word, int offset)
{
    // If there is a variable in scope
    if (!m_stack.isEmpty() && m_stack.last().scope.contains(offset)) {
        QString &value = m_stack.last().value;
        if (value.size() > 0) {
            value.append(CAPTURE_SEP);
        }
        value.append(word);
    }
}

//--------------------------------------------------------------------------------------------------

QString Lvk::Nlp::VarStack::value(const QString &varName) const
{
    for (int i = m_stack.size() - 1; i >= 0; --i) {
        if (m_stack[i].name == varName) {
            return m_stack[i].value;
        }
    }
    return QString();
}
