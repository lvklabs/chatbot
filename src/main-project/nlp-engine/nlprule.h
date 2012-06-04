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

#ifndef LVK_NLP_NLPRULE_H
#define LVK_NLP_NLPRULE_H

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
 * \brief Rule ID
 */

typedef unsigned long RuleId;

/**
 * \brief This class provides a NLP Rule
 *
 * A NLP rule is simply an structure that contains a list of input strings and a list of output
 * strings. How these input and output strings are interpreted depends on the engine implemenation.
 * Alternatively, it can contain a list of targets to which the rule is intended to apply.
 *
 * @see ExactMatchEngine, AimlEngine, SimpleAimlEngine
 */

class Rule
{
public:

    Rule(RuleId id = 0) : m_id(id) {}

    Rule(RuleId id, const QStringList &input, const QStringList &output)
        : m_id(id), m_input(input), m_output(output) {}

    Rule(RuleId id, const QStringList &input, const QStringList &output, const QStringList &target)
        : m_id(id), m_input(input), m_output(output), m_target(target) {}

    RuleId id() const { return m_id; }

    void setId(RuleId id) { m_id = id; }


    const QStringList &input() const { return m_input; }

    QStringList &input() { return m_input; }

    void setInput(const QStringList &input) { m_input = input; }


    const QStringList &output() const { return m_output; }

    QStringList &output() { return m_output; }

    void setOuput(const QStringList &output) { m_output = output; }


    const QStringList &target() const { return m_target; }

    QStringList &target() { return m_target; }

    void setTarget(const QStringList &target) { m_target = target; }

private:

    RuleId m_id;
    QStringList m_input;
    QStringList m_output;
    QStringList m_target;
};


/**
 * \brief List of NLP Rules
 */

typedef QList<Rule> RuleList;

/// @}

} // namespace Nlp

/// @}

} // namespace Lvk

#endif // LVK_NLP_NLPRULE_H
