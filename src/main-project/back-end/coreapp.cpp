#include "coreapp.h"
#include "rule.h"
#include "nlpengine.h"
#include "nlprule.h"
#include "exactmatchengine.h"
#include "random.h"

#include <QObject>


//--------------------------------------------------------------------------------------------------
// Lvk::BE::CoreApp
//--------------------------------------------------------------------------------------------------

Lvk::BE::CoreApp::CoreApp()
    : m_rootRule(0), m_nlpEngine(new Lvk::Nlp::ExactMatchEngine()), m_nextRuleId(0)
{
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::CoreApp::~CoreApp()
{
    delete m_rootRule;
    delete m_nlpEngine;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::CoreApp::load(const QString &filename)
{
    if (!m_filename.isEmpty()) {
        close();
    }

    m_filename = filename;
    m_rootRule = new BE::Rule();

    ////////////////////////////////////////////////////////////////////////
    // TODO read from file:

    BE::Rule *catGreetings    = new BE::Rule("Saludos");
    BE::Rule *catPersonalInfo = new BE::Rule("Informacion Personal");

    catGreetings->setType(BE::Rule::ContainerRule);
    catPersonalInfo->setType(BE::Rule::ContainerRule);

    m_rootRule->appendChild(catGreetings);
    m_rootRule->appendChild(catPersonalInfo);

    QStringList rule1InputList;
    QStringList rule1OutputList;
    rule1InputList << QString("Hola") << QString("Holaa") << QString("Holaaa");
    rule1OutputList << QString("Hola!");

    QStringList rule2InputList;
    QStringList rule2OutputList;
    rule2InputList << QString("Buenas") << QString("Buen dia") << QString("Buenas tardes");
    rule2OutputList << QString("Buenas, Como estas?");

    QStringList rule3InputList;
    QStringList rule3OutputList;
    rule3InputList << QString("Cual es tu nombre?") << QString("Como te llamas?") ;
    rule3OutputList << QString("Buenas, Como estas?");

    BE::Rule * rule1 = new BE::Rule("", rule1InputList, rule1OutputList);
    BE::Rule * rule2 = new BE::Rule("", rule2InputList, rule2OutputList);
    BE::Rule * rule3 = new BE::Rule("", rule3InputList, rule3OutputList);

    catGreetings->appendChild(rule1);
    catGreetings->appendChild(rule2);
    catPersonalInfo->appendChild(rule3);

    // evasives

    BE::Rule *evasives    = new BE::Rule("Evasivas");
    evasives->setType(BE::Rule::EvasiveRule);

    m_rootRule->appendChild(evasives);

    QStringList evasivesOutputList;
    evasivesOutputList << QString("Perdon no entiendo eso")
                       << QString("No entiendo, puedes explicarlo de otra manera?")
                       << QString("No se que quieres decir con eso");

    evasives->setOutput(evasivesOutputList);

    ////////////////////////////////////////////////////////////////////////

    refreshNlpEngine();

    return true;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::CoreApp::saveAs(const QString &/*filename*/)
{
    // TODO

    return false;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::CoreApp::save()
{
    // TODO

    return false;
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::CoreApp::close()
{
    m_nlpEngine->setRules(Nlp::RuleList());

    delete m_rootRule;
    m_rootRule = 0;

    m_filename = "";

    m_nextRuleId = 0;
    m_rulesHash.clear();

    m_evasives.clear();
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::Rule * Lvk::BE::CoreApp::rootRule()
{
    return m_rootRule;
}

//--------------------------------------------------------------------------------------------------

QString Lvk::BE::CoreApp::getResponse(const QString &input, MatchList  &matches)
{
    matches.clear();

    if (!m_nlpEngine) {
        return "ERROR: NLP Engine Not implemented";
    }

    Nlp::Engine::MatchList nlpRulesMatched;
    QString response = m_nlpEngine->getResponse(input, nlpRulesMatched);

    if (!nlpRulesMatched.isEmpty()) {
        Nlp::RuleId ruleId = nlpRulesMatched.first().first;
        int inputNumber = nlpRulesMatched.first().second;
        matches.append(qMakePair(m_rulesHash[ruleId], inputNumber));

        return response;
    } else if (!m_evasives.isEmpty()) {
        return m_evasives[Common::Random::getInt(0, m_evasives.size() - 1)];
    } else {
        return QObject::tr("Sorry, I don't understand that");
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::CoreApp::refreshNlpEngine()
{
    m_nextRuleId = 0;
    m_rulesHash.clear();
    m_evasives.clear();

    if (m_nlpEngine) {
        Nlp::RuleList nlpRules;

        if (m_rootRule) {
            buildNlpRulesOf(m_rootRule, nlpRules);
        }

        m_nlpEngine->setRules(nlpRules);
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::CoreApp::buildNlpRulesOf(BE::Rule *parentRule, Nlp::RuleList &nlpRules)
{
    if (!parentRule) {
        return;
    }

    for (int i = 0; i < parentRule->childCount(); ++i) {
        BE::Rule *child = parentRule->child(i);
        if (child->type() == Rule::OrdinaryRule) {
            m_rulesHash[m_nextRuleId] = child;
            Nlp::Rule nlpRule(m_nextRuleId++, child->input(), child->output());
            nlpRules.append(nlpRule);
        } else if (child->type() == Rule::EvasiveRule) {
            m_evasives = child->output(); // Design desicion: It can exist only one evasive rule
        } else if (child->type() == BE::Rule::ContainerRule) {
            buildNlpRulesOf(child, nlpRules);
        }
    }
}


