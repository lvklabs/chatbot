/*
 * Copyright (C) 2012 Andres Pagliano, Gabriel Miretti, Gonzalo Buteler,
 * Nestor Bustamante, Pablo Perez de Angelis
 *
 * This file is part of LVK Chatbot.
 *
 * LVK Chatbot is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * LVK Chatbot is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LVK Chatbot.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef LVK_BE_RULE_H
#define LVK_BE_RULE_H

#include <QList>
#include <QString>
#include <QStringList>
#include <QVariant>

#include "target.h"

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace BE
{

/// \ingroup Lvk
/// \addtogroup BE
/// @{

/**
 * \brief The Rule class provides a NLP rule with metadata to display it in the front-end.
 */

class Rule
{
private:

    /**
     * \brief Generic class used to implement iterator and const_iterator classes
     */
    template <typename T>
    class generic_iterator
    {
    public:
        generic_iterator(T rule = 0) : m_rule(rule) { }

        generic_iterator<T> operator++()
        {
            // Depth-first search

            if (m_rule->childCount() > 0) {
                m_rule = m_rule->child(0);
            } else {
                do {
                    T nextSibling = m_rule->nextSibling();

                    if (nextSibling) {
                        m_rule = nextSibling;
                        break;
                    }

                    m_rule = m_rule->parent();
                } while (m_rule);
            }

            return *this;
        }

        bool operator==(const generic_iterator& it) const
        {
            return m_rule == it.m_rule;
        }

        bool operator!=(const generic_iterator& it) const
        {
            return !operator==(it);
        }

    protected:
        T m_rule;
    };


public:
    enum Type { OrdinaryRule, EvasiveRule, ContainerRule };

    enum Status { Saved, Unsaved };

    Rule(Rule *parent = 0);

    Rule(const QString &name, Rule *parent = 0);

    Rule(const QString &name, Type type, Rule *parent = 0);

    Rule(const QString &name, const QStringList &input, const QStringList &output,
         Rule *parent = 0);

    Rule(const Rule &other, bool deepCopy = false);

    ~Rule();

    //Rule& operator=(const Rule &other);

    bool operator==(const Rule &other) const;

    bool operator!=(const Rule &other) const;

    Rule *parent();

    const Rule *parent() const;

    QList<Rule*> &children();

    const QList<Rule*> &children() const;

    bool appendChild(Rule *child);

    bool insertChildren(int position, int count);

    bool removeChildren(int position, int count);

    void removeAllChild();

    bool moveChildren(int position, int count, Rule *newParent);

    bool moveAllChildren(Rule *newParent);

    int childCount() const;

    void clear();

    Rule *child(int number);

    const Rule *child(int number) const;

    Rule *nextSibling();

    const Rule *nextSibling() const;

    /**
     * Returns the rule type, by default is \a OrdinaryRule.
     *
     * \see Type
     */
    Type type() const;

    /**
     * Sets the rule type.
     *
     * \see Type
     */
    void setType(Type type);

    /**
     * Returns the rule name, by default is empty.
     *
     * \see Type
     */
    const QString &name() const;

    /**
     * Sets the rule name.
     */
    void setName(const QString &name);

    TargetList &target();

    const TargetList &target() const;

    void setTarget(const TargetList &target);

    QStringList &input();

    const QStringList &input() const;

    void setInput(const QStringList &input);

    QStringList &output();

    const QStringList &output() const;

    void setOutput(const QStringList &output);

    /**
     * Returns true if the rule is enabled. Otherwise; returns false. By default is disabled.
     */
    bool enabled() const;

    /**
     * Sets if the rule is enabled or disabled.
     */
    void setEnabled(bool enabled);

    /**
     * Returns the rule check state, by default is \a Unchecked.
     */
    Qt::CheckState checkState() const;

    /**
     * Sets the rule check state.
     */
    void setCheckState(Qt::CheckState state);

    /**
     * Returns the rule status, by default is \a Unsaved.
     *
     * \see Status
     */
    Status status() const;

    /**
     * Sets the rule status.
     *
     * \see Status
     */
    void setStatus(Status status);

    /**
     * Returns the rule ID, by default is 0.
     */
    quint64 id() const;

    /**
     * Sets the rule ID.
     */
    void setId(quint64 id);

    /**
     * \brief The iterator class provides a STL-like Rule iterator
     */
    class iterator : public generic_iterator<Rule *>
    {
    public:
        iterator(Rule *rule = 0) : generic_iterator<Rule *>(rule) { }

        Rule* operator*() { return m_rule; }
    };

    /**
     * \brief The const_iterator class provides a STL-like Rule const iterator
     */
    class const_iterator : public generic_iterator<const Rule *>
    {
    public:
        const_iterator(const Rule *rule = 0) : generic_iterator<const Rule *>(rule) { }

        const Rule* operator*() const { return m_rule; }
    };

    /**
     * Returns an iterator referring to the first element in the rules tree.
     */
    iterator begin() { return iterator(this); }

    /**
     * Returns an iterator referring to the past-the-end element in the rules tree.
     */
    iterator end() { return iterator(0); }

    /**
     * Returns a const iterator referring to the first element in the rules tree.
     */
    const_iterator begin() const { return const_iterator(this); }

    /**
     * Returns a const iterator referring to the past-the-end element in the rules tree.
     */
    const_iterator end() const { return const_iterator(0); }

private:
    //Rule(const Rule &other);
    Rule& operator=(Rule &other);

    QList<Rule*> m_childItems;
    QString m_name;
    QStringList m_input;
    QStringList m_output;
    TargetList m_target;
    Rule *m_parentItem;
    Type m_type;
    bool m_enabled;
    Status m_status;
    Qt::CheckState m_checkState;
    quint64 m_id;
};

QDataStream &operator<<(QDataStream &stream, const Rule &rule);
QDataStream &operator>>(QDataStream &stream, Rule &rule);

/// @}

} // namespace BE

/// @}

} // namespace Lvk

#endif // LVK_BE_RULE_H
