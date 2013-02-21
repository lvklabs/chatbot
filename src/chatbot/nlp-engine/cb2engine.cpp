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

#include "nlp-engine/cb2engine.h"
#include "nlp-engine/rule.h"
#include "nlp-engine/nlpproperties.h"
#include "nlp-engine/globaltools.h"
#include "common/settings.h"
#include "common/settingskeys.h"
#include "common/logger.h"

#include <QStringList>
#include <QFile>
#include <QDir>
#include <QMutex>
#include <QMutexLocker>
#include <QtDebug>

#define ANY_USER    ""

//--------------------------------------------------------------------------------------------------
// Helpers
//--------------------------------------------------------------------------------------------------

namespace
{

// "std::make_ptr"-like function to construct QSharedPointers
template<class T>
inline QSharedPointer<T> makeSharedPtr(T *p)
{
    return QSharedPointer<T>(p);
}

//--------------------------------------------------------------------------------------------------

// Convert ResultList to (QStringList, MatchList)
inline void convert(const Lvk::Nlp::ResultList &results, QStringList &responses,
                    Lvk::Nlp::Engine::MatchList &matches)
{
    foreach (const Lvk::Nlp::Result &r, results) {
        responses.append(r.output);
        matches.append(Lvk::Nlp::Engine::RuleMatch(r.ruleId, r.inputIdx));
    }
}

} // namespace

//--------------------------------------------------------------------------------------------------
// Cb2Engine
//--------------------------------------------------------------------------------------------------

Lvk::Nlp::Cb2Engine::Cb2Engine()
    : m_logFile(new QFile()),
      m_mutex(new QMutex(QMutex::Recursive)),
      m_dirty(false),
      m_preferCurTopic(false)
{
    initLog();
}

//--------------------------------------------------------------------------------------------------

Lvk::Nlp::Cb2Engine::Cb2Engine(Sanitizer *sanitizer)
    : m_logFile(new QFile()),
      m_mutex(new QMutex(QMutex::Recursive)),
      m_dirty(false),
      m_preferCurTopic(false)
{
    Nlp::GlobalTools::instance()->setPreSanitizer(sanitizer);

    initLog();
}

//--------------------------------------------------------------------------------------------------

Lvk::Nlp::Cb2Engine::Cb2Engine(Sanitizer *preSanitizer, Lemmatizer *lemmatizer,
                               Sanitizer *postSanitizer)
    : m_logFile(new QFile()),
      m_mutex(new QMutex(QMutex::Recursive)),
      m_dirty(false),
      m_preferCurTopic(false)
{
    Nlp::GlobalTools::instance()->setPreSanitizer(preSanitizer);
    Nlp::GlobalTools::instance()->setLemmatizer(lemmatizer);
    Nlp::GlobalTools::instance()->setPostSanitizer(postSanitizer);

    initLog();
}

//--------------------------------------------------------------------------------------------------

