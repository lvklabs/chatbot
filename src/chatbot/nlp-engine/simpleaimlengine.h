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

#ifndef LVK_NLP_SIMPLEAIMLENGINE_H
#define LVK_NLP_SIMPLEAIMLENGINE_H

#include "nlp-engine/aimlengine.h"

#include <QHash>

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
 * \brief The SimpleAimlEngine class is a wrapper over the AimlEngine to provide a simpler
 *        syntax than AIML
 *
 * SimpleAimlEngine supports handy variable names and if-else conditionals. For instance instead of
 * writing:
 *
   \verbatim
   Input:  Do you like *
   Output: <think><set name="like"><star/></set></think>
           <condition>
               <li name="like" value="football"> Yes I like <star/></li>
               <li>No, I don't</li>
           </condition>
   \endverbatim
 *
 * With SimpleAimlEngine we can write:
 *
   \verbatim
   Input:  Do you like [VarName]
   Output: {if [VarName] = football}
           Yes I like [VarName]
           {else}
           No, I don't
   \endverbatim
 *
 * SimpleAimlEngine also supports regex-like quantifiers + and * where + means one or more words
 * and * means zero or more words
 */

class SimpleAimlEngine : public AimlEngine
{
public:

    /**
     * Construtcs a SimpleAimlEngine object with the default sanitizer.
     *
     * \see DefaultSanitizer
     */
    SimpleAimlEngine();

    /**
     * Construtcs a SimpleAimlEngine object with the given sanitizer.
     */
    SimpleAimlEngine(Sanitizer *sanitizer);

    /**
     * \copydoc AimlEngine::rules() const
     */
    virtual const RuleList &rules() const;

    /**
     * \copydoc AimlEngine::rules()
     */
    virtual RuleList &rules();

    /**
     * \copydoc AimlEngine::setRules()
     */
    virtual void setRules(const RuleList &rules);

    /**
     * \copydoc AimlEngine::getAllResponses(const QString &, const QString &, MatchList &)
     */
    virtual QList<QString> getAllResponses(const QString &input, const QString &target,
                                           MatchList &matches);

    /**
     * \brief Class thrown when an invalid syntax was found while parsing rules.
     */
    class InvalidSyntaxException;

private:

    struct ConvertionContext
    {
        Lvk::Nlp::Rule rule;
        QString varName;
        int inputIdx;
        QString input;
    };

    // Remaps: AimlEngine input index -> SimpleAimlEngine input index
    typedef QHash<int, int> IndexRemap;
    typedef QHash<RuleId, IndexRemap > RuleIndexRemap;

    RuleList m_rules;

    QRegExp m_varNameRegex;
    QRegExp m_ifElseRegex;
    QRegExp m_ifRegex;
    QRegExp m_keywordRegex;

    RuleIndexRemap m_indexRemap;

    void initRegexs();

    void convertToPureAiml(RuleList &rules);
    void convertToPureAiml(Lvk::Nlp::Rule &newRules, const Lvk::Nlp::Rule &rule);

    void convertInputList(QStringList &inputList, ConvertionContext &ctx);
    bool convertVariables(QStringList &inputList, ConvertionContext &ctx);
    bool convertKeywordOp(QStringList &inputList, ConvertionContext &ctx);
    bool convertOtherOps(QStringList &inputList, ConvertionContext &ctx);
    void convertOutputList(QStringList &outputList, ConvertionContext &ctx);

    void remap(Engine::MatchList &matches);
};

/// @}

} // namespace Nlp

/// @}

} // namespace Lvk

#endif // LVK_NLP_SIMPLEAIMLENGINE_H
