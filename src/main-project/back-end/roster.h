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

#ifndef LVK_BE_ROSTER_H
#define LVK_BE_ROSTER_H

#include <QString>
#include <QList>
#include <QMetaType>

namespace Lvk
{

namespace BE
{


/**
 * \brief RosterItem class provides information about a user in the roster
 */
class RosterItem
{
public:
    RosterItem() { }

    RosterItem(QString username, QString fullname) : username(username), fullname(fullname) { }

    QString username; //! Username used to log in the chat service
    QString fullname; //! Full name

    QDataStream &operator<<(QDataStream &out) { return out << username << fullname; }

    QDataStream &operator>>(QDataStream &in) { return in >> username >> fullname; }
};

/**
 * \brief The Roster class provides a list of roster items
 */
typedef QList<RosterItem> Roster;

} //namespace BE

} //namespace Lvk


Q_DECLARE_METATYPE(Lvk::BE::RosterItem)
Q_DECLARE_METATYPE(Lvk::BE::Roster)


#endif // LVK_BE_ROSTER_H
