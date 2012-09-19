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

#ifndef LVK_STATS_RULESTATSHELPER_H
#define LVK_STATS_RULESTATSHELPER_H

#include "stats/statshelper.h"
#include "back-end/rule.h"
#include "nlp-engine/enginefactory.h"

#include <QSet>
#include <QPair>
#include <memory>

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace Stats
{

/// \ingroup Lvk
/// \addtogroup Stats
/// @{

/**
 * \brief The RuleStatsHelper class provides rule statistics such as total words,
 *        total rules, lexicon size and rule points.
 */
class RuleStatsHelper : public StatsHelper
{
public:

    /**
     * Constructs an emtpy RuleStatsHelper
     */
    RuleStatsHelper()
        : m_rules(0), m_points(0), m_engine(Nlp::EngineFactory().createEngine())
    {
    }

    /**
     * Constructs a RuleStatsHelper and provides statistics for the given \a root rule.
     */
    RuleStatsHelper(const Lvk::BE::Rule *root)
        : m_rules(0), m_points(0), m_engine(Nlp::EngineFactory().createEngine())
    {
        if (root) {
            rcount(root);
        }
    }

    /**
     * Returns the total amount of rules of type OrdinaryRule or EvasivesRule.
     */
    unsigned rulesCount() const
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
    unsigned points() const
    {
        return m_points;
    }

    /**
     * Resets stats with the given new \a root
     */
    void reset(const Lvk::BE::Rule *root)
    {
        clear();
        if (root) {
            rcount(root);
        }
    }

    /**
     * Sets all stats to zero.
     */
    void clear()
    {
        StatsHelper::clear();
        m_ioPairs.clear();
        m_rules = 0;
        m_points = 0;
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
    unsigned points(const Lvk::BE::Rule *rule)
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
            // Count only unique pairs (input, output)
            IOPair pair(input, output);
            if (!m_ioPairs.contains(pair)) {
                m_ioPairs.insert(pair);
                p += points(input, output);
            }
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
        } else if (m_engine->hasVariable(input)) {
            if (m_engine->hasConditional(output)) {
                return 4;
            } else if (m_engine->hasVariable(output)) {
                return 3;
            } else {
                return 1;
            }
        } else if (m_engine->hasKeywordOp(input)) {
            return 2;
        } else if (m_engine->hasRegexOp(input)) {
            return 2;
        } else {
            return 1;
        }
    }

private:
    RuleStatsHelper(RuleStatsHelper&);
    RuleStatsHelper& operator=(RuleStatsHelper&);

    typedef QPair<QString, QString> IOPair;
    QSet<IOPair> m_ioPairs;

    unsigned m_rules;
    unsigned m_points;
    std::auto_ptr<Nlp::Engine> m_engine;
};

/// @}

} // namespace Stats

/// @}

} // namespace Lvk


#endif // LVK_STATS_RULESTATSHELPER_H

