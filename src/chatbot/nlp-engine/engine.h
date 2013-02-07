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
#include <QVariant>
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

class Sanitizer;
class Lemmatizer;

/**
 * \brief The Engine class provides an abstract interface for all NLP Engines
 */
class Engine
{
public:

    /**
     * Destroys the object.
     */
    virtual ~Engine() {}

    /**
     * RuleMatch provides a pair (RuleId, InputNumber)
     */
    typedef QPair<RuleId, int> RuleMatch;

    /**
     * MatchList provides a list of RuleMatch
     */
    typedef QList<RuleMatch> MatchList;

    /**
     * Returns a copy of the rules.
     */
    virtual RuleList rules() const = 0;

    /**
     * Sets the list of rules.
     */
    virtual void setRules(const RuleList &rules) = 0;

    /**
     * Gets a response for the given \a input ignoring targets.
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
     * Gets all responses for the given \a input ignoring targets.
     *
     * Returns a list of responses if there is match and \a matches contains
     * the list of rules and inputs that has matched. If the list contains two or more
     * objects it means there was recursion. Otherwise; returns an empty string and \a matches
     * is empty.
     */
    virtual QStringList getAllResponses(const QString &input, MatchList &matches) = 0;

    /**
     * Gets all responses for the given \a input and \a target.
     *
     * Returns a list of responses if there is match and \a matches contains
     * the list of rules and inputs that has matched. If the list contains two or more
     * objects it means there was recursion. Otherwise; returns an empty string and \a matches
     * is empty.
     */
    virtual QStringList getAllResponses(const QString &input, const QString &target,
                                        MatchList &matches) = 0;

    /**
     * Sets the pre-lemmatization \a sanitizer, i.e. the sanitizer to be executed before
     * lemmatization. The instance owns the given pointer.
     */
    virtual void setPreSanitizer(Sanitizer *sanitizer) = 0;

    /**
     * Sets the \a lemmatizer. The instance owns the given pointer.
     */
    virtual void setLemmatizer(Lemmatizer *lemmatizer) = 0;

    /**
     * Sets the post-lemmatization \a sanitizer, i.e. the sanitizer to be executed after
     * lemmatization. The instance owns the given pointer.
     */
    virtual void setPostSanitizer(Sanitizer *sanitizer) = 0;

    /**
     * Returns true if \a input contains a variable declaration. Otherwise; returns false.
     */
    virtual bool hasVariable(const QString &input) = 0;

    /**
     * Returns true if \a input contains a keyword operator. Otherwise; returns false.
     */
    virtual bool hasKeywordOp(const QString &input) = 0;

    /**
     * Returns true if \a input contains a regex operator. Otherwise; returns false.
     */
    virtual bool hasRegexOp(const QString &input) = 0;

    /**
     * Returns true if \a output contains an if-else conditional. Otherwise; returns false.
     */
    virtual bool hasConditional(const QString &output) = 0;

    /**
     * Returns the value of the given property \a name
     */
    virtual QVariant property(const QString &name) = 0;

    /**
     * Sets \a value to property \a name
     */
    virtual void setProperty(const QString &name, const QVariant &value) = 0;

    /**
     * Clears the engine state. Properties are not considered part of the state, so they are not
     * cleared.
     */
    virtual void clear() = 0;
};

/// @}

} // namespace Nlp

/// @}

} // namespace Lvk

#endif // LVK_NLP_NLPENGINE_H
