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

#ifndef LVK_CMN_CONVERSATION_H
#define LVK_CMN_CONVERSATION_H

#include <QDateTime>
#include <QString>
#include <QList>

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace Cmn
{

/// \ingroup Lvk
/// \addtogroup Cmn
/// @{

/**
 * \brief The Conversation class provides a chat conversation between a user and the chatbot
 */

class Conversation
{
public:

    /**
     * Constructs an empty Conversation object.
     */
    Conversation();

    /**
     * \brief The Entry class provides a single conversation entry between a user and the chatbot
     *
     * A conversation entry is an object that provides information about a mesage-response
     * between the user and the chatbot such as date-time, "from" user, "to" user, message,
     * response and if there were a match in the chatbot rules.
     *
     * @see Conversation class
     */
    class Entry
    {
    public:
        /**
         * Constructs an empty Entry object.
         */
        Entry();

        /**
         * Constructs an Entry object with the given date-time, from, to, message, response,
         * match and rule ID.
         */
        Entry(const QDateTime &dateTime, const QString &from, const QString &to, const QString &msg,
              const QString &response, bool match, quint64 ruleId = 0);

        QDateTime dateTime; //! Entry date-time
        QString from;       //! From user
        QString to;         //! To user
        QString msg;        //! Message
        QString response;   //! Response
        bool match;         //! True if there were match; false otherwise.
        quint64 ruleId;     //! If match, contains the rule ID of the matched rule

        /**
         * An entry is null if all class attributes are null.
         * Returns true if this entry is null; otherwise returns false.
         */
        bool isNull() const;

        /**
         * Clears the contents of the entry.
         */
        void clear();

        /**
         * Returns true if this entry is equal to entry @param other; otherwise returns false.
         */
        bool operator==(const Entry &other) const;

        /**
         * Returns true if this entry is not equal to entry @param other; otherwise returns false.
         */
        bool operator!=(const Entry &other) const;
    };

    /**
     * Sets the given entries to the conversation.
     */
    void setEntries(const QList<Entry> &entries);

    /**
     * Returns the entries of the conversation.
     */
    QList<Entry> &entries();

    /**
     * Returns the entries of the conversation.
     */
    const QList<Entry> &entries() const;

    /**
     * Appends the given entry to the conversation.
     */
    void append(const Entry &entry);

    /**
     * Removes all entries in the conversation.
     */
    void clear();

    /**
     * Returns true if this conversation is equal to conversation @param other;
     * otherwise returns false.
     */
    bool operator==(const Conversation &other) const;

    /**
     * Returns true if this conversation is not equal to conversation @param other;
     * otherwise returns false.
     */
    bool operator!=(const Conversation &other) const;

private:
    QList<Entry> m_entries;
};

/// @}

} // namespace Cmn

/// @}

} // namespace Lvk


#endif // LVK_CMN_CONVERSATION_H
