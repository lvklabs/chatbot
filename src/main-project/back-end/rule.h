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

#ifndef LVK_BE_RULE_H
#define LVK_BE_RULE_H

#include <QList>
#include <QString>
#include <QStringList>
#include <QVariant>

namespace Lvk
{

namespace BE
{

/**
 * \brief This class provides a NLP rule plus some metadata
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

    Rule(const QString &name, const QStringList &input, const QStringList &ouput,
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


    Type type() const;

    void setType(Type type);


    const QString &name() const;

    void setName(const QString &name);


    QStringList &input();

    const QStringList &input() const;

    void setInput(const QStringList &input);


    QStringList &output();

    const QStringList &output() const;

    void setOutput(const QStringList &output);


    bool enabled() const;

    void setEnabled(bool enabled);


    Qt::CheckState checkState() const;

    void setCheckState(Qt::CheckState state);


    Status status() const;

    void setStatus(Status status);

    /**
      * \brief Rule iterator
      */
    class iterator : public generic_iterator<Rule *>
    {
    public:
        iterator(Rule *rule = 0) : generic_iterator<Rule *>(rule) { }

        Rule* operator*() { return m_rule; }
    };

    /**
      * \brief Rule const iterator
      */
    class const_iterator : public generic_iterator<const Rule *>
    {
    public:
        const_iterator(const Rule *rule = 0) : generic_iterator<const Rule *>(rule) { }

        const Rule* operator*() const { return m_rule; }
    };

    iterator begin() { return iterator(this); }

    iterator end() { return iterator(0); }

    const_iterator begin() const { return const_iterator(this); }

    const_iterator end() const { return const_iterator(0); }

private:
    //Rule(const Rule &other);
    Rule& operator=(Rule &other);

    QList<Rule*> m_childItems;
    QString m_name;
    QStringList m_input;
    QStringList m_output;
    Rule *m_parentItem;
    Type m_type;
    bool m_enabled;
    Status m_status;
    Qt::CheckState m_checkState;
};

QDataStream &operator<<(QDataStream &stream, const Rule &rule);
QDataStream &operator>>(QDataStream &stream, Rule &rule);

} // namespace BE

} // namespace Lvk

#endif // LVK_BE_RULE_H
