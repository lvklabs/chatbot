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

#ifndef LVK_NLP_HYBRIDENGINE_H
#define LVK_NLP_HYBRIDENGINE_H

#include "nlp-engine/aimlengine.h"

#include <QHash>

class QMutex;

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
 * \brief The HybridEngine class is a hybrid engine that mixes AimlEngine with ExactMatchEngine
 *        and provides a much simpler syntax than AIML
 *
 * HybridEngine supports handy variable names and if-else conditionals. For instance instead of
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
 * With HybridEngine we can write:
 *
   \verbatim
   Input:  Do you like [VarName]
   Output: {if [VarName] = football}
           Yes I like [VarName]
           {else}
           No, I don't
   \endverbatim
 *
 * HybridEngine supports regex-like quantifiers + and * where + means one or more words
 * and * means zero or more words.
 *
 * HybridEngine also supports exact match if inputs are surrounded by double quotes. This is very
 * useful to match non-alphanumeric strings such as smileys.
 *
   \verbatim
   Input:  ":)"
   Output: :)
   \endverbatim
 *
 * \see AimlEngine
 */

class HybridEngine : public AimlEngine
{
public:

    /**
     * Construtcs a HybridEngine object with NullSanitizer and NullLemmatizer.
     *
     * \see NullSanitizer, NullLemmatizer
     */
    HybridEngine();

    /**
     * Construtcs a HybridEngine object with the given \a sanitizer and NullLemmatizer.
     * After construction, the object owns the given pointer.
     *
     * \see NullLemmatizer
     */
    HybridEngine(Sanitizer *sanitizer);

    /**
     * Construtcs a HybridEngine object with the given sanitizers and lemmatizer.
     * After construction, the object owns the given pointers.
     */
    HybridEngine(Sanitizer *preSanitizer, Lemmatizer *lemmatizer, Sanitizer *postSanitizer);

    /**
     * Destroys the object
     */
    ~HybridEngine();

    /**
     * \copydoc AimlEngine::rules() const
     */
    virtual RuleList rules() const;

    /**
     * \copydoc AimlEngine::setRules()
     */
    virtual void setRules(const RuleList &rules);

    /**
     * \copydoc AimlEngine::getAllResponses(const QString &, const QString &, MatchList &)
     */
    virtual QStringList getAllResponses(const QString &input, const QString &target,
                                        MatchList &matches);

    /**
     * \brief Class thrown when an invalid syntax was found while parsing rules.
     */
    class InvalidSyntaxException;

    /**
     * \copydoc Engine::hasVariable()
     */
    virtual bool hasVariable(const QString &input);

    /**
     * \copydoc Engine::hasKeywordOp()
     */
    virtual bool hasKeywordOp(const QString &input);

    /**
     * \copydoc Engine::hasRegexOp()
     */
    virtual bool hasRegexOp(const QString &input);

    /**
     * \copydoc Engine::hasConditional()
     */
    virtual bool hasConditional(const QString &output);

    /**
     * Returns true if \a input is a literal string. Otherwise; returns false.
     * Literals are declared by surrounding the string with double quotes.
     */
    virtual bool isLiteral(const QString &input);

    /**
     * \copydoc Engine::property()
     *
     * HybridEngine only supports the property NLP_PROP_EXACT_MATCH with values \a true or \a false
     * to enable or disable exact match support.
     */
    virtual QVariant property(const QString &name);

    /**
     * \copydoc Engine::setProperty()
     *
     * HybridEngine only supports the property NLP_PROP_EXACT_MATCH with values \a true or \a false
     * to enabled or disable exact match support.
     */
    virtual void setProperty(const QString &name, const QVariant &value);
private:

    struct ConvertionContext
    {
        Lvk::Nlp::Rule rule;
        QString varName;
        int inputIdx;
        QString input;
    };

    // Remaps: AimlEngine input index -> HybridEngine input index
    typedef QHash<int, int> IndexRemap;
    typedef QHash<RuleId, IndexRemap > RuleIndexRemap;

    RuleList m_rules;
    QMutex *m_engineMutex;
    Engine *m_emEngine;

    QRegExp m_varNameRegex;
    QRegExp m_ifElseRegex;
    QRegExp m_ifRegex;
    QRegExp m_keywordRegex;

    RuleIndexRemap m_indexRemap;

    void initRegexs();
    void refreshEngine();

    typedef void (HybridEngine::*ConvertionMemb)(Nlp::Rule &, const Nlp::Rule &);

    void convert(RuleList &newRules, const RuleList &rules, ConvertionMemb convertion);

    void convertToAiml(Nlp::Rule &newRules, const Nlp::Rule &rule);
    void convertToExactMatch(Nlp::Rule &newRules, const Nlp::Rule &rule);

    void convertInputList(QStringList &inputList, ConvertionContext &ctx);
    void convertVariables(QStringList &inputList, ConvertionContext &ctx);
    void convertKeywordOp(QStringList &inputList, ConvertionContext &ctx);
    void convertRegexOp(QStringList &inputList, ConvertionContext &ctx);
    void convertOutputList(QStringList &outputList, ConvertionContext &ctx);
    void convertLiterals(QStringList &inputList, ConvertionContext &ctx);

    void remap(Engine::MatchList &matches);
};

/// @}

} // namespace Nlp

/// @}

} // namespace Lvk

#endif // LVK_NLP_HYBRIDENGINE_H
