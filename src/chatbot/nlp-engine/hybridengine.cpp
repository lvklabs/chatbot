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

#include "nlp-engine/hybridengine.h"
#include "nlp-engine/rule.h"
#include "nlp-engine/exactmatchengine.h"
#include "nlp-engine/nlpproperties.h"

#include <QtDebug>
#include <QMutex>
#include <QMutexLocker>
#include <cmath>
#include <exception>

//--------------------------------------------------------------------------------------------------
// HybridEngine::InvalidSyntaxException
//--------------------------------------------------------------------------------------------------

/**
 * \brief Class thrown when an invalid syntax was found while parsing rules.
 */

class Lvk::Nlp::HybridEngine::InvalidSyntaxException : public std::exception
{
public:
    InvalidSyntaxException(const QString &what) throw()
        : m_what(what.toUtf8())
    {
    }

    ~InvalidSyntaxException() throw() {}

    virtual const char* what() throw()
    {
        return m_what.constData();
    }

private:
    QByteArray m_what;
};

//--------------------------------------------------------------------------------------------------
// Helpers
//--------------------------------------------------------------------------------------------------

namespace
{

// Sanitize harmful characters that may break the engine

void sanitize(QString &s)
{
    int j = 0;
    for (int i = 0; i < s.size(); ++i) {
        if (s[i] != '<' && s[i] != '>') {
            s[j++] = s[i];
        }
    }
    s.resize(j);
}

} // namespace

//--------------------------------------------------------------------------------------------------
// HybridEngine
//--------------------------------------------------------------------------------------------------
//
// TODO this class became too complex! Time to create our own engine and stop using AIML ;)
//

#define VAR_NAME_REGEX  "\\[([A-Za-z_]+)\\]"
#define IF_REGEX        "\\{\\s*if\\s*" VAR_NAME_REGEX "\\s*=\\s*([^}]+)\\}([^{]+)(.*)"
#define ELSE_REGEX      "\\{\\s*else\\s*\\}(.+)"

#define KEYWORD_REGEX   "\\*\\*\\s*$"
#define REGEX_REGEX     "[+*]"

#define STR_ERR_1       "A rule input cannot contain two or more variables"
#define STR_ERR_2       "Rules cannot contain two or more different variable names"


Lvk::Nlp::HybridEngine::HybridEngine()
    : AimlEngine(), m_engineMutex(new QMutex()), m_emEngine(0)
{
    initRegexs();
}

//--------------------------------------------------------------------------------------------------

Lvk::Nlp::HybridEngine::HybridEngine(Sanitizer *sanitizer)
    : AimlEngine(sanitizer), m_engineMutex(new QMutex()), m_emEngine(0)
{
    initRegexs();
}

//--------------------------------------------------------------------------------------------------

Lvk::Nlp::HybridEngine::HybridEngine(Sanitizer *preSanitizer, Lemmatizer *lemmatizer,
                                     Sanitizer *postSanitizer)
    : AimlEngine(preSanitizer, lemmatizer, postSanitizer), m_engineMutex(new QMutex()),
      m_emEngine(0)
{
    initRegexs();
}

//--------------------------------------------------------------------------------------------------

