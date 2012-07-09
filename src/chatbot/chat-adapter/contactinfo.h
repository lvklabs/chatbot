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

#ifndef LVK_CA_CONTACTINFO_H
#define LVK_CA_CONTACTINFO_H

#include <QString>
#include <QList>
#include <QDataStream>

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
 * \brief The ContactInfo class provides information about a chat contact
 */
struct ContactInfo
{
    /**
     * Constructs an empty ContactInfo object
     */
    ContactInfo() {}

    /**
     * Constructs a ContactInfo object with \a username and \a fullname
     */
    ContactInfo(QString username, QString fullname) : username(username), fullname(fullname) {}

    QString username; //! Username used to log in the chat service
    QString fullname; //! Full name

    /**
     * Returns true if all class attributes are null
     */
    bool isNull() { return username.isNull() && fullname.isNull(); }

};

/**
 *  Writes the contact information \a info to the specified \a out stream.
 */
inline QDataStream &operator<<(QDataStream &out, const Lvk::CA::ContactInfo &info)
{
    return out << info.username << info.fullname;
}

/**
 *  Reads from the specified \a in stream into the contact information \a info.
 */
inline QDataStream &operator>>(QDataStream &in, Lvk::CA::ContactInfo &info)
{
    return in >> info.username >> info.fullname;
}

/**
 * \brief The ContactInfoList class provides a list of contact information
 */
typedef QList<ContactInfo> ContactInfoList;

/// @}

} // namespace CA

/// @}

} // namespace Lvk

#endif // LVK_CA_CONTACTINFO_H
