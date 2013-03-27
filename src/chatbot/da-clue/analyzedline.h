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

#ifndef LVK_CLUE_ANALYZEDLINE_H
#define LVK_CLUE_ANALYZEDLINE_H

#include "da-clue/scriptline.h"
#include "nlp-engine/rule.h"

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace Clue
{

/// \ingroup Lvk
/// \addtogroup Clue
/// @{


/**
 * @brief The LineStatus enum
 */
enum LineStatus
{
    NotAnalyzed,
    NoAnswerFound,
    MismatchExpectedAnswer,
    MatchForbiddenAnswer,
    AnswerOk
};

/**
 * \brief The AnalyzedLine class provides an analized script line by the ClueEngine
 */
class AnalyzedLine : public ScriptLine
{
public:

    /**
     * Constructs an empty AnalyzedLine
     */
    AnalyzedLine()
        : ruleId(0), inputIdx(-1), score(0), outputIdx(-1), status(NotAnalyzed) { }

    /**
     * Constructs an AnalyzedLine with \a line
     */
    AnalyzedLine(const ScriptLine &line)
        : ScriptLine(line), ruleId(0), inputIdx(-1), score(0), outputIdx(-1), status(NotAnalyzed)
    { }

    /**
     * Constructs an AnalyzedLine with \a line, \a ruleId, \a inputIdx and \a score
     */
    AnalyzedLine(const ScriptLine &line, Nlp::RuleId ruleId, int inputIdx, float score,
                 const QString &answer = "", int outputIdx = -1, LineStatus status = NotAnalyzed)
        : ScriptLine(line), ruleId(ruleId), inputIdx(inputIdx), score(score), answer(answer),
          outputIdx(outputIdx), status(status) { }


    Nlp::RuleId ruleId; ///< The rule ID of the matched rule. 0 if there is no match.
    int inputIdx;       ///< The input index of the matched rule. -1 if there is no match.
    float score;        ///< The score of the matched rule. 0 if there is no match.
    QString answer;     ///< The selected answer to the question. Empty if there is no match.
    int outputIdx;      ///< The output index of the selected output. -1 if there is no match.
    LineStatus status;  ///< The status of the analyzed line
    QString topic;      ///< The current topic after analyzing the rule


    /**
     * Returns true if \a this instance is equal to \a other
     */
    bool operator==(const AnalyzedLine &other) const
    {
        return ScriptLine::operator==(other) &&
                ruleId == other.ruleId &&
                inputIdx == other.inputIdx &&
                score == other.score &&
                answer == other.answer &&
                outputIdx == other.outputIdx &&
                status == other.status &&
                topic == other.topic;
    }

    /**
     * Returns true if \a this instance is *not* equeal to \a other
     */
    bool operator!=(const AnalyzedLine &other) const
    {
        return !this->operator==(other);
    }
};

/// @}

} // namespace Clue

/// @}

} // namespace Lvk


#endif // LVK_CLUE_ANALYZEDLINE_H

