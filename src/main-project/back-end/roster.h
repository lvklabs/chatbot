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

#ifndef LVK_BE_ROSTER_H
#define LVK_BE_ROSTER_H

#include <QString>
#include <QList>
#include <QMetaType>

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace BE
{

/// \ingroup Lvk
/// \addtogroup BE
/// @{

/**
 * \brief The RosterItem class provides information about a user in the roster
 */
class RosterItem
{
public:
    RosterItem() { }

    RosterItem(QString username, QString fullname)
        : username(username), fullname(fullname)
    { }

    QString username; //! Username used to log in the chat service
    QString fullname; //! Full name

    const QString &displayText() const
    {
        return !fullname.isEmpty() ? fullname : username;
    }

    bool operator==(const RosterItem &other) const
    {
        return username == other.username && fullname == other.fullname;
    }

    bool operator!=(const RosterItem &other) const
    {
        return !this->operator==(other);
    }

    bool operator<(const RosterItem &other) const
    {
        return username < other.username;
    }
};

/**
 * \brief The Roster class provides a list of roster items
 */
typedef QList<RosterItem> Roster;

/// @}

} // namespace BE

/// @}

} // namespace Lvk


inline QDataStream &operator<<(QDataStream &out, const Lvk::BE::RosterItem &item)
{
    return out << item.username << item.fullname;
}

inline QDataStream &operator>>(QDataStream &in, Lvk::BE::RosterItem &item)
{
    return in >> item.username >> item.fullname;
}


Q_DECLARE_METATYPE(Lvk::BE::RosterItem)
Q_DECLARE_METATYPE(Lvk::BE::Roster)



#endif // LVK_BE_ROSTER_H