Lvk::Nlp::HybridEngine::~HybridEngine()
{
    delete m_emEngine;
    delete m_engineMutex;
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::HybridEngine::initRegexs()
{
    qDebug() << "HybridEngine: Initilizing regexs...";

    QString localizedIfRegex = QString(IF_REGEX)
            .replace("if", QObject::tr("if"));

    QString localizedElseRegex  = QString(ELSE_REGEX)
            .replace("else", QObject::tr("else"));

    m_varNameRegex = QRegExp(VAR_NAME_REGEX);
    m_ifRegex = QRegExp(localizedIfRegex);
    m_elseRegex = QRegExp(localizedElseRegex);
    m_keywordRegex = QRegExp(KEYWORD_REGEX);

    m_varNameRegex.setCaseSensitive(false);
    m_ifRegex.setCaseSensitive(false);
    m_elseRegex.setCaseSensitive(false);
    m_keywordRegex.setCaseSensitive(false);
}

//--------------------------------------------------------------------------------------------------

Lvk::Nlp::RuleList Lvk::Nlp::HybridEngine::rules() const
{
    QMutexLocker locker(m_engineMutex);

    return m_rules;
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::HybridEngine::setRules(const Nlp::RuleList &rules)
{
    QMutexLocker locker(m_engineMutex);

    m_rules = rules;
    m_indexRemap.clear();

    refreshEngine();
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::HybridEngine::refreshEngine()
{
    {
        qDebug() << "HybridEngine: Converting rules to AIML format...";
        RuleList aimlRules;
        convert(aimlRules, m_rules, &HybridEngine::convertToAiml);
        AimlEngine::setRules(aimlRules);
    }

    if (m_emEngine) {
        qDebug() << "HybridEngine: Converting rules to Exact Match format...";
        RuleList emRules;
        convert(emRules, m_rules, &HybridEngine::convertToExactMatch);
        m_emEngine->setRules(emRules);
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::HybridEngine::convert(Nlp::RuleList &newRules, const Nlp::RuleList &rules,
                                         ConvertionMemb convertion)
{
    newRules.clear();

    foreach (const Nlp::Rule &rule, rules) {
        try {
            Lvk::Nlp::Rule newRule;
            (this->*convertion)(newRule, rule);
            newRules.append(newRule);
        } catch (std::exception &) {
            // Nothing to to. If the rule is invalid, we just skip it
            // TODO better error handling
        }
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::HybridEngine::convertToAiml(Nlp::Rule &newRule, const Nlp::Rule &rule)
{
    ConvertionContext ctx;
    ctx.inputIdx = 0;
    ctx.rule = rule;

    newRule.setId(rule.id());
    newRule.setTopic(rule.topic());
    newRule.setTarget(rule.target());
    convertInputList(newRule.input(), ctx);
    convertOutputList(newRule.output(), ctx);
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::HybridEngine::convertToExactMatch(Nlp::Rule &newRule, const Nlp::Rule &rule)
{
    ConvertionContext ctx;
    ctx.inputIdx = 0;
    ctx.rule = rule;

    newRule.setId(rule.id());
    newRule.setTopic(rule.topic());
    newRule.setTarget(rule.target());
    convertLiterals(newRule.input(), ctx);
    newRule.setOuput(rule.output());
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::HybridEngine::convertInputList(QStringList &inputList, ConvertionContext &ctx)
{
    /*
     * For each input, transform variables and operators.
     * More than one feature in one single input is not supported
     */

    for (int i = 0; i < ctx.rule.input().size(); ++i) {
        ctx.input = ctx.rule.input().at(i).trimmed();
        ctx.inputIdx = i;

        if (isLiteral(ctx.input)) {
            continue;
        }

        sanitize(ctx.input);

        if (hasVariable(ctx.input)) {
            convertVariables(inputList, ctx);
            continue;
        }

        if (hasKeywordOp(ctx.input)) {
            convertKeywordOp(inputList, ctx);
            continue;
        }

        convertRegexOp(inputList, ctx);
    }
}

//--------------------------------------------------------------------------------------------------

bool Lvk::Nlp::HybridEngine::hasVariable(const QString &input)
{
    return !isLiteral(input) && m_varNameRegex.indexIn(input) != -1;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::Nlp::HybridEngine::hasKeywordOp(const QString &input)
{
    return !isLiteral(input) && m_keywordRegex.indexIn(input) != -1;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::Nlp::HybridEngine::hasRegexOp(const QString &input)
{
    return !isLiteral(input) && input.contains(QRegExp(REGEX_REGEX));
}

//--------------------------------------------------------------------------------------------------

bool Lvk::Nlp::HybridEngine::hasConditional(const QString &output)
{
    return m_ifRegex.indexIn(output) != -1;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::Nlp::HybridEngine::isLiteral(const QString &input)
{
    return input.size() >= 2 && input[0] == '"' && input[input.size() - 1] == '"';
}

//--------------------------------------------------------------------------------------------------

/*
 * Transform strings like:
 *    Do you like [VarName]
 *
 * To pure AIML:
 *    Do you like *
 */

void Lvk::Nlp::HybridEngine::convertVariables(QStringList &inputList, ConvertionContext &ctx)
{
    int pos = m_varNameRegex.indexIn(ctx.input);

    if (pos != m_varNameRegex.lastIndexIn(ctx.input)) {
        throw InvalidSyntaxException(QObject::tr(STR_ERR_1));
    }
    if (!ctx.varName.isNull() && ctx.varName != m_varNameRegex.cap(1)) {
        throw InvalidSyntaxException(QObject::tr(STR_ERR_2));
    }

    ctx.varName = m_varNameRegex.cap(1);

    QString newInput = ctx.input;
    newInput.replace(m_varNameRegex, " * ");
    inputList.append(newInput);
}

//--------------------------------------------------------------------------------------------------

/*
 * Transform strings like:
 *    Footbal **
 *
 * To 4 pure AIML rules:
 *    Footbal
 *    _ Footbal
 *    Footbal _
 *    _ Footbal *
 */

void Lvk::Nlp::HybridEngine::convertKeywordOp(QStringList &inputList, ConvertionContext &ctx)
{
    QString baseInput = ctx.input;

    baseInput.remove(m_keywordRegex);

    inputList.append(baseInput);
    inputList.append(baseInput + " _");
    inputList.append("_ " + baseInput);
    inputList.append("_ " + baseInput + " *");

    RuleId id = ctx.rule.id();
    int size = inputList.size();

    m_indexRemap[id][size - 4] = ctx.inputIdx;
    m_indexRemap[id][size - 3] = ctx.inputIdx;
    m_indexRemap[id][size - 2] = ctx.inputIdx;
    m_indexRemap[id][size - 1] = ctx.inputIdx;
}

//--------------------------------------------------------------------------------------------------

/*
 * Convert regex-like operators * and + to AIML. For instance:
 *
 *
 * Transform strings like:
 *    Football +
 *
 * To pure AIML rule:
 *    Football *
 *
 *
 * Transform strings like:
 *    * Football *
 *
 * To 4 pure AIML rules:
 *    Football
 *    Football *
 *    * Football
 *    * Football *
 *
 * Note that operator * needs an exponential rule expansion!
 */

void Lvk::Nlp::HybridEngine::convertRegexOp(QStringList &inputList, ConvertionContext &ctx)
{
    RuleId id = ctx.rule.id();

    QStringList inputParts = ctx.input.split("*");

    for (int i = 0; i < inputParts.size(); ++i) {
        inputParts[i].replace('+', '*');
    }

    int variants = pow(2, inputParts.size() - 1) - 1;

    variants = std::min(variants, 15); // Limit the exponential explosion of rule variants!

    for (int v = 0; v <= variants; ++v) {
        QString newInput = inputParts[0];
        for (int i = 1; i < inputParts.size(); ++i) {
            newInput += v & (0x1 << (i - 1)) ? " * " : " ";
            newInput += inputParts[i];
        }
        inputList.append(newInput);
        m_indexRemap[id][inputList.size() - 1] = ctx.inputIdx;
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::HybridEngine::convertOutputList(QStringList &outputList, ConvertionContext &ctx)
{
    outputList.clear();

    /*
     * For each output, convert strings like:
     *    {if [VarName] = football}
     *       Yes I like [VarName]
     *    {if [VarName] = soccer}
     *       Yeap
     *    {else}
     *       No, I don't
     *
     * To pure AIML:
     *    <think><set name="like"><star/></set></think>
     *    <condition>
     *       <li name="like" value="football"> Yes I like <star/></li>
     *       <li>No, I don't</li>
     *    </condition>
     */

    for (int i = 0; i < ctx.rule.output().size(); ++i) {

        // TODO refactor!

        QString output = ctx.rule.output().at(i);

        sanitize(output);

        // Parse If-Else

        QString newOutput;
        QString tail = output;
        int pos = 0;

        while ( (pos = m_ifRegex.indexIn(tail)) != -1 ) {
            if (newOutput.isEmpty()) {
                newOutput += "<condition>";
            }
            newOutput += QString("<li name=\"%1\" value=\"%2\">%3</li>")
                                   .arg(m_ifRegex.cap(1))
                                   .arg(normalize(m_ifRegex.cap(2)).trimmed())
                                   .arg(m_ifRegex.cap(3).trimmed());

            tail = m_ifRegex.cap(4);
        }

        if (!newOutput.isEmpty()) {
            if ( (pos = m_elseRegex.indexIn(tail)) != -1 ) {
                newOutput += QString("<li>%1</li>").arg(m_elseRegex.cap(1).trimmed());
            }
            newOutput += "</condition>";
        }

        // If could not parse If-Else

        if (newOutput.isEmpty()) {
            newOutput = output;
        }

        // Parse Variables

        pos = 0;
        while ( (pos = m_varNameRegex.indexIn(output, pos)) != -1) {
            QString varName = m_varNameRegex.cap(1);

            if (varName == ctx.varName) {
                newOutput.replace("[" + varName + "]", "<star/>", Qt::CaseInsensitive);
            } else {
                newOutput.replace("[" + varName + "]", "<get name=\"" + varName + "\" />",
                                  Qt::CaseInsensitive);
            }

            pos++;
        }

        if (!ctx.varName.isNull()) {
            newOutput.prepend("<think>"
                              "<set name=\"" + ctx.varName + "\"><star/></set>"
                              "</think>");
        }

        outputList.append(newOutput);
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::HybridEngine::convertLiterals(QStringList &inputList, ConvertionContext &ctx)
{
    /*
     * For each input, transform literal strings. (i.e. strings surroanded by "...")
     */

    for (int i = 0; i < ctx.rule.input().size(); ++i) {
        QString input = ctx.rule.input().at(i).trimmed();

        if (isLiteral(input)) {
            qDebug() << "HybridEngine: Parsing exact match input:" << input;
            inputList.append(input.mid(1, input.size() - 2));
        } else {
            inputList.append(QString());
        }
    }
}

//--------------------------------------------------------------------------------------------------

QStringList Lvk::Nlp::HybridEngine::getAllResponses(const QString &input, const QString &target,
                                                        Engine::MatchList &matches)
{
    qDebug() << "HybridEngine: Getting response for input" << input << "and" << target;

    QMutexLocker locker(m_engineMutex);

    QStringList responses;

    // If exact match is enabled, try first exact match. If no responses fallback AIML engine

    if (m_emEngine) {
        responses = m_emEngine->getAllResponses(input, target, matches);

        if (responses.size() > 0) {
            qDebug() << "HybridEngine: Found exact match response" << responses;
        } else {
            qDebug() << "HybridEngine: No exact match found. Fallback to AimlEngine.";
        }
    }

    if (responses.isEmpty()) {
        responses = AimlEngine::getAllResponses(input, target, matches);
        remap(matches);
    }

    return responses;
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::HybridEngine::remap(Engine::MatchList &matches)
{
    for (int i = 0; i < matches.size(); ++i) {
        Nlp::RuleId ruleId = matches[i].first;
        int inputIndex = matches[i].second;

        RuleIndexRemap::Iterator it = m_indexRemap.find(ruleId);
        if (it != m_indexRemap.end()) {
            IndexRemap::Iterator itt = it->find(inputIndex);
            if (itt != it->end()) {
                matches[i].second = itt.value();
            }
        }
    }
}


//--------------------------------------------------------------------------------------------------

QVariant Lvk::Nlp::HybridEngine::property(const QString &name)
{
    if (name == NLP_PROP_EXACT_MATCH) {
        QMutexLocker locker(m_engineMutex);

        return m_emEngine ? QVariant(true) : QVariant(false);
    } else if (name == NLP_PROP_PREFER_CUR_TOPIC) {
        return AimlEngine::property(name);
    } else {
        return QVariant();
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::HybridEngine::setProperty(const QString &name, const QVariant &value)
{
    if (name == NLP_PROP_EXACT_MATCH) {
        QMutexLocker locker(m_engineMutex);

        if (value.toBool() == true && !m_emEngine) {
            qDebug() << "HybridEngine: Enabled exact match support";
            m_emEngine = new ExactMatchEngine();
            refreshEngine();
        }
        if (value.toBool() == false && m_emEngine) {
            qDebug() << "HybridEngine: Disabled exact match support";
            delete m_emEngine;
            m_emEngine = 0;
        }
    } else if (name == NLP_PROP_PREFER_CUR_TOPIC) {
        AimlEngine::setProperty(name, value);
    }
}


