/*
 * Copyright (C) 2012 Andres Pagliano, Gabriel Miretti, Gonzalo Buteler,
 * Nestor Bustamante, Pablo Perez de Angelis
 *
 * This file is part of LVK Botmaster.
 *
 * LVK Botmaster is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * LVK Botmaster is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LVK Botmaster.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef LVK_CA_CHATCORPUS_H
#define LVK_CA_CHATCORPUS_H

#include <QList>
#include <QPair>
#include <QString>
#include <QFile>

namespace Lvk
{

namespace CA
{

/**
 * \brief The ChatCorpus class provides corpus of chat conversations held by the real user.
 *
 * The ChatCorpus class stores and retrieves chat conversations held by the real user (i.e. not the
 * chabot) to be used later (mainly) in NLP algorithms. The class maintains only one big corpus.
 * Currently only FbChatbot class uses ChatCorpus. FbChatbot can "sniff" the real user and it
 * stores sniffed conversations.
 */

class ChatCorpus
{
public:
    ChatCorpus();

    typedef QPair<QString, QString> CorpusEntry; //! CorpusEntry provides a pair (user, sentence)

    void add(const QString &user, const QString &sentence);

    void add(const CorpusEntry &entry);

    const QList<CorpusEntry> &corpus();

private:
    static bool m_init;
    static QFile m_corpusFile;
    static QList<CorpusEntry> m_corpus;

    void load();
};

} // namespace CA

} // namespace Lvk

#endif // LVK_CA_CHATCORPUS_H
