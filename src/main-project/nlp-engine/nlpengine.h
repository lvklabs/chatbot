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

#ifndef LVK_NLP_NLPENGINE_H
#define LVK_NLP_NLPENGINE_H

#include <QString>
#include <QPair>

#include "nlprule.h"

namespace Lvk
{

namespace Nlp
{

/**
 * \brief Abstract interface for all NLP Engines
 */

class Engine
{
public:
    typedef QList< QPair<RuleId, int> > MatchList; //! list of pairs (rule_id, matched_input_number)

    virtual const RuleList &rules() const = 0;

    virtual RuleList &rules() = 0;

    virtual void setRules(const RuleList &rules) = 0;

    virtual QString getResponse(const QString &input, MatchList &matches) = 0;

    virtual QList<QString> getAllResponses(const QString &input, MatchList &matches) = 0;

    virtual ~Engine() {}
};

} // namespace Nlp

} // namespace Lvk

#endif // LVK_NLP_NLPENGINE_H
