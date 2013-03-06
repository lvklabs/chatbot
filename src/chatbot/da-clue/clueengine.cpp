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

#include "da-clue/clueengine.h"
#include "da-clue/analyzedscript.h"
#include "da-clue/script.h"
#include "nlp-engine/enginefactory.h"

#include <QtDebug>

//--------------------------------------------------------------------------------------------------
// ClueEngine
//--------------------------------------------------------------------------------------------------

Lvk::Clue::ClueEngine::ClueEngine()
    : m_engine(Nlp::EngineFactory().createEngine())
{
}

//--------------------------------------------------------------------------------------------------

Lvk::Clue::ClueEngine::~ClueEngine()
{
    delete m_engine;
}

//--------------------------------------------------------------------------------------------------

void Lvk::Clue::ClueEngine::setRules(const Nlp::RuleList &rules)
{
    m_engine->setRules(rules);
}

//--------------------------------------------------------------------------------------------------

void Lvk::Clue::ClueEngine::clear()
{
    m_engine->clear();
}

//--------------------------------------------------------------------------------------------------

void Lvk::Clue::ClueEngine::analyze(const Clue::ScriptList &scripts,
                                    Clue::AnalyzedList &ascripts)
{
    Clue::AnalyzedScript as;

    foreach (const Clue::Script &s, scripts) {
        analyze(s, as);
        ascripts.append(as);
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::Clue::ClueEngine::analyze(const Clue::Script &script, Clue::AnalyzedScript &ascript)
{
    ascript.clear();
    ascript.filename = script.filename;
    ascript.character = script.character;
    ascript.number = script.number;

    Nlp::Engine::MatchList matches;

    // FIXME not setting real score and outputIdx

    qDebug() << "ClueEngine: Analyzing script:"  << script.filename;

    foreach (const Clue::ScriptLine &line, script) {
        qDebug() << "ClueEngine: Getting response for question:"  << line.question;

        QString resp = m_engine->getResponse(line.question, matches);

        if (matches.isEmpty()) {
            qDebug() << "ClueEngine: no response!";
            ascript.append(Clue::AnalyzedLine(line));
        } else {
            qDebug() << "ClueEngine: Checking response:" << resp
                     << "with expected pattern:" << line.expAnswer
                     << "and forbidden pattern:" << line.forbidAnswer;
            ascript.append(Clue::AnalyzedLine(line, matches[0].first, matches[0].second, 0, resp));

            if (m_regexp.exactMatch(line.expAnswer, resp) &&
                    !m_regexp.exactMatch(line.forbidAnswer, resp)) {
                qDebug() << "ClueEngine: pattern OK";
                ascript.last().outputIdx = 0;
            } else {
                qDebug() << "ClueEngine: pattern failed!";
            }
        }
    }
}


