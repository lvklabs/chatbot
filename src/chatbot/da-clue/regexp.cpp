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

#include "da-clue/regexp.h"
#include "nlp-engine/enginefactory.h"
#include "nlp-engine/rule.h"

#include <QStringList>
#include <QtDebug>


//--------------------------------------------------------------------------------------------------
// RegExp
//--------------------------------------------------------------------------------------------------

Lvk::Clue::RegExp::RegExp()
    : m_engine(Nlp::EngineFactory().createEngine())
{
}

//--------------------------------------------------------------------------------------------------

Lvk::Clue::RegExp::~RegExp()
{
    delete m_engine;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::Clue::RegExp::exactMatch(const QString &pattern, const QString &str)
{
    qDebug() << "RegExp: exactMatch:" << pattern << str;

    if (pattern.isEmpty()) {
        return false;
    }

    m_engine->clear();
    m_engine->addRule(Nlp::Rule(1, pattern.split("|"), QStringList() << "dummy"));

    Nlp::Engine::MatchList matches;
    bool match = m_engine->getResponse(str, matches).size() > 0;

    qDebug() << "RegExp: Match?" << match;

    return match;
}
