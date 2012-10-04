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

#ifndef LVK_CA_HISTORYHELPER_H
#define LVK_CA_HISTORYHELPER_H

#include <QString>
#include "common/conversation.h"

class QFile;
class QReadWriteLock;

namespace Lvk
{

namespace Cmn
{
    class ConversationWriter;
}

/// \addtogroup Lvk
/// @{

namespace CA
{

/// \ingroup Lvk
/// \addtogroup CA
/// @{

/**
 * \brief The HistoryHelper class provides persitent and easy way to store and retrieve chat
 *        history.
 *
 * Given a filename, the class loads the chat history. If the history does not
 * exits, it creates an empty one. All ChatHistory operations are persistent.
 *
 * This class is thread-safe.
 */
class HistoryHelper
{
public:

    /**
     * Constructs a HistoryHelper without filename.
     */
    HistoryHelper();

    /**
     * Constructs a HistoryHelper that reads and stores from \a filename
     */
    HistoryHelper(const QString &filename);

    /**
     * Destroys the object.
     */
    ~HistoryHelper();

    /**
     * Appends \a entry to the history
     */
    void append(const Cmn::Conversation::Entry &entry);

    /**
     * Returns the full chat history for the current file.
     */
    const Cmn::Conversation &history() const;

    /**
     * Sets \a conv as the chat history for the current file.
     */
    void setHistory(const Cmn::Conversation &conv);

    /**
     * Sets \a filename as the new filename to store data. If the file already exists,
     * it also loads the file contents.
     */
    void setFilename(const QString &filename);

    /**
     * Returns the current filename
     */
    QString filename() const;

    /**
     * Clears the chat history and removes any file created.
     */
    void clear();

private:
    HistoryHelper(HistoryHelper&);
    HistoryHelper& operator=(const HistoryHelper&);

    QString m_filename;
    Cmn::Conversation m_conv;
    Cmn::ConversationWriter *m_convWriter;
    QReadWriteLock *m_rwLock;

    void load();
    void resetHistoryLog();
};

/// @}

} // namespace CA

/// @}

} // namespace Lvk


#endif // LVK_CA_HISTORYHELPER_H

