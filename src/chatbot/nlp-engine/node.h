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
 * \brief The Node class provides a node of a Tree
 *
 * Node is the base class for all nodes such as WordNode, WildcardNode and VariableNode
 *
 * \see Tree, WordNode, WildcardNode and VariableNode
 */
class Node
{
public:

    /**
     * Constructs a Node object with \a parent
     */
    Node(Node *parent = 0)
        : parent(parent), m_useCount(0) { }

    Node *parent;           ///< The node's parent
    OutputMap omap;         ///< The node's output map

    /**
     * Returns a reference to the list of childs.
     * Warning: Do not append childs to that list. To append new childs use appendChild()
     */
    QList<Node *> & childs()
    {
        return m_childs;
    }

    /**
     * Returns a const reference to the list of childs
     */
    const QList<Node *> & childs() const
    {
        return m_childs;
    }

    /**
     * Appends a child \a node
     */
    void appendChild(Node *node)
    {
        m_childs.append(node);
        ++node->m_useCount;
    }

    /**
     * Returns the string representation of the object
     */
    virtual QString toString() const
    {
        return "Node()";
    }

    /**
     * Returns true if \a this is of type T
     */
    template<class T>
    bool is() const
    {
        return dynamic_cast<const T *>(this) != 0;
    }

    /**
     * if \a this is has type T returns a pointer of type T. Otherwise; returns 0
     */
    template<class T>
    T * to()
    {
        return dynamic_cast<T *>(this);
    }

    /**
     * if \a this is has type T returns a const pointer of type T. Otherwise; returns 0
     */
    template<class T>
    const T * to() const
    {
        return dynamic_cast<const T *>(this);
    }

    /**
     * Destroys the object and all child nodes not being used anymore
     */
    virtual ~Node()
    {
        foreach (Node *child, m_childs) {
            --child->m_useCount;
            if (child->m_useCount == 0 && child != this) {
                delete child;
            }
        }
    }

private:
    Node(const Node&);
    Node& operator=(const Node&);

    int m_useCount;
    QList<Node *> m_childs;
};

/**
 * \brief The WordNode class provides a Node that contains information about a word
 */
class WordNode : public Node
{
public:

    /**
     * Constructs a WordNode object with word \a w and \a parent
     */
    WordNode(const Word &w = Word(), Node *parent = 0)
        : Node(parent), word(w) { }

    Word word; ///< The word information

    /**
     * Returns the string representation of the object
     */
    QString toString() const
    {
        return QString("WordNode(%1)").arg(word.origWord);
    }
};

/**
 * \brief The WildcarNode class provides a Node that contains information about a wildcard operator
 */
class WildcardNode : public Node
{
public:

    /**
     * Constructs a WildcarNode object with range [min,max] \a parent
     */
    WildcardNode(int min = 0, int max = 0, Node *parent = 0)
        : Node(parent), min(min), max(max) { }

    /**
     * Constructs a WildcarNode from string \a wc with \a parent. \wc must be the string
     * representation of a wildcard operator such as STAR_OP or PLUS_OP
     */
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

    int min; ///< The minimun amount of ocurrences
    int max; ///< The maximun amount of ocurrences. -1 means infinite.

    /**
     * Returns the string representation of the object
     */
    QString toString() const
    {
        return QString("WildcardNode(%1,%2)").arg(min).arg(max);
    }
};


/**
 * \brief The VariableNode class provides a Node that contains information about a variable
 */
class VariableNode : public Node
{
public:

    /**
     * Constructs a VariableNode object with variable name \a varName and \a parent
     */
    VariableNode(const QString &varName = QString(), Node *parent = 0)
        : Node(parent), varName(varName) { }

    QString varName; ///< The variable name

    /**
     * Returns the string representation of the object
     */
    QString toString() const
    {
        return "VariableNode(" + varName + ")";
    }
};


/**
 * \brief This method adds support to print debug information of Node objects
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

