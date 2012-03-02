#include "coreapp.h"
#include "rule.h"
#include "nlpengine.h"
#include "nlprule.h"

Lvk::BE::CoreApp::CoreApp()
    : m_rootRule(0), m_nlpEngine(0)
{
}

Lvk::BE::CoreApp::~CoreApp()
{
    delete m_rootRule;
    delete m_nlpEngine;
}


bool Lvk::BE::CoreApp::load(const QString &/*filename*/)
{
    if (!m_filename.isEmpty()) {
        close();
    }

    m_rootRule = new BE::Rule();

    ////////////////////////////////////////////////////////////////////////
    // TODO read from file:

    BE::Rule *catGreetings    = new BE::Rule("Saludos");

    catGreetings->setType(BE::Rule::ContainerRule);

    m_rootRule->appendChild(catGreetings);

    QList<QString> rule1InputList;
    QList<QString> rule1OutputList;
    rule1InputList << QString("Hola") << QString("Hola *");
    rule1OutputList << QString("Hola $USERNAME");

    QList<QString> rule2InputList;
    QList<QString> rule2OutputList;
    rule2InputList << QString("Buenas") << QString("Buena dia") << QString("Buena dia");
    rule2OutputList << QString("Buen dia $USERNAME");

    BE::Rule * rule1 = new BE::Rule("", rule1InputList, rule1OutputList);
    BE::Rule * rule2 = new BE::Rule("", rule2InputList, rule2OutputList);

    catGreetings->appendChild(rule1);
    catGreetings->appendChild(rule2);

    ////////////////////////////////////////////////////////////////////////

    return true;
}

bool Lvk::BE::CoreApp::saveAs(const QString &/*filename*/)
{
    return false;
}

bool Lvk::BE::CoreApp::save()
{
    return false;
}

void Lvk::BE::CoreApp::close()
{
    delete m_rootRule;
    m_rootRule = 0;

    m_filename = "";
}

Lvk::BE::Rule * Lvk::BE::CoreApp::rootRule()
{
    return m_rootRule;
}

QString Lvk::BE::CoreApp::getResponse(const QString &input, QList<BE::Rule *> &matched)
{
    matched.clear();

    if (m_nlpEngine) {
        return m_nlpEngine->getResponse(input);
    } else {
        return "Not implemented";
    }
}

void Lvk::BE::CoreApp::refreshNlpEngine()
{
    if (m_nlpEngine && m_rootRule) {
        Nlp::RuleList nlpRules;
        buildNlpRulesOf(m_rootRule, nlpRules);
        m_nlpEngine->setRules(nlpRules);
    }
}

void Lvk::BE::CoreApp::buildNlpRulesOf(const BE::Rule *parentRule, Nlp::RuleList &nlpRules)
{
    if (!parentRule) {
        return;
    }

    for (int i = 0; i < parentRule->childCount(); ++i) {
        const BE::Rule *child = parentRule->child(i);

        if (child->type() == Rule::FinalRule) {
            Nlp::Rule nlpRule(0, child->input(), child->output());
            nlpRules.append(nlpRule);
        } else if (child->type() == Rule::ContainerRule) {
            buildNlpRulesOf(child, nlpRules);
        }
    }
}


