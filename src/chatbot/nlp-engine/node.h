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
#include "nlp-engine/condoutputlist.h"

#include <QList>
#include <QString>
#include <QDebug>
#include <QHash>

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
typedef QHash<quint64, CondOutputList> OutputMap;

/**
 * \brief
 */
struct Node
{
    Node(Node *parent = 0)
        : parent(parent) { }

    virtual ~Node() { }

    Node *parent;
    QList<Node *> childs;
    OutputMap omap;

    virtual QString toString() const
    {
        return "Node()";
    }

    template<class T>
    bool is() const
    {
        return dynamic_cast<const T *>(this) != 0;
    }

    template<class T>
    T * to()
    {
        return dynamic_cast<T *>(this);
    }

    template<class T>
    const T * to() const
    {
        return dynamic_cast<const T *>(this);
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
        return QString("WordNode(%1)").arg(word.origWord);
    }
};

/**
 * \brief
 */
struct WildcardNode : Node
{
    WildcardNode(int min = 0, int max = 0, Node *parent = 0)
        : Node(parent), min(min), max(max) { }

    WildcardNode(const QString &wc, Node *parent = 0)
        : Node(parent), min(0), max(0)
    {
        if (wc == STAR_OP) {
            min = 0;
            max = -1;
        } else if (wc == PLUS_OP) {
            min = 1;
            max = -1;
        }
    }

    int min;
    int max;

    QString toString() const
    {
        return QString("WildcardNode(%1,%2)").arg(min).arg(max);
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

