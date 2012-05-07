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

namespace Lvk
{

namespace BE
{


/**
 * \brief RosterItem struct provides information about a user in the roster
 */
struct RosterItem
{
    RosterItem() { }

    RosterItem(QString username, QString fullname) : username(username), fullname(fullname) { }

    QString username; //! Username used to log in the chat service
    QString fullname; //! Full name
};


/**
 * \brief The Roster class provides a list of roster items
 */
typedef QList<RosterItem> Roster;

} //namespace BE

} //namespace Lvk

#endif // LVK_BE_ROSTER_H
