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
#include "common/random.h"

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
 * \brief
 */
struct Predicate
{
    virtual ~Predicate() { }

    bool operator()() const
    {
        return true;
    }

};

/**
 * \brief
 */
class CondOutput
{
public:
    CondOutput(const QString &output = "", const Predicate &p = Predicate())
        : output(output), predicate(p) { }

    QString output;
    Predicate predicate;

    bool isNull() const
    {
        return output.isNull() /* TODO && null_predicate */;
    }
};

/**
 * \brief
 */
class CondOutputList : public QList<CondOutput>
{
public:
    CondOutputList(bool random = false)
        : m_next(0)
    {
        setRandomOutput(random);
    }

    const CondOutput &nextValid() const
    {
        // If random
        if (m_next == -1) {
            QList<CondOutput> valid;
            for (int i = 0; i < size(); ++i) {
                const Nlp::CondOutput &co = at(i);
                if (co.predicate()) {
                    valid.append(co);
                }
            }
            if (!valid.isEmpty()) {
                return valid[Cmn::Random::getInt(0, valid.size() - 1)];
            }
        // if secuential
        } else {
            for (int i = 0; i < size(); ++i) {
                int j = (m_next  + i) % size();
                const Nlp::CondOutput &co = at(j);
                if (co.predicate()) {
                    m_next = j + 1;
                    return co;
                }
            }
        }

        // No valid output, returning null
        static Nlp::CondOutput nullco;
        return nullco;
    }

    void setRandomOutput(bool random)
    {
        m_next = random ? -1 : 0;
    }

    // TODO check if we need to define operator=

private:
    mutable int m_next;
};

/// @}

} // namespace Nlp

/// @}

} // namespace Lvk


#endif // LVK_NLP_CONDOUTPUT_H

