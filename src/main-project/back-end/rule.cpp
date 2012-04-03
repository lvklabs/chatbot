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

#include "rule.h"

#include <QtAlgorithms>
#include <QIcon>
#include <assert.h>

Lvk::BE::Rule::Rule(Rule *parent /*= 0*/)
    : m_name(""), m_input(), m_ouput(), m_parentItem(parent), m_type(OrdinaryRule),
      m_enabled(false), m_status(Unsaved)
{
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::Rule::Rule(const QString &name, Rule *parent /*= 0*/)
    : m_name(name), m_input(), m_ouput(), m_parentItem(parent), m_type(OrdinaryRule),
      m_enabled(false), m_status(Unsaved)
{
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::Rule::Rule(const QString &name, Type type, Rule *parent /*= 0*/)
    : m_name(name), m_input(), m_ouput(), m_parentItem(parent), m_type(type),
      m_enabled(false), m_status(Unsaved)
{
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::Rule::Rule(const QString &name, const QList<QString> &input,
                        const QList<QString> &ouput, Rule *parent /*= 0*/)
    : m_name(name), m_input(input), m_ouput(ouput), m_parentItem(parent), m_type(OrdinaryRule),
      m_enabled(false), m_status(Unsaved)
{
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::Rule::~Rule()
{
    qDeleteAll(m_childItems);
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::Rule * Lvk::BE::Rule::parent()
{
    return m_parentItem;
}

//--------------------------------------------------------------------------------------------------

const Lvk::BE::Rule * Lvk::BE::Rule::parent() const
{
    return m_parentItem;
}

//--------------------------------------------------------------------------------------------------

QList<Lvk::BE::Rule *> & Lvk::BE::Rule::children()
{
    return m_childItems;
}

//--------------------------------------------------------------------------------------------------

const QList<Lvk::BE::Rule *> & Lvk::BE::Rule::children() const
{
    return m_childItems;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::Rule::appendChild(Rule *item)
{
    assert(item->m_parentItem == 0 || item->m_parentItem == this);

    item->m_parentItem = this;

    m_childItems.append(item);

    m_status = Unsaved;

    return true;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::Rule::insertChildren(int position, int count)
{
    if (position < 0 || position > m_childItems.size()) {
        return false;
    }

    for (int i = 0; i < count; ++i) {
        Rule *rule = new Rule(this);
        m_childItems.insert(position, rule);
    }

    m_status = Unsaved;

    return true;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::Rule::removeChildren(int position, int count)
{
    if (position < 0 || position + count > m_childItems.size()) {
        return false;
    }

    for (int i = 0; i < count; ++i) {
        delete m_childItems.takeAt(position);
    }

    m_status = Unsaved;

    return true;
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::Rule::removeAllChild()
{
    removeChildren(0, childCount());
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::Rule * Lvk::BE::Rule::child(int row)
{
    return m_childItems.value(row);
}

//--------------------------------------------------------------------------------------------------

const Lvk::BE::Rule * Lvk::BE::Rule::child(int row) const
{
    return m_childItems.value(row);
}

//--------------------------------------------------------------------------------------------------

int Lvk::BE::Rule::childCount() const
{
    return m_childItems.count();
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::Rule * Lvk::BE::Rule::nextSibling()
{
    if (m_parentItem != 0) {
        // TODO consider not using indexOf to improve performance. Instead store the sibling number
        //      as a class atribute
        int i = m_parentItem->m_childItems.indexOf(const_cast<Rule *>(this));

        if (i + 1 <  m_parentItem->m_childItems.size()) {
            return m_parentItem->m_childItems[i + 1];
        }
    }
    return 0;
}

//--------------------------------------------------------------------------------------------------

const Lvk::BE::Rule * Lvk::BE::Rule::nextSibling() const
{
    if (m_parentItem != 0) {
        // TODO consider not using indexOf to improve performance. Instead store the sibling number
        //      as a class atribute
        int i = m_parentItem->m_childItems.indexOf(const_cast<Rule *>(this));

        if (i + 1 <  m_parentItem->m_childItems.size()) {
            return m_childItems[i + 1];
        }
    }
    return 0;
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::Rule::setType(Rule::Type type)
{
    if (m_type != type) {
        m_type = type;
        m_status = Unsaved;
    }
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::Rule::Type Lvk::BE::Rule::type() const
{
    return m_type;
}

//--------------------------------------------------------------------------------------------------

QList<QString> &Lvk::BE::Rule::input()
{
    m_status = Unsaved;

    return m_input;
}

const QList<QString> &Lvk::BE::Rule::input() const
{
    return m_input;
}

void Lvk::BE::Rule::setInput(const QList<QString> &input)
{
    m_status = Unsaved;

    m_input = input;
}

//--------------------------------------------------------------------------------------------------

QList<QString> &Lvk::BE::Rule::output()
{
    m_status = Unsaved;

    return m_ouput;
}

//--------------------------------------------------------------------------------------------------

const QList<QString> &Lvk::BE::Rule::output() const
{
    return m_ouput;
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::Rule::setOutput(const QList<QString> &output)
{
    m_status = Unsaved;

    m_ouput = output;
}

//--------------------------------------------------------------------------------------------------

const QString &Lvk::BE::Rule::name() const
{
    return m_name;
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::Rule::setName(const QString &name)
{
    if (m_name != name) {
        m_name = name;

        m_status = Unsaved;
    }
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

    return stream;
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
    rule.setType(static_cast<Rule::Type>(type));

    for (int i = 0; i < childCount; ++i) {
        Rule *child = new Rule();
        stream >> *child;
        rule.appendChild(child);
    }

    return stream;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::Rule::enabled() const
{
    return m_enabled;
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::Rule::setEnabled(bool enabled)
{
    if (m_enabled != enabled) {
        m_enabled = enabled;
        m_status = Unsaved;
    }
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::Rule::Status Lvk::BE::Rule::status() const
{
    return m_status;
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::Rule::setStatus(Status status)
{
    m_status = status;
}




