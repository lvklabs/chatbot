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

#ifndef LVK_NLP_RESULT_H
#define LVK_NLP_RESULT_H

#include "nlp-engine/rule.h"

#include <QString>
#include <QList>

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
 * \brief The result class provides all the information regarding a sentence match
 */
class Result
{
public:
    /**
     * Constructs a Result object with \a output, rule ID \a ruleId, input index \a inputIdx
     * and \a score
     */
    Result(const QString &output = "", RuleId ruleId = 0, int inputIdx = 0, float score = 0)
        : output(output), ruleId(ruleId), inputIdx(inputIdx), score(score) { }

    QString output; ///< The output string without expanding variables
    RuleId ruleId;  ///< The original rule ID
    int inputIdx;   ///< The input index of the rule
    float score;    ///< The matching score

    /**
     * Returns true if the score of \a this is less than the score of \a other.
     * Otherwise; returns false.
     */
    bool operator<(const Result &other) const
    {
        return score < other.score;
    }

    /**
     * Returns true if the result is null. Otherwise; returns false.
     */
    bool isNull()
    {
        return output.isEmpty() && !ruleId && !inputIdx && !score;
    }

    /**
     * Returns true if the result is *not* null. Otherwise; returns false.
     */
    bool isValid()
    {
        return !isNull();
    }

    /**
     * Clears the content of the object
     */
    void clear()
    {
        output.clear();
        ruleId = 0;
        inputIdx = 0;
        score = 0;
    }
};


/**
 * \brief This method adds support to print debug information of Result objects
 */
inline QDebug& operator<<(QDebug& dbg, const Result &r)
{
    dbg.space() << r.score << r.output << r.ruleId << r.inputIdx;

    return dbg.maybeSpace();
}


/**
 * The ResultList class provides a list of Result's
 */
typedef QList<Result> ResultList;

/// @}

} // namespace Nlp

/// @}

} // namespace Lvk


#endif // LVK_NLP_RESULT_H

