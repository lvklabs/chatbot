#include "coreapp.h"
#include "rule.h"

Lvk::BE::CoreApp::CoreApp()
    : m_rootRule(0)
{
}

Lvk::BE::CoreApp::~CoreApp()
{
    delete m_rootRule;
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

QString Lvk::BE::CoreApp::getResponse(const QString &/*input*/, QList<Lvk::BE::Rule> &matched)
{
    matched.clear();

    return "Not implemented";
}
