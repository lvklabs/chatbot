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

#include "nlp-engine/nlpengine.h"

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
 * \brief Exact match case insentive NLP engine
 *
 */

class ExactMatchEngine : public Engine
{
public:
    ExactMatchEngine();

    ~ExactMatchEngine();

    virtual const RuleList &rules() const;

    virtual RuleList &rules();

    virtual void setRules(const RuleList &rules);

    virtual QString getResponse(const QString &input, MatchList &matches);

    virtual QString getResponse(const QString &input, const QString &target,
                                MatchList &matches);

    virtual QList<QString> getAllResponses(const QString &input, MatchList &matches);

    virtual QList<QString> getAllResponses(const QString &input, const QString &target,
                                           MatchList &matches);

private:

    RuleList m_rules;
};

/// @}

} // namespace Nlp

/// @}

} // namespace Lvk

#endif // EXACTMATCHENGINE_H
