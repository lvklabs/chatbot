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
#include <QStringList>
#include <QList>
#include <QDataStream>
#include <QMetaType>

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
 * \brief Word struct
 */
struct Word
{
    Word(const QString origWord = "") : origWord(origWord) { }

    QString origWord;
    QString normWord;
    QString lemma;
    QString posTag;
    QStringList altSpells;

    //bool operator==(const Word &other)
};


inline QDebug& operator<<(QDebug& dbg, const Word &w)
{
    dbg.space() << w.origWord << w.normWord << w.lemma << w.posTag /*<< w.altSpells*/;

    return dbg.maybeSpace();
}


typedef QList<Word> WordList;

/// @}

} // namespace Nlp

/// @}

} // namespace Lvk

#endif // LVK_NLP_WORD_H

