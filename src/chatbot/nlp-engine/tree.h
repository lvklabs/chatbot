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

#ifndef LVK_NLP_TREE_H
#define LVK_NLP_TREE_H

#include <QString>
#include <QStringList>
#include <QPair>
#include <QList>
#include <QSet>

#include "nlp-engine/engine.h"
#include "nlp-engine/word.h"
#include "nlp-engine/result.h"
#include "nlp-engine/parser.h"
#include "nlp-engine/searchcontext.h"

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace Nlp
{

class Rule;
class Node;
class MatchPolicy;
class ScoringAlgorithm;

/// \ingroup Lvk
/// \addtogroup Nlp
/// @{

/**
 * \brief The Tree class provides tree to perform searches...
 */
class Tree
{
public:

    /**
     * Constructs an empty tree
     */
    Tree();

    /**
     * Destroys the object
     */
    ~Tree();

    /**
     * Adds \a rule to the tree
     */
    void add(const Rule &rule);

    /**
     * Gets the list of responses for the given input
     */
    void getResponses(const QString &input, Nlp::ResultList &results);

    /**
     * Gets the response with the highest score
     */
    void getResponse(const QString &input, Nlp::Result &result);

private:
    Tree(Tree&);
    Tree& operator=(Tree&);

    typedef QPair<int, Nlp::Node *> PairedNode; // pair (input idx, node)

    Node *m_root;
    MatchPolicy *m_matchPolicy;
    Nlp::Parser m_parser;
    Nlp::SearchContext m_searchCtx;
    QSet< QPair<const Nlp::Node*, int> > m_loopDetector;

    Nlp::Node * addNode(const Nlp::Word &word, Nlp::Node *parent);
    void addNodeOutput(const Rule &rule, const QSet<PairedNode> &onodes);
    void scoredDFS(ResultList &r, const Nlp::Node *root, const Nlp::WordList &words,
                   int offset = 0);
    void handleEndWord(Nlp::ResultList &results, const Nlp::Node *node, int offset);
    Nlp::ResultList getResultsForNode(const Nlp::Node *node);
    QString expandVars(const QString &output, bool *ok);
    void parseRuleInput(const QString &input, Nlp::WordList &words);
    void parseUserInput(const QString &input, Nlp::WordList &words);
    void checkSyntax(Nlp::WordList &words);
    void filterSymbols(Nlp::WordList &words);
    void parseExactMatch(Nlp::WordList &words);
};

/// @}

} // namespace Nlp

/// @}

} // namespace Lvk


#endif // LVK_NLP_TREE_H

