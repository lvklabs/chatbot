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

#ifndef LVK_NLP_CONDOUTPUTLIST_H
#define LVK_NLP_CONDOUTPUTLIST_H

#include <QList>
#include <QString>
#include <QStringList>

#include "nlp-engine/varstack.h"
#include "nlp-engine/condoutput.h"

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
 * \brief The CondOutputList provides a list of CondOutput's
 */
class CondOutputList : public QList<CondOutput>
{
public:
    CondOutputList(const QStringList &outputs = QStringList(), bool random = false);

    QString nextValidOutput(const Nlp::VarStack &varStack) const;

    void setRandomOutput(bool random);

private:
    mutable int m_next;
};

/// @}

} // namespace Nlp

/// @}

} // namespace Lvk


#endif // LVK_NLP_CONDOUTPUTLIST_H

