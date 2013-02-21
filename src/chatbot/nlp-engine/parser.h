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

#ifndef LVK_NLP_PARSER_H
#define LVK_NLP_PARSER_H

#include <QRegExp>

#include "nlp-engine/predicate.h"

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace Nlp
{

/// \ingroup Lvk
/// \addtogroup Nlp
/// @{

/**
 * \brief The Parser class provides methods to parse the engine syntax
 */
class Parser
{
public:

    /**
     * Constructs a Parser object
     */
    Parser();

    /**
     * Parses string \a s starting from position \a offset searching for a variable declaration.
     * If the declaration is found, it returns the index position of the  declaration
     * in \a s and sets \a varName and \a recursive. Otherwise; returns -1
     */
    int parseVariable(const QString &s, QString *varName = 0, bool *recursive = 0, int offset = 0);

    /**
     * Parses string \a s starting from position \a offset searching for a 'if' declaration.
     * If the declaration is found, it returns the index position of the declaration
     * in \a s and sets \a predicate and \a body. Otherwise; returns -1
     */
    int parseIf(const QString &s, Nlp::Predicate **pred = 0, QString *body = 0, int offset = 0);

    /**
     * Parses string \a s starting from position \a offset searching for a 'else' declaration.
     * If the declaration is found, it returns the index position of the declaration
     * in \a s and sets \a body. Otherwise; returns -1
     */
    int parseElse(const QString &s, QString *body = 0, int offset = 0);

private:
    QRegExp m_varRegex;
    QRegExp m_ifRegex;
    QRegExp m_elseRegex;

    void initRegexps();
    Predicate * parsePredicate(const QString &c1, const QString &c2, const QString &comp);
};

/// @}

} // namespace Nlp

/// @}

} // namespace Lvk


#endif // LVK_NLP_PARSER_H

