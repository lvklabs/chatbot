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

#ifndef LVK_FE_ROSTERHELPER_H
#define LVK_FE_ROSTERHELPER_H

#include <QString>
#include "back-end/roster.h"

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace FE
{

/// \ingroup Lvk
/// \addtogroup FE
/// @{

/**
 * Roster types
 */
enum RosterType
{
    FullRoster, ///< Full contact list
    BlackRoster ///< Contacts not allowed to talk with the chatbot
};

/**
 * \brief The RosterHelper class provides helper methods to load and save for the given a account
 *        name
 */
class RosterHelper
{
public:

    /**
     * Constructs a RosterHelper with the given \a type and
     */
    RosterHelper(RosterType type, const QString &account);

    /**
     * Saves \a roster for the current account. Returns true on success. Otherwise, returns false.
     */
    bool save(const Lvk::BE::Roster &roster);

    /**
     * Loads the roster for the current account into  \a roster. Returns true on success,
     * false otherwise.
     */
    bool load(Lvk::BE::Roster &roster);

    /**
     * Returns true if exists a roster file for the current account. Otherwise, returns false.
     */
    bool exists();

    /**
     * Removes the roster file for the current account. Returns true on success. Otherwise,
     * returns false.
     */
    bool remove();

private:
    RosterType m_type;
    QString m_account;
    QString m_filename;

    bool saveFile(const Lvk::BE::Roster &roster, const QString &filename);
    bool loadFile(Lvk::BE::Roster &roster, const QString &filename);
};

/// @}

} // namespace FE

/// @}

} // namespace Lvk


#endif // LVK_FE_ROSTERHELPER_H

