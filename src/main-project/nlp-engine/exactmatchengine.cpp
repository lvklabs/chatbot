#include "exactmatchengine.h"
#include "nlprule.h"
#include <QStringList>

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

QString Lvk::Nlp::ExactMatchEngine::getResponse(const QString &input)
{
    RuleList matched;
    return getResponse(input, matched);
}

QString Lvk::Nlp::ExactMatchEngine::getResponse(const QString &input, RuleList &matched)
{
    for (int i = 0; i < m_rules.size(); ++i) {
        const QStringList &ruleInput = m_rules[i].input();
        const QStringList &ruleOuput = m_rules[i].output();

        for (int j = 0; j < ruleInput.size() && !ruleOuput.empty() ; ++j) {
            if (QString::compare(ruleInput[j], input, Qt::CaseInsensitive) == 0) {
                matched.append(m_rules[i]);
                return ruleOuput[0];
            }
        }
    }

    return "";
}

