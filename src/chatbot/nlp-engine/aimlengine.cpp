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

#include "nlp-engine/aimlengine.h"
#include "nlp-engine/rule.h"
#include "nlp-engine/nullsanitizer.h"
#include "nlp-engine/nulllemmatizer.h"
#include "common/settings.h"
#include "common/settingskeys.h"
#include "common/logger.h"

#include <QStringList>
#include <QFile>
#include <QDir>
#include <QMutex>
#include <QMutexLocker>
#include <QtDebug>
#include <cassert>

#define ANY_USER    ""

//--------------------------------------------------------------------------------------------------
// Helpers
//--------------------------------------------------------------------------------------------------

namespace
{

// getCategoryId provide a simple way to map two numbers in one unique number
// getRuleId and getInputNumber retrieve the original values
// Asumming sizeof(long) == 8, that's true for most modern archs
// 2^12 = 4K input rules should be far enough and we have 2^52 different rules ids

const int  INPUT_NUMBER_BITS = 12; 
const long INPUT_NUMBER_MASK = 0xfffl;

//--------------------------------------------------------------------------------------------------

inline long getCategoryId(Lvk::Nlp::RuleId ruleId, int inputNumber)
{
    return ((long)ruleId << INPUT_NUMBER_BITS) | (long)inputNumber;
}

//--------------------------------------------------------------------------------------------------

inline Lvk::Nlp::RuleId getRuleId(long categoryId)
{
    return categoryId >> INPUT_NUMBER_BITS;
}

//--------------------------------------------------------------------------------------------------

inline int getInputNumber(long categoryId)
{
    return categoryId & INPUT_NUMBER_MASK;
}

//--------------------------------------------------------------------------------------------------

// convert IDs returned by AIMLParser to Nlp::Engine::MatchList
inline void convert(Lvk::Nlp::Engine::MatchList &matches, const QList<long> &categoriesId)
{
    matches.clear();

    if (categoriesId.size() > 0) {
        long catId = categoriesId.last();
        matches.append(QPair<Lvk::Nlp::RuleId, int>(getRuleId(catId), getInputNumber(catId)));
    }
}

//--------------------------------------------------------------------------------------------------

// "std::make_ptr"-like function to construct QSharedPointers
template<class T>
inline QSharedPointer<T> makeSharedPtr(T *p)
{
    return QSharedPointer<T>(p);
}

} // namespace

//--------------------------------------------------------------------------------------------------
// AimlEngine
//--------------------------------------------------------------------------------------------------

Lvk::Nlp::AimlEngine::AimlEngine()
    : m_preSanitizer(new Nlp::NullSanitizer()),
      m_postSanitizer(new Nlp::NullSanitizer()),
      m_lemmatizer(new Nlp::NullLemmatizer()),
      m_logFile(new QFile()),
      m_mutex(new QMutex(QMutex::Recursive)),
      m_dirty(false)
{
    initLog();
}

//--------------------------------------------------------------------------------------------------

Lvk::Nlp::AimlEngine::AimlEngine(Sanitizer *sanitizer)
    : m_preSanitizer(sanitizer),
      m_postSanitizer(new Nlp::NullSanitizer()),
      m_lemmatizer(new Nlp::NullLemmatizer()),
      m_logFile(new QFile()),
      m_mutex(new QMutex(QMutex::Recursive)),
      m_dirty(false)
{
    initLog();
}

//--------------------------------------------------------------------------------------------------

Lvk::Nlp::AimlEngine::AimlEngine(Sanitizer *preSanitizer, Lemmatizer *lemmatizer,
                                 Sanitizer *postSanitizer)
    : m_preSanitizer(preSanitizer),
      m_postSanitizer(postSanitizer),
      m_lemmatizer(lemmatizer),
      m_logFile(new QFile()),
      m_mutex(new QMutex(QMutex::Recursive)),
      m_dirty(false)
{
    initLog();
}

//--------------------------------------------------------------------------------------------------

