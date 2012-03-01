#include "ruleitem.h"

#include <QtAlgorithms>
#include <QIcon>
#include <assert.h>

Lvk::RuleItem::RuleItem(RuleItem *parent /*= 0*/)
    : m_name(""), m_input(), m_ouput(), m_parentItem(parent), m_type(Rule)
{
}

Lvk::RuleItem::RuleItem(const QString &name, RuleItem *parent /*= 0*/)
    : m_name(name), m_input(), m_ouput(), m_parentItem(parent), m_type(Rule)
{
}

Lvk::RuleItem::RuleItem(const QString &name, const QList<QString> &input,
                        const QList<QString> &ouput, RuleItem *parent /*= 0*/)
    : m_name(name), m_input(input), m_ouput(ouput), m_parentItem(parent), m_type(Rule)
{
}

Lvk::RuleItem::~RuleItem()
{
    qDeleteAll(m_childItems);
}

//--------------------------------------------------------------------------------------------------

Lvk::RuleItem * Lvk::RuleItem::parent()
{
    return m_parentItem;
}

const Lvk::RuleItem * Lvk::RuleItem::parent() const
{
    return m_parentItem;
}

//--------------------------------------------------------------------------------------------------

QList<Lvk::RuleItem *> & Lvk::RuleItem::children()
{
    return m_childItems;
}

const QList<Lvk::RuleItem *> & Lvk::RuleItem::children() const
{
    return m_childItems;
}

bool Lvk::RuleItem::appendChild(Lvk::RuleItem *item)
{
    assert(item->m_parentItem == 0 || item->m_parentItem == this);

    item->m_parentItem = this;

    m_childItems.append(item);

    return true;
}

bool Lvk::RuleItem::insertChildren(int position, int count)
{
    if (position < 0 || position > m_childItems.size()) {
        return false;
    }

    for (int i = 0; i < count; ++i) {
        Lvk::RuleItem *rule = new Lvk::RuleItem(this);
        m_childItems.insert(position, rule);
    }

    return true;
}

bool Lvk::RuleItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > m_childItems.size()) {
        return false;
    }

    for (int i = 0; i < count; ++i) {
        delete m_childItems.takeAt(position);
    }

    return true;
}

Lvk::RuleItem * Lvk::RuleItem::child(int row)
{
    return m_childItems.value(row);
}

int Lvk::RuleItem::childCount() const
{
    return m_childItems.count();
}

//--------------------------------------------------------------------------------------------------

void Lvk::RuleItem::setType(Lvk::RuleItem::ItemType type)
{
    m_type = type;
}

Lvk::RuleItem::ItemType Lvk::RuleItem::type() const
{
    return m_type;
}

//--------------------------------------------------------------------------------------------------

QList<QString> &Lvk::RuleItem::input()
{
    return m_input;
}

const QList<QString> &Lvk::RuleItem::input() const
{
    return m_input;
}

void Lvk::RuleItem::setInput(const QList<QString> &input)
{
    m_input = input;
}

//--------------------------------------------------------------------------------------------------

QList<QString> &Lvk::RuleItem::output()
{
    return m_ouput;
}

const QList<QString> &Lvk::RuleItem::output() const
{
    return m_ouput;
}

void Lvk::RuleItem::setOutput(const QList<QString> &output)
{
    m_ouput = output;
}

//--------------------------------------------------------------------------------------------------

QString &Lvk::RuleItem::name()
{
    return m_name;
}

const QString &Lvk::RuleItem::name() const

{
    return m_name;
}

void Lvk::RuleItem::setName(const QString &name)
{
    m_name = name;
}



