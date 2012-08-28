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

#ifndef LVK_BE_RULESTATSHELPER_H
#define LVK_BE_RULESTATSHELPER_H

#include "back-end/statshelper.h"
#include "back-end/rule.h"
#include "nlp-engine/simpleaimlengine.h" // TODO use factories


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
 * \brief The RuleStatsHelper class provides rule statistics such as total words,
 *        total rules, lexicon size and rule points.
 */
class RuleStatsHelper : public StatsHelper
{
public:

    /**
     * Constructs a RuleStatsHelper and provides statistics for the given \a root rule.
     */
    RuleStatsHelper(const Lvk::BE::Rule *root)
        : m_rules(0), m_points(0)
    {
        if (root) {
            rcount(root);
        }
    }

    /**
     * Returns the total amount of rules of type OrdinaryRule or EvasivesRule.
     */
    unsigned totalRules() const
    {
        return m_rules;
    }

    /**
     * Returns the total amount of rule points.
     *
     * For each rule we give the following points:
     * <ul>
     *   <li>1 point simple rules</li>
     *   <li>2 points rules with operators</li>
     *   <li>3 points rules with variables</li>
     *   <li>4 points rules with conditionals</li>
     * </ul>
     */
    unsigned totalRulePoints() const
    {
        return m_points;
    }

protected:

    /**
     * Counts all words in the given rule \a rule and gets statistics. If the rule has children
     * it counts recursively.
     */
    void rcount(const Lvk::BE::Rule *rule)
    {
        Lvk::BE::Rule::const_iterator it;
        for (it = rule->begin(); it != rule->end(); ++it) {
            const Lvk::BE::Rule *rule = *it;
            if (rule->isComplete() && rule->type() != Lvk::BE::Rule::ContainerRule) {
                count(rule);
                m_rules++;
                m_points += points(rule);
            }
        }
    }

    /**
     * Counts all words in the given rule \a rule and gets statistics
     */
    void count(const Lvk::BE::Rule *rule)
    {
       StatsHelper::count(rule->input());
       StatsHelper::count(rule->output());
    }

    /**
     * Returns the points of the given \a rule
     */
    unsigned points(const Lvk::BE::Rule *rule) const
    {
        unsigned p = 0;

        // To calculate points Only using first non-empty output

        QString output;
        foreach (const QString &o, rule->output()) {
            if (o.trimmed().size() > 0) {
                output = o;
                break;
            }
        }
        foreach (const QString &input, rule->input()) {
            p += points(input, output);
        }

        return p;
    }

    /**
     * Returns the points of the pair \a (input, output).
     * <ul>
     *   <li>1 point simple rules</li>
     *   <li>2 points rules with operators</li>
     *   <li>3 points rules with variables</li>
     *   <li>4 points rules with conditionals</li>
     * </ul>
     */
    unsigned points(const QString &input, const QString &output) const
    {
        if (input.isEmpty() || output.isEmpty()) {
            return 0;
        } else if (Nlp::SimpleAimlEngine::hasVariable(input)) {
            if (Nlp::SimpleAimlEngine::hasConditional(output)) {
                return 4;
            } else if (Nlp::SimpleAimlEngine::hasVariable(output)) {
                return 3;
            } else {
                return 1;
            }
        } else if (Nlp::SimpleAimlEngine::hasKeywordOp(input)) {
            return 2;
        } else if (Nlp::SimpleAimlEngine::hasRegexOp(input)) {
            return 2;
        } else {
            return 1;
        }
    }

private:
    unsigned m_rules;
    unsigned m_points;
};

/// @}

} // namespace BE

/// @}

} // namespace Lvk


#endif // LVK_BE_RULESTATSHELPER_H

