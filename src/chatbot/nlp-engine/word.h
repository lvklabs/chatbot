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

#ifndef LVK_NLP_WORD_H
#define LVK_NLP_WORD_H

#include <QString>
#include <QDebug>
#include <QRegExp>
#include <QStringList>
#include <QList>
#include <QDataStream>
#include <QMetaType>

#include "nlp-engine/syntax.h"

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
 * \brief The Word class provides information about a word such as lemma, PoS tag,
 *        alternative spells, etc.
 *
 * The Word class is used by Lemmatizer's to return the information about a sentence
 */
class Word
{
public:

    /**
     * Consructs a Word object with original word \a origWord, normalized word \a normWord and
     * lemma \a lemma
     */
    Word(const QString origWord = "", const QString normWord = "", const QString lemma = "")
        : origWord(origWord), normWord(normWord), lemma(lemma) { }

    QString origWord; ///< The original word
    QString normWord; ///< The normalized form of the original word
    QString lemma;    ///< Word lemma
    QString posTag;   ///< Word PoS tag
    QStringList altSpells; ///< Alternative spellings for the original word

    /**
     * Returns true if \a this is equal to \a other. Otherwise; returns false.
     */
    bool operator==(const Word &other) const
    {
        return origWord == other.origWord &&
                normWord == other.normWord &&
                lemma == other.lemma &&
                posTag == other.posTag &&
                altSpells == other.altSpells;
    }

    /**
     * Returns true if \a this is *not* equal to \a other. Otherwise; returns false.
     */
    bool operator!=(const Word &other) const
    {
        return !this->operator==(other);
    }

    /**
     * Returns true if \a this is the star operator. Otherwise; returns false.
     */
    bool isStar() const
    {
        return origWord == STAR_OP ;
    }

    /**
     * Returns true if \a this is the plus operator. Otherwise; returns false.
     */
    bool isPlus() const
    {
        return origWord == PLUS_OP;
    }

    /**
     * Returns true if \a this is a wildcard operator. Otherwise; returns false.
     */
    bool isWildcard() const
    {
        return isPlus() || isStar();
    }

    /**
     * Returns true if \a this is a variable. Otherwise; returns false.
     */
    bool isVariable() const
    {
        return QRegExp(VAR_DECL_REGEX).exactMatch(origWord);
    }

    /**
     * Returns true if \a this is a symbol. Otherwise; returns false.
     */
    bool isSymbol() const
    {
        return !isWildcard() && origWord.size() == 1 && !origWord[0].isLetterOrNumber();
    }

    /**
     * Returns true if \a this is a word. Otherwise; returns false.
     */
    bool isWord() const
    {
        return !isWildcard() && !isSymbol() && !isVariable();
    }
};


/**
 * This method adds support to print debug information of Word classes
 */
inline QDebug& operator<<(QDebug& dbg, const Word &w)
{
    dbg.space() << w.origWord << w.normWord << w.lemma << w.posTag /*<< w.altSpells*/;

    return dbg.maybeSpace();
}


/**
 * The WordList class provides a list of Word's
 */
typedef QList<Word> WordList;

/// @}

} // namespace Nlp

/// @}

} // namespace Lvk

#endif // LVK_NLP_WORD_H

