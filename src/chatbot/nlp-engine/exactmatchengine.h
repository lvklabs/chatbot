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

#ifndef LVK_NLP_EXACTMATCHENGINE_H
#define LVK_NLP_EXACTMATCHENGINE_H

#include "nlp-engine/engine.h"

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
 * \brief The ExactMatchEngine class provides an exact-match case insentive NLP engine.
 */

class ExactMatchEngine : public Engine
{
public:

    /**
     * Construtcs an ExactMatchEngine object.
     */
    ExactMatchEngine();

    /**
     * Destroys the object.
     */
    ~ExactMatchEngine();

    /**
     * \copydoc Engine::rules() const
     */
    virtual RuleList rules() const;

    /**
     * \copydoc Engine::setRules()
     */
    virtual void setRules(const RuleList &rules);

    /**
     * \copydoc Engine::getResponse(const QString &, MatchList &)
     */
    virtual QString getResponse(const QString &input, MatchList &matches);

    /**
     * \copydoc Engine::getResponse(const QString &input, const QString&, MatchList &)
     */
    virtual QString getResponse(const QString &input, const QString &target, MatchList &matches);

    /**
     * \copydoc Engine::getAllResponses(const QString &, MatchList &)
     */
    virtual QStringList getAllResponses(const QString &input, MatchList &matches);

    /**
     * \copydoc Engine::getAllResponses(const QString &, const QString &, MatchList &)
     */
    virtual QStringList getAllResponses(const QString &input, const QString &target,
                                        MatchList &matches);

    /**
     * ExactMatchEngine ignores this method.
     */
    virtual void setPreSanitizer(Sanitizer *sanitizer);

    /**
     * ExactMatchEngine ignores this method.
     */
    virtual void setLemmatizer(Lemmatizer *lemmatizer);

    /**
     * ExactMatchEngine ignores this method.
     */
    virtual void setPostSanitizer(Sanitizer *sanitizer);

private:

    RuleList m_rules;
};

/// @}

} // namespace Nlp

/// @}

} // namespace Lvk

#endif // EXACTMATCHENGINE_H
