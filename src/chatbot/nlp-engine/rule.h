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

#ifndef LVK_NLP_RULE_H
#define LVK_NLP_RULE_H

#include <QList>
#include <QString>
#include <QStringList>

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace Nlp
{

/// \ingroup Lvk
/// \addtogroup Nlp
/// @{

/**
 * \brief RuleId provides a unique identifier for NLP rules
 */
typedef quint64 RuleId;


/**
 * \brief This class provides a NLP rule
 *
 * A NLP rule is simply a structure that contains a list of input strings and a list of output
 * strings. How these input and output strings are interpreted depends on the engine
 * implementation.
 * Optionally, it can contain a list of targets to which the rule is intended to apply.
 *
 * @see ExactMatchEngine, AimlEngine, HybridEngine
 */
class Rule
{
public:

    /**
     * Constructs a NLP rule with \a id
     */
    Rule(RuleId id = 0) : m_id(id), m_random(false) {}

    /**
     * Constructs a NLP rule with \a id, \a input and \a output
     */
    Rule(RuleId id, const QStringList &input, const QStringList &output)
        : m_id(id), m_input(input), m_output(output), m_random(false) {}

    /**
     * Constructs a NLP rule with \a id, \a input,\a output and \a target
     */
    Rule(RuleId id, const QStringList &input, const QStringList &output, const QStringList &target)
        : m_id(id), m_input(input), m_output(output), m_target(target), m_random(false) {}

    /**
     * Returns the rule ID, by default 0.
     */
    RuleId id() const { return m_id; }

    /**
     * Sets the rule ID.
     */
    void setId(RuleId id) { m_id = id; }

    /**
     * Returns a const reference to the list of input strings in the rule.
     */
    const QStringList &input() const { return m_input; }

    /**
     * Returns a reference to the list of input strings in the rule.
     */
    QStringList &input() { return m_input; }

    /**
     * Sets the list of input strings.
     */
    void setInput(const QStringList &input) { m_input = input; }

    /**
     * Returns a const reference to the list of output strings in the rule.
     */
    const QStringList &output() const { return m_output; }

    /**
     * Returns a reference to the list of output strings in the rule.
     */
    QStringList &output() { return m_output; }

    /**
     * Sets the list of output strings.
     */
    void setOuput(const QStringList &output) { m_output = output; }

    /**
     * Returns a const reference to the list of targets in the rule.
     */
    const QStringList &target() const { return m_target; }

    /**
     * Returns a reference to the list of targets in the rule.
     */
    QStringList &target() { return m_target; }

    /**
     * Sets the list of targets.
     */
    void setTarget(const QStringList &target) { m_target = target; }

    /**
     * Returns a const reference to the topic of the rule
     */
    const QString &topic() const { return m_topic; }

    /**
     * Returns a reference to the topic of the rule
     */
    QString &topic() { return m_topic; }

    /**
     * Sets the topic of the rule
     */
    void setTopic(const QString &topic) { m_topic = topic; }

    /**
     * Returns a const reference to the next topic of the rule
     */
    const QString &nextTopic() const { return m_nextTopic; }

    /**
     * Returns a reference to the next topic of the rule
     */
    QString &nextTopic() { return m_nextTopic; }

    /**
     * Sets the next topic of the rule
     */
    void setNextTopic(const QString &topic) { m_nextTopic = topic; }

    /**
     * Returns true if the output should be selected randomly; otherwise returns false.
     * By default is sequential.
     */
    bool randomOutput() const { return m_random; }

    /**
     * Sets whether the output should be selected randomly or sequentially.
     * By default is sequential.
     */
    void setRandomOutput(bool random) { m_random = random; }

private:

    RuleId m_id;
    QStringList m_input;
    QStringList m_output;
    QStringList m_target;
    QString m_topic;
    QString m_nextTopic;
    bool m_random;
};


/**
 * \brief List of NLP Rules
 */

typedef QList<Rule> RuleList;

/// @}

} // namespace Nlp

/// @}

} // namespace Lvk

#endif // LVK_NLP_RULE_H
