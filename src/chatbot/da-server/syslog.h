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

#ifndef LVK_DAS_SYSLOG_H
#define LVK_DAS_SYSLOG_H

#include <QByteArray>

class QString;

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace DAS
{

/// \ingroup Lvk
/// \addtogroup DAS
/// @{

/**
 * \brief The Syslog class provides a Syslog message.
 *
 * Syslog is a standard for computer data logging.
 */

class Syslog
{
public:

    /**
     * Constructs a Syslog message with message \a msg
     */
    Syslog(const QString &msg);

    /**
     * Returns a byte array containing the Syslog message
     */
    const QByteArray& data() const;

private:
    QByteArray m_data;

};

/// @}

} // namespace DAS

/// @}

} // namespace Lvk


#endif // LVK_DAS_SYSLOG_H

