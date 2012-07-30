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

#ifndef LVK_NLP_AIMLENGINE_H
#define LVK_NLP_AIMLENGINE_H

#include "nlp-engine/nlpengine.h"

class AIMLParser;

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace Nlp
{

/// \ingroup Lvk
/// \addtogroup Nlp
/// @{

class Sanitizer;

/**
 * \brief The AimlEngine class provides a NLP engine based on AIML (Artificial Intelligence
 *        Markup Language)
 *
 * The AimlEngine class supports most of the AIML features. The class maps input strings into the
 * \tt pattern tag and output strings into the \tt template tag. For instance, given a rule:
 *
   \verbatim
   Input List: "Hi there", "Hello *"
   Output List: "Hi!", "Hello!"
   \endverbatim
 *
 * It produces an AIML string:
 *
   \verbatim
   <category>
     <pattern>Hi there</pattern>
     <template>
       <random>
         <li>Hi!</li>
         <li>Hello!</li>
       </random>
     </template>
   </category>

   <category>
     <pattern>Hello *</pattern>
     <template>
       <random>
         <li>Hi!</li>
         <li>Hello!</li>
       </random>
     </template>
   </category>
  \endverbatim
 */

class AimlEngine : public Engine
{
public:

    /**
     * Construtcs a AimlEngine object with the default sanitizer.
     *
     * \see DefaultSanitizer
     */
    AimlEngine();

    /**
     * Construtcs a AimlEngine object with the given sanitizer.
     */
    AimlEngine(Sanitizer *sanitizer);

    /**
     * Destroys the object.
     */
    ~AimlEngine();

    /**
     * \copydoc Engine::rules() const
     */
    virtual const RuleList &rules() const;

    /**
     * \copydoc Engine::rules()
     */
    virtual RuleList &rules();

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
    virtual QList<QString> getAllResponses(const QString &input, MatchList &matches);

    /**
     * \copydoc Engine::getAllResponses(const QString &, const QString &, MatchList &)
     */
    virtual QList<QString> getAllResponses(const QString &input, const QString &target,
                                           MatchList &matches);

private:
    AimlEngine(AimlEngine&);
    AimlEngine& operator=(AimlEngine&);

    RuleList m_rules;
    AIMLParser *m_aimlParser;
    Sanitizer *m_sanitizer;

    void resetParser();
    void buildAiml(QString &aiml);
    void buildAiml(QString &aiml, const Rule &rule);
};

/// @}

} // namespace Nlp

/// @}

} // namespace Lvk

#endif // LVK_NLP_AIMLENGINE_H
