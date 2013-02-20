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

#ifndef LVK_NLP_CONDOUTPUT_H
#define LVK_NLP_CONDOUTPUT_H

#include <QList>
#include <QString>
#include <QStringList>
#include <QSharedPointer>

#include "nlp-engine/predicate.h"

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace Nlp
{

class VarStack;

/// \ingroup Lvk
/// \addtogroup Nlp
/// @{

/**
 * \brief
 */
class CondOutput
{
public:
    CondOutput(const QString &rawOutput = "");

    bool eval(const Nlp::VarStack &varStack, QString &output) const;

private:
    QStringList m_outputs;
    QList< QSharedPointer<Nlp::Predicate> > m_predicates;

    void append(const QString &output, Nlp::Predicate *pred);
};

/// @}

} // namespace Nlp

/// @}

} // namespace Lvk


#endif // LVK_NLP_CONDOUTPUT_H

