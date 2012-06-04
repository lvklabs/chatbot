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

#ifndef LVK_NLP_AIMLENGINE_H
#define LVK_NLP_AIMLENGINE_H

#include "nlpengine.h"

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
 * \brief NLP Engine based on AIML (Artificial Intelligence Markup Language)
 *
 * The AimlEngine class supports most of the AIML features. The class maps input strings into the
 * pattern tag and output strings into the template tags. For instance, given a rule:
 *
 *   Input List: "Hi there", "Hello *"
 *   Output List: "Hi!", "Hello!"
 *
 * It produces an AIML string:
 *
 * <category>
 *   <pattern>Hi there</pattern>
 *   <template>
 *     <random>
 *       <li>Hi!</li>
 *       <li>Hello!</li>
 *     </random>
 *   </template>
 * </category>
 *
 * <category>
 *   <pattern>Hello *</pattern>
 *   <template>
 *     <random>
 *       <li>Hi!</li>
 *       <li>Hello!</li>
 *     </random>
 *   </template>
 * </category>
 *
 */

class AimlEngine : public Engine
{
public:
    AimlEngine();

    AimlEngine(Sanitizer *sanitizer);

    ~AimlEngine();

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
