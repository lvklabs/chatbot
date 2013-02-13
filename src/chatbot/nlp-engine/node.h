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
#include <QDebug>
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

/**
 * \brief
 */
struct Predicate
{
    virtual ~Predicate() { }

    bool operator()() const
    {
        return true;
    }

};

/**
 * \brief
 */
struct OutputInfo
{
    OutputInfo(const QString &output = "", Nlp::RuleId ruleId = 0, int inputIdx = 0,
               int outputIdx = 0, const Predicate &p = Predicate())
        : output(output), ruleId(ruleId), inputIdx(inputIdx), outputIdx(outputIdx), predicate(p) { }

    QString output;
    Nlp::RuleId ruleId;
    int inputIdx;
    int outputIdx;
    Predicate predicate;
};

/**
 * \brief
 */
struct Node
{
    Node(Node *parent = 0)
        : parent(parent), randomOutput(false), nextOutput(0) { }

    virtual ~Node() { }

    Node *parent;
    QList<Node *> childs;
    QList<OutputInfo> outputs;
    bool randomOutput;
    int nextOutput;

    virtual QString toString() const
    {
        return "Node()";
    }
};

/**
 * \brief
 */
struct WordNode : Node
{
    WordNode(const Word &w = Word(), Node *parent = 0)
        : Node(parent), word(w) { }

    Word word;

    QString toString() const
    {
        return "WordNode(" + word.origWord + ")";
    }
};

/**
 * \brief
 */
struct WildcardNode : Node
{
    WildcardNode(Node *parent = 0)
        : Node(parent) { }

    QString toString() const
    {
        return "WildcardNode()";
    }
};

/**
 * \brief
 */
struct VariableNode : Node
{
    VariableNode(const QString &varName = QString(), Node *parent = 0)
        : Node(parent), varName(varName) { }

    QString varName;

    QString toString() const
    {
        return "VariableNode(" + varName + ")";
    }
};

/**
 * Makes the Node type printable for debugging purposes
 */
inline QDebug& operator<<(QDebug& dbg, const Node &n)
{
    dbg.space() << n.toString();
    return dbg.space();
}

/// @}

} // namespace Nlp

/// @}

} // namespace Lvk


#endif // LVK_NLP_NODE_H

