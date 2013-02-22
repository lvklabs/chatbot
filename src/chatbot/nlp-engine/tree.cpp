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

#ifdef DEBUG_TRACE
#define TRACE(offset)   (QDebug() << QString((offset+1)*4, '#'))
#else
#define TRACE(offset)   QNoDebug()
#endif

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
      m_matchPolicy(new Nlp::MatchPolicy())
{
}

//--------------------------------------------------------------------------------------------------

Lvk::Nlp::Tree::~Tree()
{
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

    Nlp::CondOutputList l(rule.output(), rule.randomOutput());

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

    if (word.isVariable()) {
        // TODO check
    }

    // Otherwise, add new node

    Nlp::Node *newNode = 0;

    if (word.isWildcard()) {
        newNode = new Nlp::WildcardNode(word.origWord, parent);
        newNode->childs.append(newNode); // Loop node (see engine documentation)
    } else if (word.isVariable()) {
        QString varName = word.origWord.mid(1, word.origWord.size() - 2); // Remove square braces
        newNode = new Nlp::VariableNode(varName, parent);
        newNode->childs.append(newNode); // Loop node (see engine documentation)
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

void Lvk::Nlp::Tree::getResponse(const QString &input, Nlp::Result &result)
{
    result.clear();

    Nlp::ResultList results;
    getResponses(input, results);

    if (!results.isEmpty()) {
        result = results.first();
    }
}
//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::Tree::getResponses(const QString &input, Nlp::ResultList &results)
{
    Nlp::WordList words;
    parseUserInput(input, words);

    m_searchCtx.push();

    scoredDFS(results, m_root, words);

    qSort(results.begin(), results.end(), highScoreFirst);

    m_searchCtx.pop();

    if (m_searchCtx.isEmpty()) {
        m_loopDetector.clear();
    }

    qDebug() << "Nlp::Tree: Results: " << results;
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::Tree::scoredDFS(Nlp::ResultList &results, const Nlp::Node *root,
                               const Nlp::WordList &words, int offset /*= 0*/)
{
    if (offset >= words.size()) {
        return;
    }

    foreach (const Nlp::Node *node, root->childs) {
        TRACE(offset) << "Current node" << *node;

        float matchWeight = (*m_matchPolicy)(node, words[offset]);

        QString varName;
        if (const Nlp::VariableNode *varNode = node->to<Nlp::VariableNode>()) {
            varName = varNode->varName;
        }
        m_searchCtx.stack().update(varName, offset);

        if (matchWeight > 0) {
            TRACE(offset) << words[offset] << "matched with weight" << matchWeight;

            m_searchCtx.stack().capture(words[offset].origWord, offset);
            m_searchCtx.score().updateScore(offset, matchWeight);

            if (offset + 1 < words.size()) {
                scoredDFS(results, node, words, offset + 1);
            } else {
                handleEndWord(results, node, offset);
            }
        }
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::Tree::handleEndWord(Nlp::ResultList &results, const Nlp::Node *node, int offset)
{
    QPair<const Nlp::Node*, int> p(node, offset);

    if (!m_loopDetector.contains(p)) {
        m_loopDetector.insert(p);

        Nlp::ResultList r = getResultsForNode(node);
        if (!r.isEmpty()) {
            results.append(r);
        } else {
           TRACE(offset) << "No valid outputs found!";
        }

        m_loopDetector.remove(p);
    } else {
        TRACE(offset) << "Infinite loop detected!";
    }
}

//--------------------------------------------------------------------------------------------------

Lvk::Nlp::ResultList Lvk::Nlp::Tree::getResultsForNode(const Nlp::Node *node)
{
    Nlp::ResultList results;
    QSet<Nlp::RuleId> ruleIds;
    Nlp::OutputMap::const_iterator it;
    float score = m_searchCtx.score().currentScore();

    // For each rule definition, try to find a valid output
    for (it = node->omap.constBegin(); it != node->omap.constEnd(); ++it) {
        Nlp::RuleId ruleId = getRuleId(it.key());
        int inputIdx = getInputIndex(it.key());

        if (ruleIds.contains(ruleId)) {
            continue;
        }

        const Nlp::CondOutputList &l = it.value();
        QString output = l.nextValidOutput(m_searchCtx.stack());

        if (output.isNull()) {
            continue;
        }

        bool ok;
        QString expOutput = expandVars(output, &ok);
        if (ok) {
            results.append(Nlp::Result(expOutput, ruleId, inputIdx, score));
        } else {
            qDebug() << "Failed to expand output" << output << ". Trying with next output";
        }

    }

    return results;
}

//--------------------------------------------------------------------------------------------------

QString Lvk::Nlp::Tree::expandVars(const QString &output, bool *ok)
{
    // TODO a possible optimization is to have all outputs already splitted

    QString newOutput;
    QString varName;
    QString varValue;
    int offset = 0;
    int i = 0;
    bool recursive = false;

    while (true) {
        i = m_parser.parseVariable(output, &varName, &recursive, offset);
        if (i != -1) {
            varValue = m_searchCtx.stack().value(varName);

            // if recursive variable
            if (recursive) {
                Nlp::Result result;
                getResponse(varValue, result);

                if (result.isValid()) {
                    varValue = result.output;
                } else {
                    newOutput.clear();
                    *ok = false;
                    break;
                }

                i--;
            }

            newOutput += output.mid(offset, i - offset) + varValue;
            offset =  i + varName.size() + (recursive ? 3 : 2);
        } else {
            newOutput += output.mid(offset);
            *ok = true;
            break;
        }
    }

    return newOutput;
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::Tree::parseRuleInput(const QString &input, Nlp::WordList &words)
{
    qDebug() << "Nlp::Tree: Parsing rule input" << input;

    words.clear();

    Nlp::GlobalTools::instance()->lemmatizer()->lemmatize(input, words);

    parseExactMatch(words);
    filterSymbols(words);

    qDebug() << "Nlp::Tree: Parsed rule input" << words;
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::Tree::parseUserInput(const QString &input, Nlp::WordList &words)
{
    qDebug() << "Nlp::Tree: Parsing user input" << input;

    words.clear();

    QString szInput = input;
    szInput.remove('\'');
    Nlp::GlobalTools::instance()->lemmatizer()->lemmatize(szInput, words);

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

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::Tree::parseExactMatch(Nlp::WordList &words)
{
    for (int i = 0; i < words.size(); ++i) {
        QString &w = words[i].origWord;
        if (w.size() >= 3 && w[0] == '\'' && w[w.size() - 1] == '\'') {
            w = w.mid(1, w.size() - 2).toLower(); // TODO check if we want to normalize to lower
            words[i].normWord = w;
            words[i].lemma = "";
            words[i].posTag = "";
        }
    }
}

