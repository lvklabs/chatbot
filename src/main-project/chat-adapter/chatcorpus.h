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

#ifndef LVK_CA_CHATCORPUS_H
#define LVK_CA_CHATCORPUS_H

#include <QList>
#include <QPair>
#include <QString>
#include <QFile>

class QMutex;

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace CA
{

/// \ingroup Lvk
/// \addtogroup CA
/// @{

/**
 * \brief The ChatCorpus class provides corpus of chat conversations held by the real user.
 *
 * The ChatCorpus class provides methods to store and retrieve chat conversations held by the
 * real user (i.e. not the chabot) to be used later to train NLP algorithms. The class maintains
 * only one big corpus. It is not possible to instantiate different corpora.
 *
 * Currently, only FbChatbot class uses ChatCorpus. FbChatbot can "hear" chat conversations
 * from the real user but GtalkChatbot does not.
 */

class ChatCorpus
{
public:

    typedef QPair<QString, QString> CorpusEntry; //! CorpusEntry provides a pair (user, sentence)

    /**
     * Constructs a ChatCorpus object initialized with the default data file.
     */
    ChatCorpus();

    /**
     * Adds a new entry with the given user name and sentence to the corpus.
     */
    void add(const QString &user, const QString &sentence);

    /**
     * Adds the given entry to the corpus.
     */
    void add(const CorpusEntry &entry);

    /**
     * Returns all the entries in the corpus.
     */
    QList<CorpusEntry> corpus();

private:
    ChatCorpus(const ChatCorpus&);
    ChatCorpus& operator=(const ChatCorpus&);

    static bool m_init;
    static QFile m_corpusFile;
    static QList<CorpusEntry> m_corpus;
    static QMutex *m_mutex;

    void init();
    void load();
};

/// @}

} // namespace CA

/// @}

} // namespace Lvk

#endif // LVK_CA_CHATCORPUS_H
