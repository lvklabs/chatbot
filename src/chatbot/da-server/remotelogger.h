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

#ifndef LVK_DAS_REMOTELOGGER_H
#define LVK_DAS_REMOTELOGGER_H

#include <QString>
#include <QPair>
#include <QList>

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
 * \brief The RemoteLogger class provides the interface for all remote loggers.
 *
 * A remote logger is a logger that does not store messages locally. Instead it sends messages
 * to a remote server
 */
class RemoteLogger
{
public:

    /**
     * The Field class provides a (key, value) pair to set additional information besides
     * the log message
     */
    typedef QPair<QString, QString> Field;

    /**
     * The FieldList class provides a list of fields
     */
    typedef QList<Field> FieldList;

    /**
     * Destroys the object.
     */
    virtual ~RemoteLogger() {}

    /**
     * Logs \a msg in a remote server. Returns 0 if success. Otherwise; returns a non-zero value.
     */
    virtual int log(const QString &msg) = 0;

    /**
     * Logs \a msg in a remote server with additional fields \a fields.
     * Returns 0 if success. Otherwise; returns a non-zero value.
     */
    virtual int log(const QString &msg, const FieldList& fields) = 0;
};

/// @}

} // namespace DAS

/// @}

} // namespace Lvk


#endif // LVK_DAS_REMOTELOGGER_H

