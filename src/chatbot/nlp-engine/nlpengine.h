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

#ifndef LVK_NLP_NLPENGINE_H
#define LVK_NLP_NLPENGINE_H

#include <QString>
#include <QPair>

#include "nlp-engine/rule.h"

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
 * \brief The Engine class provides an abstract interface for all NLP Engines
 */
class Engine
{
public:
    /**
     * MatchList provides a list of pairs (RuleId, InputNumber)
     */
    typedef QList< QPair<RuleId, int> > MatchList;

    /**
     * Returns a const reference to the list of rules.
     */
    virtual const RuleList &rules() const = 0;

    /**
     * Returns a reference to the list of rules.
     */
    virtual RuleList &rules() = 0;

    /**
     * Sets the list of rules.
     */
    virtual void setRules(const RuleList &rules) = 0;

    /**
     * Gets a response for the given \a input ignoring target rules.
     *
     * Returns the response if there is a match and \a matches contains
     * the list of rules and inputs that has matched. If the list contains two or more
     * objects it means there was recursion. Otherwise; returns an empty string and \a matches
     * is empty.
     */
    virtual QString getResponse(const QString &input, MatchList &matches) = 0;

    /**
     * Gets a response for the given \a input and \a target.
     *
     * Returns the response if there is a match and \a matches contains
     * the list of rules and inputs that has matched. If the list contains two or more
     * objects it means there was recursion. Otherwise; returns an empty string and \a matches
     * is empty.
     */
    virtual QString getResponse(const QString &input, const QString &target,
                                MatchList &matches) = 0;

    /**
     * Gets all responses for the given \a input ignoring target rules.
     *
     * Returns a list of responses if there is match and \a matches contains
     * the list of rules and inputs that has matched. If the list contains two or more
     * objects it means there was recursion. Otherwise; returns an empty string and \a matches
     * is empty.
     */
    virtual QList<QString> getAllResponses(const QString &input, MatchList &matches) = 0;

    /**
     * Gets all responses for the given \a input and \a target.
     *
     * Returns a list of responses if there is match and \a matches contains
     * the list of rules and inputs that has matched. If the list contains two or more
     * objects it means there was recursion. Otherwise; returns an empty string and \a matches
     * is empty.
     */
    virtual QList<QString> getAllResponses(const QString &input, const QString &target,
                                           MatchList &matches) = 0;

    virtual ~Engine() {}
};

/// @}

} // namespace Nlp

/// @}

} // namespace Lvk

#endif // LVK_NLP_NLPENGINE_H
