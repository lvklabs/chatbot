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

#include "nlp-engine/tree.h"
#include "nlp-engine/node.h"
#include "nlp-engine/word.h"
#include "nlp-engine/globaltools.h"
#include "nlp-engine/matchpolicy.h"
#include "nlp-engine/scoringalgorithm.h"

#include <QtAlgorithms>

#define MAX_INPUT_IDX_SIZE  10   // in bits
#define INPUT_IDX_MASK      ((1 << MAX_INPUT_IDX_SIZE) - 1)

//--------------------------------------------------------------------------------------------------
// Helpers
//--------------------------------------------------------------------------------------------------

namespace
{

inline bool highScoreFirst(const Lvk::Nlp::Result &r1, const Lvk::Nlp::Result &r2)
{
    return r1.score > r2.score;
}

//--------------------------------------------------------------------------------------------------

inline quint64 getOmapId(Lvk::Nlp::RuleId ruleId, int inputIdx)
{
    return (inputIdx & INPUT_IDX_MASK) + (ruleId << MAX_INPUT_IDX_SIZE);
}

//--------------------------------------------------------------------------------------------------

inline Lvk::Nlp::RuleId getRuleId(quint64 id)
{
    return id >> MAX_INPUT_IDX_SIZE;
}

//--------------------------------------------------------------------------------------------------

inline int getInputIndex(quint64 id)
{
    return id & INPUT_IDX_MASK;
}

} // namespace

//--------------------------------------------------------------------------------------------------
// Tree
//--------------------------------------------------------------------------------------------------

Lvk::Nlp::Tree::Tree()
    : m_root(new Nlp::Node()),
      m_matchPolicy(new Nlp::MatchPolicy()),
      m_scoringAlg(new Nlp::ScoringAlgorithm())
{
}

//--------------------------------------------------------------------------------------------------

