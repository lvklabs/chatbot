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

#include "nlp-engine/exactmatchengine.h"
#include "nlp-engine/rule.h"
#include "common/random.h"

#include <QStringList>
#include <cassert>

//--------------------------------------------------------------------------------------------------
// Helpers
//--------------------------------------------------------------------------------------------------

namespace
{

// Returns the index of value in list doing a case *insensitive* search or -1 if not found

int indexOf(const QString &value, const QStringList &list)
{
    for (int i = 0; i < list.size(); ++i) {
        if (QString::compare(value, list[i], Qt::CaseInsensitive)) {
            return i;
        }
    }

    return -1;
}

} // namespace

//--------------------------------------------------------------------------------------------------
// ExactMatchEngine
//--------------------------------------------------------------------------------------------------

Lvk::Nlp::ExactMatchEngine::ExactMatchEngine()
{
}

//--------------------------------------------------------------------------------------------------

Lvk::Nlp::ExactMatchEngine::~ExactMatchEngine()
{
}

//--------------------------------------------------------------------------------------------------

Lvk::Nlp::RuleList Lvk::Nlp::ExactMatchEngine::rules() const
{
    return m_rules;
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::ExactMatchEngine::setRules(const Lvk::Nlp::RuleList &rules)
{
    m_rules = rules;
}

//--------------------------------------------------------------------------------------------------

QString Lvk::Nlp::ExactMatchEngine::getResponse(const QString &input, MatchList &matches)
{
    return getResponse(input, "", matches);
}

//--------------------------------------------------------------------------------------------------

QString Lvk::Nlp::ExactMatchEngine::getResponse(const QString &input, const QString &target,
                                                MatchList &matches)
{
    matches.clear();

    MatchList allMatches;
    QStringList responses = getAllResponses(input, target, allMatches);

    if (!allMatches.empty()) {
        matches.append(allMatches.first());

        assert(!responses.empty());
        return responses.first();
    } else {
        return "";
    }
}

//--------------------------------------------------------------------------------------------------

QStringList Lvk::Nlp::ExactMatchEngine::getAllResponses(const QString &input, MatchList &matches)
{
    return getAllResponses(input, "", matches);
}

//--------------------------------------------------------------------------------------------------

QStringList Lvk::Nlp::ExactMatchEngine::getAllResponses(const QString &input, const QString &target,
                                                        MatchList &matches)
{
    QStringList responses;

    for (int i = 0; i < m_rules.size(); ++i) {
        const QStringList &ruleInput = m_rules[i].input();
        const QStringList &ruleOuput = m_rules[i].output();
        const QStringList &ruleTarget = m_rules[i].output();

        int inputIndex = indexOf(input, ruleInput);

        if (inputIndex != -1) {
            if (target.isEmpty() || indexOf(target, ruleTarget) != -1) {
                int randomOutput = Cmn::Random::getInt(0, ruleOuput.size() - 1);
                responses.append(ruleOuput[randomOutput]);
                matches.append(qMakePair(m_rules[i].id(), inputIndex));
            }
        }
    }

    return responses;
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::ExactMatchEngine::setPreSanitizer(Lvk::Nlp::Sanitizer */*sanitizer*/)
{
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::ExactMatchEngine::setLemmatizer(Lvk::Nlp::Lemmatizer */*lemmatizer*/)
{
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::ExactMatchEngine::setPostSanitizer(Lvk::Nlp::Sanitizer */*sanitizer*/)
{
}
