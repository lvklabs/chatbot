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
// Helpers
//--------------------------------------------------------------------------------------------------

namespace
{

template<typename T1, typename T2>
Lvk::Nlp::Comparison<T1, T2> * make_comp(const T1 &t1, const T2 &t2)
{
    return new Lvk::Nlp::Comparison<T1, T2>(t1, t2);
}

} // namespace


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

int Lvk::Nlp::Parser::parseVariable(const QString &s, QString *varName, bool *recursive, int offset)
{
    int i = m_varRegex.indexIn(s, offset);

    if (i != -1) {
        qDebug() << "Parsed var" << m_ifRegex.cap(1);

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

int Lvk::Nlp::Parser::parseIf(const QString &s, Nlp::Predicate **pred, QString *body, int offset)
{
    int i = m_ifRegex.indexIn(s, offset);

    if (i != -1) {
        qDebug() << "Parsed if" << m_ifRegex.cap(1)  << m_ifRegex.cap(2)
                 << m_ifRegex.cap(3) << m_ifRegex.cap(4);

        if (pred) {
            *pred = parsePredicate(m_ifRegex.cap(1).trimmed(),
                                   m_ifRegex.cap(3).trimmed(),
                                   m_ifRegex.cap(2).trimmed());
        }
        if (body) {
            *body =  m_ifRegex.cap(4).trimmed();
        }

    }

    return i;
}

//--------------------------------------------------------------------------------------------------

int Lvk::Nlp::Parser::parseElse(const QString &s, QString *body, int offset)
{
    int i = m_elseRegex.indexIn(s, offset);

    if (i != -1) {
        qDebug() << "Parsed else" << m_ifRegex.cap(1);

        if (body) {
            *body = m_elseRegex.cap(1).trimmed();
        }
    }

    return i;
}

//--------------------------------------------------------------------------------------------------

Lvk::Nlp::Predicate * Lvk::Nlp::Parser::parsePredicate(const QString &c1, const QString &c2,
                                                       const QString &/*comp*/)
{
    // TODO set comparison type

    QString varName1;
    QString varName2;

    if (parseVariable(c1, &varName1) != -1) {
        if (parseVariable(c2, &varName2) != -1) {
            return make_comp(Nlp::Variable(varName1), Nlp::Variable(varName2));
        } else {
            return make_comp(Nlp::Variable(varName1), c2);
        }
    } else {
        if (parseVariable(c2, &varName2) != -1) {
            return make_comp(c1, Nlp::Variable(varName2));
        } else {
            return make_comp(c1, c2);
        }
    }
}

