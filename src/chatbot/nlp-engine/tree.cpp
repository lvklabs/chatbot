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
#include "nlp-engine/globaltools.h"
#include "nlp-engine/matchpolicy.h"
#include "nlp-engine/scoringalgorithm.h"

#define STAR_OP "*"
#define PLUS_OP "+"

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

    foreach (QString input, rule.input()) {
        Nlp::GlobalTools::instance()->lemmatizer()->lemmatize(input, words);

        filterPunct(words);

        if (words.isEmpty()) {
            continue;
        }

        Nlp::Node *curNode = m_root;

        foreach (Nlp::Word w, words) {
            curNode = addNode(w, curNode);
        }

        setNodeOutput(curNode, rule.output());

        // FIXME Workaround
        if (words.last().normWord == STAR_OP && curNode->parent != m_root) {
            setNodeOutput(curNode->parent, rule.output());
        }
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::Tree::setNodeOutput(Nlp::Node *node, const QStringList &output)
{
    for (int i = 0; i < output.size(); ++i) {
        // TODO set conditionals
        node->outputs.append(Nlp::CondOutput(output[i], i));
    }
}

//--------------------------------------------------------------------------------------------------

Lvk::Nlp::Node * Lvk::Nlp::Tree::addNode(const Nlp::Word &w, Nlp::Node *parent)
{
    // TODO
    // if (w in parent.childs)
    //      return child
    // else

    if (w.origWord == STAR_OP) {
        Nlp::Node *newNode = new Nlp::WildcardNode(parent);
        parent->childs.append(newNode);
        newNode->childs.append(newNode); // Loop node
        return newNode;
    } else if (w.origWord == PLUS_OP) {
//        Nlp::Node *newNode = new Nlp::WildcardNode(parent);
//        parent->childs.append(newNode);
//        Nlp::Node *newNode2 = new Nlp::WildcardNode(newNode);
//        newNode->childs.append(newNode2);
//        newNode2->childs.append(newNode2); // Loop node
//        return newNode2;
        Nlp::Node *newNode = new Nlp::WildcardNode(parent);
        parent->childs.append(newNode);
        newNode->childs.append(newNode); // Loop node
        return newNode;
    // } else if (variable) {
        // TODO
    } else {
        Nlp::Node *newNode = new Nlp::WordNode(w, parent);
        parent->childs.append(newNode);
        return newNode;
    }
}

//--------------------------------------------------------------------------------------------------

QStringList Lvk::Nlp::Tree::getResponses(const QString &input, Engine::MatchList &matches)
{
    matches.clear();

    Nlp::WordList words;
    Nlp::GlobalTools::instance()->lemmatizer()->lemmatize(input, words);

    filterPunct(words);

    ResultList results;
    scoredDFS(results, m_root, words);

    // TODO sort results by score

    QStringList responses;

    foreach (const Result &r, results) {
        responses.append(r.output);
        matches.append(Engine::RuleMatch(r.ruleId, r.inputIndex));
    }

    return responses;
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::Tree::scoredDFS(ResultList &r, const Nlp::Node *root, const Nlp::WordList &words,
                               int offset /*= 0*/)
{
    foreach (const Nlp::Node *node, root->childs) {
//        qDebug() << QString((offset+1)*4, '#') << "Current node"
//                 << (dynamic_cast<const Nlp::WordNode*>(node) ?
//                         dynamic_cast<const Nlp::WordNode*>(node)->word.origWord : "*");

        float matchWeight = (*m_matchPolicy)(node, words[offset]);
        if (matchWeight > 0) {

//            qDebug() << QString((offset+1)*4, '#') << words[offset] << "match with w" << matchWeight;

            m_scoringAlg->updateScore(offset, matchWeight);
            if (offset + 1 < words.size()) {
                scoredDFS(r, node, words, offset + 1);
            } else {
                QString output = getOutput(node);
                if (!output.isEmpty()) {
                    // TODO add rule id and input index
                    r.append(Result(output, 0, 0, m_scoringAlg->currentScore()));
//                } else {
//                    qDebug() << QString((offset+1)*4, '#') << "No output found!";
                }
            }
        }
    }
}

//--------------------------------------------------------------------------------------------------

QString Lvk::Nlp::Tree::getOutput(const Nlp::Node *node)
{
    QString output;

    foreach (const Nlp::CondOutput &co, node->outputs) {
        if (!co.output.isEmpty() && co.predicate()) {
            output = co.output;
            // TODO expand variables
            break;
        }
    }
    return output;
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::Tree::filterPunct(Nlp::WordList &words)
{
    qDebug() << "pre" << words;
    for (int i = 0; i < words.size(); ++i) {
        const QString &w = words[i].origWord;
        // TODO check .isPunct(). Consider !.isAlphanum()
        if (w.size() == 1 && w[0].isPunct() && w != STAR_OP && w != PLUS_OP) {
            words.removeAt(i);
        }
    }
    qDebug() << "post" << words;
}