Lvk::Nlp::AimlEngine::~AimlEngine()
{
    delete m_mutex;
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::AimlEngine::initLog()
{
    Cmn::Settings settings;
    QString logsPath = settings.value(SETTING_LOGS_PATH).toString();
    QString filename = logsPath + QDir::separator() + "aiml_parser.log";

    Cmn::Logger::rotateLog(filename);

    m_logFile->setFileName(filename);

    if (!m_logFile->open(QFile::Append)) {
        qCritical() << "AimlEngine: Cannot open log file" << filename;
    }
}

//--------------------------------------------------------------------------------------------------

Lvk::Nlp::RuleList Lvk::Nlp::AimlEngine::rules() const
{
    QMutexLocker locker(m_mutex);

    return m_rules;
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::AimlEngine::setRules(const Lvk::Nlp::RuleList &rules)
{
    qDebug() << "AimlEngine: Setting new AIML rules...";

    QMutexLocker locker(m_mutex);

    m_rules = rules;

    m_dirty = true;
}

//--------------------------------------------------------------------------------------------------

QString Lvk::Nlp::AimlEngine::getResponse(const QString &input, MatchList &matches)
{
    return getResponse(input, ANY_USER, matches);
}

//--------------------------------------------------------------------------------------------------

QString Lvk::Nlp::AimlEngine::getResponse(const QString &input, const QString &target,
                                          MatchList &matches)
{
    matches.clear();

    MatchList allMatches;
    QStringList responses = getAllResponses(input, target, allMatches);

    if (!allMatches.empty()) {
        matches.append(allMatches.first());

        assert(!responses.empty());
        return responses.first();
    } else {
        return "";
    }
}

//--------------------------------------------------------------------------------------------------

QStringList Lvk::Nlp::AimlEngine::getAllResponses(const QString &input, MatchList &matches)
{
    return getAllResponses(input, ANY_USER, matches, true);
}

//--------------------------------------------------------------------------------------------------

QStringList Lvk::Nlp::AimlEngine::getAllResponses(const QString &input, const QString &target,
                                                  MatchList &matches)
{
    return getAllResponses(input, target, matches, true);
}

//--------------------------------------------------------------------------------------------------

inline QStringList Lvk::Nlp::AimlEngine::getAllResponses(const QString &input,
                                                         const QString &target,
                                                         MatchList &matches, bool norm)
{
    QMutexLocker locker(m_mutex);

    if (m_dirty) {
        qDebug("AimlEngine: Dirty flag set. Refreshing AIML rules...");
        refreshAiml();
        m_dirty = false;
    }

    qDebug() << "AimlEngine: Getting response for input" << input
             << "and target" << target << "...";

    QString normInput = input;

    if (norm) {
        normalize(normInput);
    }

    QString response;
    QList<long> categoriesId;

    ParsersMap::iterator it = m_parsers.find(target);
    if (it != m_parsers.end()) {
        response = (*it)->getResponse(normInput, categoriesId);
    }

    QStringList responses;

    if (response != "Internal Error!" && categoriesId.size() > 0) { // FIXME harcoded string
        responses.append(response);
        convert(matches, categoriesId);
    } else if (target != ANY_USER) {
        // No response found with the given target, fallback to rules with any user:
        return getAllResponses(normInput, ANY_USER, matches, false);
    }

    qDebug() << "AimlEngine: Responses found: " << responses;

    return responses;
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::AimlEngine::refreshAiml()
{
    QString aiml;

    m_parsers.clear();

    // Initialize AIML parser for rules without targets

    buildAiml(aiml, ANY_USER);
    m_parsers[ANY_USER] = makeSharedPtr(new AIMLParser(aiml, m_logFile.get()));

    // Initialize AIML parser for each different target

    foreach (const Nlp::Rule &rule, m_rules) {
        foreach (const QString &target, rule.target()) {
            if (!m_parsers.contains(target)) {
                buildAiml(aiml, target);
                m_parsers[target] = makeSharedPtr(new AIMLParser(aiml, m_logFile.get()));
            }
        }
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::AimlEngine::buildAiml(QString &aiml, const QString &target)
{
    aiml = "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>";
    aiml += "<aiml>";

    // Build AIML for rules that match the given target

    for (int i = 0; i < m_rules.size(); ++i) {
        const QStringList &targetList = m_rules[i].target();
        if ((target == ANY_USER && targetList.isEmpty()) || targetList.contains(target)) {
            buildAiml(aiml, m_rules[i]);
        }
    }

    aiml += "</aiml>";
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::AimlEngine::buildAiml(QString &aiml, const Rule &rule)
{
    QStringList input = rule.input();
    const QStringList &output = rule.output();

    normalize(input);

    for (int i = 0; i < input.size(); ++i) {
        // id is not part of AIML standar. It's an LVK extension to know which
        // rule has matched
        QString categoryId = QString::number(getCategoryId(rule.id(), i));

        aiml += "<category>";
        aiml += "<id>" + categoryId + "</id>";
        aiml += "<pattern>" + input[i] + "</pattern>";

        if (output.size() == 1) {
            aiml += "<template>" + output[0] + "</template>";
        } else if (output.size() > 1) {
            aiml += "<template><random>";
            for (int j = 0; j < output.size(); ++j) {
                aiml += "<li>" + output[j] + "</li>";
            }
            aiml += "</random></template>";
        }

        aiml += "</category>";
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::AimlEngine::normalize(QStringList &inputList)
{
    for (int i = 0; i < inputList.size(); ++i) {
        normalize(inputList[i]);
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::AimlEngine::normalize(QString &input)
{
    qDebug() << " - Normalizing input" << input;

    input = m_preSanitizer->sanitize(input);
    input = m_lemmatizer->lemmatize(input);
    input = m_postSanitizer->sanitize(input);
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::AimlEngine::setPreSanitizer(Lvk::Nlp::Sanitizer *sanitizer)
{
    QMutexLocker locker(m_mutex);

    m_preSanitizer.reset(sanitizer ? sanitizer : new NullSanitizer());
    m_dirty = true;
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::AimlEngine::setLemmatizer(Lvk::Nlp::Lemmatizer *lemmatizer)
{
    QMutexLocker locker(m_mutex);

    m_lemmatizer.reset(lemmatizer ? lemmatizer : new NullLemmatizer());
    m_dirty = true;
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::AimlEngine::setPostSanitizer(Lvk::Nlp::Sanitizer *sanitizer)
{
    QMutexLocker locker(m_mutex);

    m_postSanitizer.reset(sanitizer ? sanitizer : new NullSanitizer);
    m_dirty = true;
}

