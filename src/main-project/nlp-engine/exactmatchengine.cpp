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

