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

#include "back-end/target.h"

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
 * \brief The Rule class provides a NLP rule with metadata.
 *
 * The Rule class is the main data structure. It provides standard NLP
 * attributes such as input, output and target plus metadata like name, rule type, rule status,
 * enabled flag, check state, etc. There are three types of rules. See Rule::Type.
 *
 * Rules can be nested in a tree-way fashion. Rules can be nested by passing a parent in the
 * constructor or by adding children with appendChild() or insertChildren().
 *
 * Given a root rule it can be iterated using STL-like iterator classes Rule::iterator and
 * Rule::const_iterator
 */
class Rule
{
private:

    /**
     * \brief Generic class used to implement Rule::iterator and Rule::const_iterator classes
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

    /**
     * Rule type
     */
    enum Type {
        OrdinaryRule,   ///< The rule is a ordinary input/output rule
        EvasiveRule,    ///< The rule is a evasives rule which does not contain any input
        ContainerRule   /** The rule is a container rule which does not contain any input/output
                            but contains ordinary rules */
    };

    /**
     * Constructs an empty rule
     */
    Rule();

    /**
     * Constructs a new rule with the given \a name
     */
    Rule(const QString &name);

    /**
     * Constructs a new rule with the given \a name and \a type
     */
    Rule(const QString &name, Type type);

    /**
     * Constructs a new rule with the given \a name, \a input and \a output
     */
    Rule(const QString &name, const QStringList &input, const QStringList &output);

    /**
     * Constructs a new rule with the given \a name, \a type, \a input and \a output
     */
    Rule(const QString &name, Type type, const QStringList &input, const QStringList &output);

    /**
     * Constructs a new rule from \a other. If \a deepCopy is false, children are not copied.
     * Otherwise; all children are copied recursively.
     */
    Rule(const Rule &other, bool deepCopy = false);

    /**
     * Destroys the object.
     */
    ~Rule();



    //Rule& operator=(const Rule &other);

    /**
     * Returns true if other is equal to this rule; otherwise returns false.
     */
    bool operator==(const Rule &other) const;

    /**
     * Returns true if other is different to this rule; otherwise returns false.
     */
    bool operator!=(const Rule &other) const;



    /**
     * Returns the rule's parent as non-const pointer. If has no parent, it returns 0.
     */
    Rule *parent();

    /**
     * Returns the rule's parent as const pointer. If has no parent, it returns 0.
     */
    const Rule *parent() const;

    /**
     * Returns a reference to the list of children.
     */
    QList<Rule*> &children();

    /**
     * Returns a const reference to the list of children.
     */
    const QList<Rule*> &children() const;

    /**
     * Appends \a child.
     */
    bool appendChild(Rule *child);

    /**
     * Inserts \a count children at \a position
     */
    bool insertChildren(int position, int count);

    /**
     * Removes \a count children starting from \a position
     */
    bool removeChildren(int position, int count);

    /**
     * Removes all child.
     */
    void removeAllChild();

    /**
     * Moves \a count children starting from \a position to a new parent \a newParent
     */
    bool moveChildren(int position, int count, Rule *newParent);

    /**
     * Moves all children to a new parent \a newParent
     */
    bool moveAllChildren(Rule *newParent);

    /**
     * Returns the number of children.
     */
    int childCount() const;

    /**
     * Returns the child rule at index position \a n as a non-const pointer. \a n must be a
     * valid index position in the list (i.e., 0 <= i < childCount()).
     */
    Rule *child(int n);

    /**
     * Returns the child rule at index position \a n as a const pointer. \a n must be a
     * valid index position in the list (i.e., 0 <= i < childCount()).
     */
    const Rule *child(int n) const;

    /**
     * Returns the rule's next sibling as a non-const pointer. If there is no next
     * sibling, it return 0.
     */
    Rule *nextSibling();

    /**
     * Returns the rule's next sibling as const pointer. If there is no next sibling, it returns 0.
     */
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

    /**
     * Returns a reference to the list of targets in the rule.
     */
    TargetList &target();

    /**
     * Returns a const reference to the list of targets in the rule.
     */
    const TargetList &target() const;

    /**
     * Sets the list of targets.
     */
    void setTarget(const TargetList &target);

    /**
     * Returns a reference to the list of input strings in the rule.
     */
    QStringList &input();

    /**
     * Returns a const reference to the list of input strings in the rule.
     */
    const QStringList &input() const;

    /**
     * Sets the list of input strings.
     */
    void setInput(const QStringList &input);

    /**
     * Returns a reference to the list of output strings in the rule.
     */
    QStringList &output();

    /**
     * Returns a const reference to the list of output strings in the rule.
     */
    const QStringList &output() const;

    /**
     * Sets the list of output strings.
     */
    void setOutput(const QStringList &output);


    /**
     * Returns the next category ID of the rule
     */
    quint64 nextCategory() const;

    /**
     * Sets the next topic of the rule
     */
    void setNextCategory(quint64 catId);


    /**
     * Returns true if output, input, target and name are empty. Otherwise; returns false.
     */
    bool isEmpty() const;

    /**
     * Returns true if:
     * <ul>
     * <li>The rule is OridnaryRule and contains at least one input and output</li>
     * <li>The rule is ContainerRule and contains a name</li>
     * <li>The rule is EvasiveRule and contains at least one output</li>
     * </ul>
     * Otherwise; returns false.
     */
    bool isComplete() const;

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
     * Rule status
     */
    enum Status {
        Saved,      ///< The rule is saved
        Unsaved     ///< The rule is unsaved
    };

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
     * Clears the rule by setting all attribures to the default value.
     */
    void clear();



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
    quint64 m_nextCatId;
};

/**
 * Writes a \a rule to the stream and returns a reference to the stream.
 */
QDataStream &operator<<(QDataStream &stream, const Rule &rule);

/**
 * Reads a rule from the \a stream into \a rule, and returns a reference to the stream.
 */
QDataStream &operator>>(QDataStream &stream, Rule &rule);

/// @}

} // namespace BE

/// @}

} // namespace Lvk

#endif // LVK_BE_RULE_H
