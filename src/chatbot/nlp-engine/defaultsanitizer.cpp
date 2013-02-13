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

#include "nlp-engine/defaultsanitizer.h"

#include <QtDebug>

#define utf8_a_acute                    "\xc3\xa1"
#define utf8_e_acute                    "\xc3\xa9"
#define utf8_i_acute                    "\xc3\xad"
#define utf8_o_acute                    "\xc3\xb3"
#define utf8_u_acute                    "\xc3\xba"
#define utf8_A_acute                    "\xc3\x81"
#define utf8_E_acute                    "\xc3\x89"
#define utf8_I_acute                    "\xc3\x8d"
#define utf8_O_acute                    "\xc3\x93"
#define utf8_U_acute                    "\xc3\x9a"
#define utf8_u_diaeresis                "\xc3\xbc"
#define utf8_U_diaeresis                "\xc3\x9c"
#define utf8_inverted_exclamation_mark  "\xc2\xa1"
#define utf8_inverted_question_mark     "\xc2\xbf"


//--------------------------------------------------------------------------------------------------
// DefaultSanitizer
//--------------------------------------------------------------------------------------------------

Lvk::Nlp::DefaultSanitizer::DefaultSanitizer()
    : m_options(RemoveDiacritic | RemovePunctuation | RemoveDupChars | RemoveBraces),
      m_logEnabled(false)
{
    initSets();
}

//--------------------------------------------------------------------------------------------------

Lvk::Nlp::DefaultSanitizer::DefaultSanitizer(unsigned options)
    : m_options(options), m_logEnabled(false)
{
    initSets();
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::DefaultSanitizer::initSets()
{
    // Punctuation chars to be removed
    m_punctSet.insert(',');
    m_punctSet.insert(';');
    m_punctSet.insert('.');
    m_punctSet.insert('!');
    m_punctSet.insert('?');
    m_punctSet.insert(QString::fromUtf8(utf8_inverted_exclamation_mark)[0]);
    m_punctSet.insert(QString::fromUtf8(utf8_inverted_question_mark)[0]);

    // Map of vowels with diacritic to vowels without
    // NOTE: this maps only makes sense for Spanish language.
    m_diacMap.insert(QString::fromUtf8(utf8_a_acute)[0],     'a');
    m_diacMap.insert(QString::fromUtf8(utf8_e_acute)[0],     'e');
    m_diacMap.insert(QString::fromUtf8(utf8_i_acute)[0],     'i');
    m_diacMap.insert(QString::fromUtf8(utf8_o_acute)[0],     'o');
    m_diacMap.insert(QString::fromUtf8(utf8_u_acute)[0],     'u');
    m_diacMap.insert(QString::fromUtf8(utf8_A_acute)[0],     'A');
    m_diacMap.insert(QString::fromUtf8(utf8_E_acute)[0],     'E');
    m_diacMap.insert(QString::fromUtf8(utf8_I_acute)[0],     'I');
    m_diacMap.insert(QString::fromUtf8(utf8_O_acute)[0],     'O');
    m_diacMap.insert(QString::fromUtf8(utf8_U_acute)[0],     'U');
    m_diacMap.insert(QString::fromUtf8(utf8_u_diaeresis)[0], 'u');
    m_diacMap.insert(QString::fromUtf8(utf8_U_diaeresis)[0], 'U');

    // Chars allowed to repeat once
    // NOTE: this set only makes sense for Spanish language.
    m_rSet.insert('r');
    m_rSet.insert('l');
    m_rSet.insert('n');
    m_rSet.insert('c');
    m_rSet.insert('z');
    //m_rSet.insert('o');

    // Braces to be removed
    m_bracesSet.insert('{');
    m_bracesSet.insert('}');
    m_bracesSet.insert('[');
    m_bracesSet.insert(']');
    m_bracesSet.insert('(');
    m_bracesSet.insert(')');
}

//--------------------------------------------------------------------------------------------------

QString Lvk::Nlp::DefaultSanitizer::sanitize(const QString &str) const
{
    if (str.isEmpty()) {
        return str;
    }

    int rcount = 0;     // repeat count
    QChar prev;         // previous char
    QChar cur;          // current char
    QString szStr;      // Sanitized string

    for (int i = 0; i < str.size(); ++i) {
        prev = i > 0 ? str[i-1] : QChar();
        cur = str[i];

        bool append = true;

        //-------------------------------------------------------------------------------
        // Removed dup chars
        //
        // NOTE: this implementation only makes sense for Spanish language.

        if ((m_options & RemoveDupChars) && i > 0) {

             if (cur.isLetter() && cur.toLower() == prev.toLower()) {
                ++rcount;
             } else {
                 rcount = 0;
             }

             append = rcount == 0 || (rcount == 1 && m_rSet.contains(cur));

             if (!append) {
                 continue;
             }
        }

        //-------------------------------------------------------------------------------
        // Remove punctuation

        if (m_options & RemovePunctuation) {
            append = !m_punctSet.contains(cur);

            if (!append) {
                continue;
            }
        }

        //-------------------------------------------------------------------------------
        // Remove diacritic

        if (m_options & RemoveDiacritic) {
            QHash<QChar,QChar>::const_iterator it = m_diacMap.find(cur);

            if (it != m_diacMap.constEnd()) {
                cur = *it;
            }
        }

        //-------------------------------------------------------------------------------
        // Remove braces

        if (m_options & RemoveBraces) {
            append = !m_bracesSet.contains(cur);

            if (!append) {
                continue;
            }
        }

        //-------------------------------------------------------------------------------
        // Remove double quotes

        if (m_options & RemoveDoubleQuotes) {
            append = cur != '"';

            if (!append) {
                continue;
            }
        }

        //-------------------------------------------------------------------------------
        // Lower case letters

        if (m_options & LowerCase) {
            cur = cur.toLower();
        }

        //-------------------------------------------------------------------------------

        szStr.append(cur);
    }

    if (m_logEnabled) {
        qDebug() << "   - Sanitized:" << str << "->" << szStr;
    }

    return szStr;
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::DefaultSanitizer::setLogEnabled(bool enabled)
{
    m_logEnabled = enabled;
}
