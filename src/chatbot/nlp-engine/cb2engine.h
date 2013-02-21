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

#ifndef LVK_NLP_CB2ENGINE_H
#define LVK_NLP_CB2ENGINE_H

#include "nlp-engine/engine.h"
#include "nlp-engine/tree.h"

#include <QHash>
#include <QString>
#include <QSharedPointer>
#include <memory>

class QMutex;
class QFile;

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
 * \brief The Cb2Engine class provides a custom NLP engine
 *
 * Optionally, sanitizers and lemmatizers can be provided at construction time to improve
 * rule matching.
 */
class Cb2Engine : public Engine
{
public:

    /**
     * Construtcs a Cb2Engine object with NullSanitizer and NullLemmatizer.
     *
     * \see NullSanitizer, NullLemmatizer
     */
    Cb2Engine();

    /**
     * Construtcs a Cb2Engine object with the given \a sanitizer and NullLemmatizer.
     * After construction, the object owns the given pointer.
     *
     * \see NullLemmatizer
     */
    Cb2Engine(Sanitizer *sanitizer);

    /**
     * Construtcs a Cb2Engine object with the given sanitizers and lemmatizer.
     * After construction, the object owns the given pointers.
     */
    Cb2Engine(Sanitizer *preSanitizer, Lemmatizer *lemmatizer, Sanitizer *postSanitizer);

    /**
     * Destroys the object.
     */
    ~Cb2Engine();

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
     * \copydoc Engine::property()
     *
     * Cb2Engine supports only one property:
     * NLP_PROP_PREFER_CUR_TOPIC with values \a true or \a false. If \a true rules on the current
     * topic have higher priority. By default is false.
     */
    virtual QVariant property(const QString &name);

    /**
     * \copydoc Engine::setProperty()
     *
     * Cb2Engine supports only one property:
     * NLP_PROP_PREFER_CUR_TOPIC with values \a true or \a false. If \a true rules on the current
     * topic have higher priority. By default is false.
     */
    virtual void setProperty(const QString &name, const QVariant &value);

    /**
     * \copydoc Engine::clear()
     */
    virtual void clear();

private:
    Cb2Engine(Cb2Engine&);
    Cb2Engine& operator=(Cb2Engine&);

    typedef QHash<QString, QSharedPointer<Nlp::Tree> > TreesMap;
    typedef QHash<QString, QString> TopicsMap;

    RuleList m_rules;
    std::auto_ptr<QFile>      m_logFile;
    TreesMap                  m_trees;
    TopicsMap                 m_topics;
    QMutex *m_mutex;
    bool m_dirty;
    bool m_preferCurTopic;

    void initLog();
    void getAllResponsesWithTree(const QString &treeName, const QString &input,
                                 Nlp::ResultList &results);
    void refresh();
    Nlp::Tree * buildTree(const QString &target);
    void reorderByTopic(const QString &topic, Nlp::ResultList &results);
    QString topicForRule(Nlp::RuleId ruleId);
};

/// @}

} // namespace Nlp

/// @}

} // namespace Lvk

#endif // LVK_NLP_CB2ENGINE_H
