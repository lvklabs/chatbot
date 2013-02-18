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

#define CAPTURE_SEP " "

inline void removeLastWord(QString &s)
{
    int n = s.lastIndexOf(CAPTURE_SEP);

    s = (n == -1) ? QString() : s.mid(0, n);
}

//--------------------------------------------------------------------------------------------------
// MatchPolicy
//--------------------------------------------------------------------------------------------------

float Lvk::Nlp::MatchPolicy::operator()(const Nlp::Node *node, const Nlp::WordList &words,
                                        int offset)
{
    float weight = 0.0;

    if (node->is<Nlp::WildcardNode>()) {
        weight = 0.001;
    } else if (node->is<Nlp::VariableNode>()) {
        weight = 0.001;
    } else if (const Nlp::WordNode *wNode = node->to<Nlp::WordNode>()) {
        if (wNode->word.origWord == words[offset].origWord) {
            weight = 1.0;
        } else if (wNode->word.lemma.size() > 0) {
            if (wNode->word.lemma == words[offset].lemma) {
                weight = 0.5;
            }
        }
    }

    updateVarStack(node, offset);

    // If there is match
    if (weight > 0.0) {
        // If there is a variable in scope
        if (!m_stack.isEmpty() && m_stack.last().scope.contains(offset)) {
            QString &capture = m_stack.last().capture;
            if (capture.size() > 0) {
                capture.append(CAPTURE_SEP);
            }
            capture.append(words[offset].origWord);
        }
    }

    return weight;
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::MatchPolicy::updateVarStack(const Nlp::Node *node, int offset)
{
    // If current variable out of scope
    while (m_stack.size() > 0 && m_stack.last().scope.start >= offset) {
        m_stack.removeLast();
    }

    // Rewind
    while (m_stack.size() > 0 && m_stack.last().scope.end >= offset) {
        m_stack.last().scope.end -= 1;
        removeLastWord(m_stack.last().capture); // Ugly! TODO move this out and optimize!
    }

    const Nlp::VariableNode *varNode = node->to<Nlp::VariableNode>();

    if (varNode) {
        if (m_stack.isEmpty()) {
            m_stack.append(Nlp::VarInfo(varNode->varName, Nlp::VarScope(offset, offset)));
        } else {
            if (m_stack.last().name != varNode->varName) {
                m_stack.append(Nlp::VarInfo(varNode->varName, Nlp::VarScope(offset, offset)));
            } else {
                m_stack.last().scope.end = offset;
            }
        }
    } else {
        // Nothing to do
    }

    qDebug() << "VarStack:" << m_stack;
}

//--------------------------------------------------------------------------------------------------

QString Lvk::Nlp::MatchPolicy::getCapture(const QString &varName)
{
    for (int i = m_stack.size() - 1; i >= 0; --i) {
        if (m_stack[i].name == varName) {
            return m_stack[i].capture;
        }
    }
    return QString();
}
