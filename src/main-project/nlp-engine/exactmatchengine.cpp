/*
 * Copyright (C) 2012 Andres Pagliano, Gabriel Miretti, Gonzalo Buteler,
 * Nestor Bustamante, Pablo Perez de Angelis
 *
 * This file is part of LVK Botmaster.
 *
 * LVK Botmaster is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * LVK Botmaster is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LVK Botmaster.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "exactmatchengine.h"
#include "nlprule.h"
#include "random.h"

#include <QStringList>
#include <cassert>

Lvk::Nlp::ExactMatchEngine::ExactMatchEngine()
{
}

Lvk::Nlp::ExactMatchEngine::~ExactMatchEngine()
{
}

const Lvk::Nlp::RuleList & Lvk::Nlp::ExactMatchEngine::rules() const
{
    return m_rules;
}

Lvk::Nlp::RuleList & Lvk::Nlp::ExactMatchEngine::rules()
{
    return m_rules;
}

void Lvk::Nlp::ExactMatchEngine::setRules(const Lvk::Nlp::RuleList &rules)
{
    m_rules = rules;
}

QString Lvk::Nlp::ExactMatchEngine::getResponse(const QString &input, MatchList &matches)
{
    matches.clear();

    MatchList allMatches;
    QList<QString> responses = getAllResponses(input, allMatches);

    if (!allMatches.empty()) {
        matches.append(allMatches.first());

        assert(!responses.empty());
        return responses.first();
    } else {
        return "";
    }
}

QList<QString> Lvk::Nlp::ExactMatchEngine::getAllResponses(const QString &input, MatchList &matches)
{
    QList<QString> responses;

    for (int i = 0; i < m_rules.size(); ++i) {
        const QStringList &ruleInput = m_rules[i].input();
        const QStringList &ruleOuput = m_rules[i].output();

        for (int j = 0; j < ruleInput.size() && !ruleOuput.empty() ; ++j) {
            if (QString::compare(ruleInput[j], input, Qt::CaseInsensitive) == 0) {
                int randomOutput = Common::Random::getInt(0, ruleOuput.size() - 1);
                responses.append(ruleOuput[randomOutput]);

                matches.append(qMakePair(m_rules[i].id(), j));
            }
        }
    }

    return responses;
}