Lvk::Nlp::Cb2Engine::~Cb2Engine()
{
    delete m_mutex;
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::Cb2Engine::initLog()
{
    Cmn::Settings settings;
    QString logsPath = settings.value(SETTING_LOGS_PATH).toString();
    QString filename = logsPath + QDir::separator() + "cb2engine.log";

    Cmn::Logger::rotateLog(filename);

    m_logFile->setFileName(filename);

    if (!m_logFile->open(QFile::Append)) {
        qCritical() << "Cb2Engine: Cannot open log file" << filename;
    }
}

//--------------------------------------------------------------------------------------------------

Lvk::Nlp::RuleList Lvk::Nlp::Cb2Engine::rules() const
{
    QMutexLocker locker(m_mutex);

    return m_rules;
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::Cb2Engine::setRules(const Lvk::Nlp::RuleList &rules)
{
    qDebug() << "Cb2Engine: Setting new rules...";

    QMutexLocker locker(m_mutex);

    m_rules = rules;

    m_dirty = true;
}

//--------------------------------------------------------------------------------------------------

QString Lvk::Nlp::Cb2Engine::getResponse(const QString &input, MatchList &matches)
{
    return getResponse(input, ANY_USER, matches);
}

//--------------------------------------------------------------------------------------------------

QString Lvk::Nlp::Cb2Engine::getResponse(const QString &input, const QString &target,
                                         MatchList &matches)
{
    matches.clear();

    MatchList allMatches;
    QStringList responses = getAllResponses(input, target, allMatches);

    if (!allMatches.empty()) {
        matches.append(allMatches.first());

        return responses.first();
    } else {
        return "";
    }
}

//--------------------------------------------------------------------------------------------------

QStringList Lvk::Nlp::Cb2Engine::getAllResponses(const QString &input, MatchList &matches)
{
    return getAllResponses(input, ANY_USER, matches);
}

//--------------------------------------------------------------------------------------------------

QStringList Lvk::Nlp::Cb2Engine::getAllResponses(const QString &input, const QString &target,
                                                  MatchList &matches)
{
    QMutexLocker locker(m_mutex);

    if (m_dirty) {
        qDebug("Cb2Engine: Dirty flag set. Refreshing trees...");
        refresh();
        m_dirty = false;
    }

    qDebug() << "Cb2Engine: Getting response for input" << input
             << "and target" << target << "...";

    Nlp::ResultList results;

    // If no response found with the given target, fallback to rules with any user
    getAllResponsesWithTree(target, input, results);
    if (results.isEmpty() && target != ANY_USER) {
        getAllResponsesWithTree(ANY_USER, input, results);
    }

    // If rules with current topic are prefered: Reorder results and update current topic
    if (m_preferCurTopic && !results.isEmpty()) {
        QString &topic = m_topics[target];
        reorderByTopic(topic, results);
        topic = topicForRule(results[0].ruleId);
    }

    // TODO Avoid this convertion. In the future remove MatchList and use only ResultList
    QStringList responses;
    convert(results, responses, matches);
    qDebug() << "Cb2Engine: Responses found: " << responses;

    return responses;
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::Cb2Engine::getAllResponsesWithTree(const QString &treeName, const QString &input,
                                                  Nlp::ResultList &results)
{
    results.clear();

    qDebug() << "Cb2Engine: Searching tree with name" << treeName;

    TreesMap::iterator it = m_trees.find(treeName);
    if (it != m_trees.end()) {
        qDebug() << "Cb2Engine: Found!";
        (*it)->getResponses(input, results);
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::Cb2Engine::refresh()
{
    m_trees.clear();

    // Initialize tree for rules without targets

    m_trees[ANY_USER] = makeSharedPtr(buildTree(ANY_USER));

    // Initialize tree for each different target

    foreach (const Nlp::Rule &rule, m_rules) {
        foreach (const QString &target, rule.target()) {
            if (!m_trees.contains(target)) {
                m_trees[target] = makeSharedPtr(buildTree(target));
            }
        }
    }
}

//--------------------------------------------------------------------------------------------------

Lvk::Nlp::Tree * Lvk::Nlp::Cb2Engine::buildTree(const QString &target)
{
    Nlp::Tree *tree = new Nlp::Tree();

    qDebug() << "Cb2Engine: Building tree for target" << target;

    for (int i = 0; i < m_rules.size(); ++i) {
        const QStringList &targetList = m_rules[i].target();
        if ((target == ANY_USER && targetList.isEmpty()) || targetList.contains(target)) {
            tree->add(m_rules[i]);
        }
    }

    return tree;
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::Cb2Engine::reorderByTopic(const QString &topic, Nlp::ResultList &results)
{
    if (topic.isEmpty()) {
        return;
    }

    for (int i = 0, j = 0; i < results.size(); ++i) {
        if (topicForRule(results[i].ruleId) == topic) {
            results.move(i, j++);
        }
    }
}

//--------------------------------------------------------------------------------------------------

QString Lvk::Nlp::Cb2Engine::topicForRule(Nlp::RuleId ruleId)
{
    foreach (const Nlp::Rule &rule, m_rules) {
        if (rule.id() == ruleId) {
            return rule.topic();
        }
    }
    return "";
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::Cb2Engine::setPreSanitizer(Lvk::Nlp::Sanitizer *sanitizer)
{
    QMutexLocker locker(m_mutex);

    Nlp::GlobalTools::instance()->setPreSanitizer(sanitizer);

    m_dirty = true;
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::Cb2Engine::setLemmatizer(Lvk::Nlp::Lemmatizer *lemmatizer)
{
    QMutexLocker locker(m_mutex);

    Nlp::GlobalTools::instance()->setLemmatizer(lemmatizer);

    m_dirty = true;
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::Cb2Engine::setPostSanitizer(Lvk::Nlp::Sanitizer *sanitizer)
{
    QMutexLocker locker(m_mutex);

    Nlp::GlobalTools::instance()->setPostSanitizer(sanitizer);

    m_dirty = true;
}

//--------------------------------------------------------------------------------------------------

QVariant Lvk::Nlp::Cb2Engine::property(const QString &name)
{
    if (name == NLP_PROP_PREFER_CUR_TOPIC) {
        return QVariant(m_preferCurTopic);
    } else {
        return QVariant();
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::Cb2Engine::setProperty(const QString &name, const QVariant &value)
{
    if (name == NLP_PROP_PREFER_CUR_TOPIC) {
        QMutexLocker locker(m_mutex);

        if (value.toBool() == true && !m_preferCurTopic) {
            qDebug() << "Cb2Engine: Enabled topics";
            m_preferCurTopic = true;
        }
        if (value.toBool() == false && m_preferCurTopic) {
            qDebug() << "Cb2Engine: Disabled topics";
            m_preferCurTopic = false;
        }
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::Cb2Engine::clear()
{
    QMutexLocker locker(m_mutex);

    m_dirty = true;
    m_rules.clear();
    m_trees.clear();
    m_topics.clear();
}
