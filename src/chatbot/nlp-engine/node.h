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

#ifndef LVK_NLP_NODE_H
#define LVK_NLP_NODE_H

#include "nlp-engine/word.h"
#include "nlp-engine/rule.h"

#include <QList>
#include <QString>
//#include <QSet>

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace Nlp
{

/// \ingroup Lvk
/// \addtogroup Nlp
/// @{

struct Predicate
{
    virtual ~Predicate() { }

    bool operator()() const
    {
        return true;
    }

};

struct CondOutput
{
    CondOutput(const QString &output = "", int index = 0, const Predicate &p = Predicate())
        : output(output), index(index), predicate(p) { }

    QString output;
    int index;
    Predicate predicate;
};

struct Node
{
    Node(Node *parent = 0)
        : parent(parent), randomOutput(false), nextOutput(0) { }

    virtual ~Node() { }

    Node *parent;
    QList<Node *> childs;
    QList<CondOutput> outputs;
    bool randomOutput;
    int nextOutput;
    //QSet<Nlp::RuleId> ruleIds;
};

struct WordNode : Node
{
    WordNode(const Word &w = Word(), Node *parent = 0)
        : Node(parent), word(w) { }

    Word word;
};

struct WildcardNode : Node
{
    WildcardNode(Node *parent = 0)
        : Node(parent) { }
};

struct VariableNode : Node
{
    VariableNode(const QString &varName = QString(), Node *parent = 0)
        : Node(parent), varName(varName) { }

    QString varName;
};

/// @}

} // namespace Nlp

/// @}

} // namespace Lvk


#endif // LVK_NLP_NODE_H

