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

#include "nlp-engine/parser.h"
#include "nlp-engine/syntax.h"

#include <QObject>
#include <QDebug>

//--------------------------------------------------------------------------------------------------
// Parser
//--------------------------------------------------------------------------------------------------

Lvk::Nlp::Parser::Parser()
{
    initRegexps();
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::Parser::initRegexps()
{
    m_varRegex = QRegExp(VAR_DECL_REGEX);

    QString localizedIfRegex = QString(IF_REGEX).replace("if", QObject::tr("if"));
    QString localizedElseRegex = QString(ELSE_REGEX).replace("else", QObject::tr("else"));

    m_ifRegex = QRegExp(localizedIfRegex);
    m_elseRegex = QRegExp(localizedElseRegex);

    m_ifRegex.setCaseSensitivity(Qt::CaseInsensitive);
    m_elseRegex.setCaseSensitivity(Qt::CaseInsensitive);
}

//--------------------------------------------------------------------------------------------------

int Lvk::Nlp::Parser::parseVariable(const QString &s, int offset, QString *varName, bool *recursive)
{
    int i = m_varRegex.indexIn(s, offset);

    if (i != -1) {
        if (varName) {
            *varName = m_varRegex.cap(1).trimmed();
        }
        if (recursive) {
            *recursive = i > 0 && s[i - 1].toLower() == 'r';
        }
    }

    return i;
}

//--------------------------------------------------------------------------------------------------

int Lvk::Nlp::Parser::parseIf(const QString &s, int offset, Nlp::Predicate **pred, QString *body)
{
    int i = m_ifRegex.indexIn(s, offset);

    if (i != -1) {
        if (pred) {
            *pred = buildPredicate(m_ifRegex.cap(1).trimmed(), m_ifRegex.cap(3).trimmed());
        }
        if (body) {
            *body =  m_ifRegex.cap(4).trimmed();
        }

    }

    return i;
}

//--------------------------------------------------------------------------------------------------

int Lvk::Nlp::Parser::parseElse(const QString &s, int offset, QString *body)
{
    int i = m_elseRegex.indexIn(s, offset);

    if (i != -1) {
        if (body) {
            *body = m_elseRegex.cap(1).trimmed();
        }
    }

    return i;
}

//--------------------------------------------------------------------------------------------------

Lvk::Nlp::Predicate * Lvk::Nlp::Parser::buildPredicate(const QString &c1, const QString &c2)
{
    // TODO set comparison type
    return new Nlp::Comparison<Nlp::Variable, QString>(Nlp::Variable(c1), c2);
}

