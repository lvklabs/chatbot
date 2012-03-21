#include "rule.h"

#include <QtAlgorithms>
#include <QIcon>
#include <assert.h>

Lvk::BE::Rule::Rule(Rule *parent /*= 0*/)
    : m_name(""), m_input(), m_ouput(), m_parentItem(parent), m_type(OrdinaryRule)
{
}

Lvk::BE::Rule::Rule(const QString &name, Rule *parent /*= 0*/)
    : m_name(name), m_input(), m_ouput(), m_parentItem(parent), m_type(OrdinaryRule)
{
}

Lvk::BE::Rule::Rule(const QString &name, RuleType type, Rule *parent /*= 0*/)
    : m_name(name), m_input(), m_ouput(), m_parentItem(parent), m_type(type)
{
}

Lvk::BE::Rule::Rule(const QString &name, const QList<QString> &input,
                        const QList<QString> &ouput, Rule *parent /*= 0*/)
    : m_name(name), m_input(input), m_ouput(ouput), m_parentItem(parent), m_type(OrdinaryRule)
{
}

Lvk::BE::Rule::~Rule()
{
    qDeleteAll(m_childItems);
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::Rule * Lvk::BE::Rule::parent()
{
    return m_parentItem;
}

const Lvk::BE::Rule * Lvk::BE::Rule::parent() const
{
    return m_parentItem;
}

//--------------------------------------------------------------------------------------------------

QList<Lvk::BE::Rule *> & Lvk::BE::Rule::children()
{
    return m_childItems;
}

const QList<Lvk::BE::Rule *> & Lvk::BE::Rule::children() const
{
    return m_childItems;
}

bool Lvk::BE::Rule::appendChild(Rule *item)
{
    assert(item->m_parentItem == 0 || item->m_parentItem == this);

    item->m_parentItem = this;

    m_childItems.append(item);

    return true;
}

bool Lvk::BE::Rule::insertChildren(int position, int count)
{
    if (position < 0 || position > m_childItems.size()) {
        return false;
    }

    for (int i = 0; i < count; ++i) {
        Rule *rule = new Rule(this);
        m_childItems.insert(position, rule);
    }

    return true;
}

bool Lvk::BE::Rule::removeChildren(int position, int count)
{
    if (position < 0 || position + count > m_childItems.size()) {
        return false;
    }

    for (int i = 0; i < count; ++i) {
        delete m_childItems.takeAt(position);
    }

    return true;
}

void Lvk::BE::Rule::removeAllChild()
{
    removeChildren(0, childCount());
}

Lvk::BE::Rule * Lvk::BE::Rule::child(int row)
{
    return m_childItems.value(row);
}

const Lvk::BE::Rule * Lvk::BE::Rule::child(int row) const
{
    return m_childItems.value(row);
}

int Lvk::BE::Rule::childCount() const
{
    return m_childItems.count();
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::Rule::setType(Rule::RuleType type)
{
    m_type = type;
}

Lvk::BE::Rule::RuleType Lvk::BE::Rule::type() const
{
    return m_type;
}

//--------------------------------------------------------------------------------------------------

QList<QString> &Lvk::BE::Rule::input()
{
    return m_input;
}

const QList<QString> &Lvk::BE::Rule::input() const
{
    return m_input;
}

void Lvk::BE::Rule::setInput(const QList<QString> &input)
{
    m_input = input;
}

//--------------------------------------------------------------------------------------------------

QList<QString> &Lvk::BE::Rule::output()
{
    return m_ouput;
}

const QList<QString> &Lvk::BE::Rule::output() const
{
    return m_ouput;
}

void Lvk::BE::Rule::setOutput(const QList<QString> &output)
{
    m_ouput = output;
}

//--------------------------------------------------------------------------------------------------

QString &Lvk::BE::Rule::name()
{
    return m_name;
}

const QString &Lvk::BE::Rule::name() const

{
    return m_name;
}

void Lvk::BE::Rule::setName(const QString &name)
{
    m_name = name;
}

//--------------------------------------------------------------------------------------------------


QDataStream &Lvk::BE::operator<<(QDataStream &stream, const Rule &rule)
{
    stream << rule.name();
    stream << rule.input();
    stream << rule.output();
    stream << static_cast<int>(rule.type());
    stream << rule.childCount();

    for (int i = 0; i < rule.children().size(); ++i) {
        stream << *(rule.children()[i]);
    }
}

//--------------------------------------------------------------------------------------------------

QDataStream &Lvk::BE::operator>>(QDataStream &stream, Rule &rule)
{
    QString name;
    QStringList input;
    QStringList output;
    int type;
    int childCount;

    stream >> name;
    stream >> input;
    stream >> output;
    stream >> type;
    stream >> childCount;

    rule.setName(name);
    rule.setInput(input);
    rule.setOutput(output);
    rule.setType(static_cast<Rule::RuleType>(type));

    for (int i = 0; i < childCount; ++i) {
        Rule *child = new Rule();
        stream >> *child;
        rule.appendChild(child);
    }
}