Lvk::Nlp::Tree::~Tree()
{
    delete m_scoringAlg;
    delete m_matchPolicy;
    delete m_root;
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::Tree::add(const Nlp::Rule &rule)
{
    Nlp::WordList words;

    QSet<PairedNode> onodes;    // Set of nodes with output

    // Parse each rule input and add nodes in the tree

    for (int i = 0; i < rule.input().size(); ++i) {

        qDebug() << "Nlp::Tree: Parsing rule id" << rule.id() << "input #" << i;
        parseRuleInput(rule.input().at(i), words);

        if (words.isEmpty()) {
            continue;
        }

        Nlp::Node *curNode = m_root;

        foreach (Nlp::Word w, words) {
            curNode = addNode(w, curNode);
        }

        onodes.insert(PairedNode(i, curNode));

        if (words.last().normWord == STAR_OP && curNode->parent != m_root) {
            onodes.insert(PairedNode(i, curNode->parent));
        }
    }

    // Add rule output to each node in onodes

    addNodeOutput(rule, onodes);
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::Tree::addNodeOutput(const Lvk::Nlp::Rule &rule, const QSet<PairedNode> &onodes)
{
    // Build list of outputs with their condition
    // TODO add condition

    Nlp::CondOutputList l;
    foreach (const QString &o, rule.output()) {
        QString ot = o.trimmed();
        if (!ot.isEmpty()) {
            l.append(Nlp::CondOutput(ot));
        }
    }

    // Add the output list to all nodes in onodes.
    // Because CondOutputList inherits the "Implicit Shared Model" from QList, all These
    // copies don't waste a lot of memory

    foreach (const PairedNode &onode, onodes) {
        onode.second->omap[getOmapId(rule.id(), onode.first)] = l;
    }
}

//--------------------------------------------------------------------------------------------------

Lvk::Nlp::Node * Lvk::Nlp::Tree::addNode(const Nlp::Word &word, Nlp::Node *parent)
{
    // TODO check if we can avoid these casts:

    // If node already exists for the given word, return that node

    if (word.isWord()) {
        foreach (Nlp::Node *node, parent->childs) {
            if (Nlp::WordNode* wNode = node->to<Nlp::WordNode>()) {
                if (wNode->word == word) {
                    return node;
                }
            }
        }
    }

    if (word.isWildcard()) {
        foreach (Nlp::Node *node, parent->childs) {
            if (Nlp::WildcardNode* wcNode = node->to<Nlp::WildcardNode>()) {
                // Currently we only support two wildcards: * and +
                // We must handle the case where new node is a * node and we already have
                // a + node
                if (word.origWord == STAR_OP && wcNode->min == 1) {
                    wcNode->min = 0;
                }
                return node;
            }
        }
    }

    // Otherwise, add new node

    Nlp::Node *newNode = 0;

    if (word.isWildcard()) {
        newNode = new Nlp::WildcardNode(word.origWord, parent);
        newNode->childs.append(newNode); // Loop node (see engine documentation)
    } else if (word.isVariable()) {
        // TODO
    } else {
        newNode = new Nlp::WordNode(word, parent);
    }

    parent->childs.append(newNode);

    // If parent is *, we need to add a new edge from parent->parent to newNode
    // TODO handle case where there are two or more * adjacent
    if (parent->is<Nlp::WildcardNode>() && parent->to<Nlp::WildcardNode>()->min == 0) {
        parent->parent->childs.append(newNode);
    }

    qDebug() << "Nlp::Tree: Added new node" << *newNode << "with parent" << *parent;

    return newNode;
}

//--------------------------------------------------------------------------------------------------

QStringList Lvk::Nlp::Tree::getResponses(const QString &input, Engine::MatchList &matches)
{
    Nlp::WordList words;
    parseUserInput(input, words);

    Nlp::ResultList results;
    scoredDFS(results, m_root, words);

    qSort(results.begin(), results.end(), highScoreFirst);

    QStringList responses;
    matches.clear();

    foreach (const Result &r, results) {
        responses.append(r.output);
        matches.append(Engine::RuleMatch(r.ruleId, r.inputIdx));
    }

    return responses;
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::Tree::scoredDFS(Nlp::ResultList &results, const Nlp::Node *root,
                               const Nlp::WordList &words, int offset /*= 0*/)
{
    QString dgbMargin = QString((offset+1)*4, '#');

    foreach (const Nlp::Node *node, root->childs) {
        qDebug() <<  dgbMargin << "Current node" << *node;

        float matchWeight = (*m_matchPolicy)(node, words[offset]);

        if (matchWeight > 0) {
            qDebug() << dgbMargin << words[offset] << "matched with weight" << matchWeight;

            m_scoringAlg->updateScore(offset, matchWeight);

            if (offset + 1 < words.size()) {
                scoredDFS(results, node, words, offset + 1);
            } else {
                Nlp::Result r = getValidOutput(node);
                if (!r.isNull()) {
                    r.score = m_scoringAlg->currentScore();
                    results.append(r);
                } else {
                    qDebug() << dgbMargin << "No output found!";
                }
            }
        }
    }
}

//--------------------------------------------------------------------------------------------------

Lvk::Nlp::Result Lvk::Nlp::Tree::getValidOutput(const Nlp::Node *node)
{
    Nlp::Result r;

    Nlp::OutputMap::const_iterator it;
    for (it = node->omap.constBegin(); it != node->omap.constEnd() && r.isNull(); ++it) {
        const Nlp::CondOutputList &l = it.value();

        // TODO if random output, choose randomly

        for (int i = 0; i < l.size(); ++i) {
            const Nlp::CondOutput &co = l[i];

            if (co.predicate(/* TODO pass context */)) {
                r.ruleId = getRuleId(it.key());
                r.inputIdx = getInputIndex(it.key());
                r.output = co.output; // TODO expand variables
                break;
            }
        }
    }

    return r;
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::Tree::parseRuleInput(const QString &input, Nlp::WordList &words)
{
    qDebug() << "Nlp::Tree: Parsing rule input" << input;

    words.clear();

    Nlp::GlobalTools::instance()->lemmatizer()->lemmatize(input, words);

     // TODO parse variables

    filterSymbols(words);

    qDebug() << "Nlp::Tree: Parsed rule input" << words;
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::Tree::parseUserInput(const QString &input, Nlp::WordList &words)
{
    qDebug() << "Nlp::Tree: Parsing user input" << input;

    words.clear();

    Nlp::GlobalTools::instance()->lemmatizer()->lemmatize(input, words);

    filterSymbols(words);

    qDebug() << "Nlp::Tree: Parsed user input" << words;
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::Tree::filterSymbols(Nlp::WordList &words)
{
    for (int i = 0; i < words.size();) {
        if (words[i].isSymbol()) {
            words.removeAt(i);
        } else {
            ++i;
        }
    }
}


