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

Lvk::RuleItem::RuleItem(const QString &name, const QList<QVariant> &input,
                        const QList<QVariant> &ouput, RuleItem *parent /*= 0*/)
    : m_name(name), m_input(input), m_ouput(ouput), m_parentItem(parent), m_type(Rule)
{
}

Lvk::RuleItem::~RuleItem()
{
    qDeleteAll(m_childItems);
}

Lvk::RuleItem * Lvk::RuleItem::child(int row)
{
    return m_childItems.value(row);
}

int Lvk::RuleItem::childCount() const
{
    return m_childItems.count();
}

int Lvk::RuleItem::row() const
{
    if (m_parentItem) {
        return m_parentItem->m_childItems.indexOf(const_cast<Lvk::RuleItem *>(this));
    }

    return 0;
}

int Lvk::RuleItem::columnCount() const
{
    return 1;
}

QVariant Lvk::RuleItem::data(int column, int role) const
{
    switch (column) {
    case 0:
        switch (role) {
        case Qt::EditRole:
        case Qt::DisplayRole:
            return m_name;

        case Qt::DecorationRole:
            return m_type == Rule ? QIcon(":/icons/rule_16x16.png")
                                  : QIcon(":/icons/category_16x16.png");
        case InputRole:
            return m_input;

        case OutputRole:
            return m_ouput;

        default:
            return QVariant();
        }

    default:
        return QVariant();
    }
}

bool Lvk::RuleItem::setData(const QVariant &value, int column, int role)
{
    switch (column) {
    case 0:
        switch (role) {
        case Qt::EditRole:
        case Qt::DisplayRole:
            m_name = value.toString();
            return true;

        case Qt::DecorationRole:
            return false;

        case InputRole:
            m_input = value.toList();
            return true;

        case OutputRole:
            m_ouput = value.toList();
            return true;

        default:
            return false;
        }

    default:
        return false;
    }
}

Lvk::RuleItem * Lvk::RuleItem::parent()
{
    return m_parentItem;
}

const Lvk::RuleItem * Lvk::RuleItem::parent() const
{
    return m_parentItem;
}

void Lvk::RuleItem::setType(Lvk::RuleItem::ItemType type)
{
    m_type = type;
}

Lvk::RuleItem::ItemType Lvk::RuleItem::type() const
{
    return m_type;
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

