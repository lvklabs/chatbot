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

#include "nlp-engine/engine.h"
#include "ProgramQ/aimlparser.h"

#include <QHash>
#include <QString>
#include <QSharedPointer>
#include <memory>

class AIMLParser;
class QFile;
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

class Sanitizer;
class Lemmatizer;

/**
 * \brief The AimlEngine class provides a NLP engine based on AIML (Artificial Intelligence
 *        Markup Language)
 *
 * The AimlEngine class supports most of the AIML features. The class maps input strings into the
 * \c pattern tag and output strings into the \c template tag. For instance, given a rule:
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
 *
 * Optionally, sanitizers and lemmatizers can be provided at construction time to improve
 * rule matching. Every time an input is provided, AimlEngine applies the following pipeline
 * to the input: \b preSanitizer \b -> \b lemmatizer \b -> \b postSanitizer
 */
class AimlEngine : public Engine
{
public:

    /**
     * Construtcs a AimlEngine object with NullSanitizer and NullLemmatizer.
     *
     * \see NullSanitizer, NullLemmatizer
     */
    AimlEngine();

    /**
     * Construtcs a AimlEngine object with the given \a sanitizer and NullLemmatizer.
     * After construction, the object owns the given pointer.
     *
     * \see NullLemmatizer
     */
    AimlEngine(Sanitizer *sanitizer);

    /**
     * Construtcs a AimlEngine object with the given sanitizers and lemmatizer.
     * After construction, the object owns the given pointers.
     */
    AimlEngine(Sanitizer *preSanitizer, Lemmatizer *lemmatizer, Sanitizer *postSanitizer);

    /**
     * Destroys the object.
     */
    ~AimlEngine();

    /**
     * \copydoc Engine::rules() const
     */
    virtual RuleList rules() const;

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
    virtual QStringList getAllResponses(const QString &input, MatchList &matches);

    /**
     * \copydoc Engine::getAllResponses(const QString &, const QString &, MatchList &)
     */
    virtual QStringList getAllResponses(const QString &input, const QString &target,
                                        MatchList &matches);
    /**
     * \copydoc Engine::setPreSanitizer()
     *
     * Passing a null pointer resets to the default pre-sanitizer.
     */
    virtual void setPreSanitizer(Sanitizer *sanitizer);

    /**
     * \copydoc Engine::setLemmatizer()
     *
     * Passing a null pointer resets to the default lemmatizer.
     */
    virtual void setLemmatizer(Lemmatizer *lemmatizer);

    /**
     * \copydoc Engine::setPostSanitizer()
     *
     * Passing a null pointer resets to the default post-sanitizer.
     */
    virtual void setPostSanitizer(Sanitizer *sanitizer);

    /**
     * Not implemented. This method always returns false.
     */
    virtual bool hasVariable(const QString &input);

    /**
     * Not implemented. This method always returns false.
     */
    virtual bool hasKeywordOp(const QString &input);

    /**
     * Not implemented. This method always returns false.
     */
    virtual bool hasRegexOp(const QString &input);

    /**
     * Not implemented. This method always returns false.
     */
    virtual bool hasConditional(const QString &output);

    /**
     * AimlEngine does not support properties. This method always a null QVariant.
     */
    virtual QVariant property(const QString &name);

    /**
     * AimlEngine does not support properties. This method does nothing.
     */
    virtual void setProperty(const QString &name, const QVariant &value);

private:
    AimlEngine(AimlEngine&);
    AimlEngine& operator=(AimlEngine&);

    typedef QHash<QString, QSharedPointer<AIMLParser> > ParsersMap;

    RuleList m_rules;
    std::auto_ptr<Sanitizer>  m_preSanitizer;
    std::auto_ptr<Sanitizer>  m_postSanitizer;
    std::auto_ptr<Lemmatizer> m_lemmatizer;
    std::auto_ptr<QFile>      m_logFile;
    ParsersMap                m_parsers;
    QMutex *m_mutex;
    bool m_dirty;

    void initLog();

    inline QStringList getAllResponses(const QString &input, const QString &target,
                                       MatchList &matches, bool norm);

    void refreshAiml();
    void buildAiml(QString &aiml, const QString &target);
    void buildAiml(QString &aiml, const Rule &rule);
    void buildAimlRandOutput(QString &aiml,const QStringList &output) const;
    void normalize(QString &input);
    void normalize(QStringList &inputList);
};

/// @}

} // namespace Nlp

/// @}

} // namespace Lvk

#endif // LVK_NLP_AIMLENGINE_H
