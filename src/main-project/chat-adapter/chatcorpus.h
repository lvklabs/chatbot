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
#include <QDateTime>

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
 *
 * TODO explain log format
 */

class ChatCorpus
{
public:

    /**
     * The CorpusEntry class provides a single entry of the corpus
     */
    struct CorpusEntry
    {
        QDateTime timestamp; ///< Entry timestamp
        QString thread;      ///< Conversation thread ID
        QString username;    ///< Username
        QString message;     ///< Message
    };

    /**
     * Constructs a ChatCorpus object initialized with the default data file.
     */
    ChatCorpus();

    /**
     * Adds the \a entry to the corpus.
     */
    void add(const CorpusEntry &entry);

    /**
     * Adds a new entry with \a username, \a message and \a thread to the corpus.
     * The timestamp is set to the current date and time.
     */
    void add(const QString &username, const QString &message, const QString &thread);

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
