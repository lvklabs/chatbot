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

#ifndef LVK_BE_CONVERSATION_H
#define LVK_BE_CONVERSATION_H

#include <QDateTime>
#include <QString>
#include <QList>

namespace Lvk
{

namespace BE

{

/**
 * \brief The Conversation Class provides a conversation held between a user and the chatbot
 */

class Conversation
{
public:
    Conversation();
    Conversation(const QString &str);

    /**
     * \brief The Entry Class provides a single conversation entry between a user and the chatbot
     */

    class Entry
    {
    public:
        Entry();
        Entry(const QString &str);
        Entry(const QDateTime &dateTime, const QString &from, const QString &to, const QString &msg,
              const QString &response, bool match);

        QDateTime dateTime;
        QString from;
        QString to;
        QString msg;
        QString response;
        bool match;

        bool isNull();

        QString toString() const;
    };

    void setEntries(const QList<Entry> &entries);
    QList<Entry> &entries();
    const QList<Entry> &entries() const;

    void append(const Entry &entry);

    QString toString() const;

private:
    QList<Entry> m_entries;
};

} // namespace BE

} // namespace Lvk


#endif // LVK_BE_CONVERSATION_H
