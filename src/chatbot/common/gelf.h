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

#ifndef LVK_CMN_GELF_H
#define LVK_CMN_GELF_H

#include <QByteArray>
#include <QList>
#include <QPair>
#include <QString>

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
 * \brief The Gelf class provides a Graylog Extended Log Format (GELF) message.
 *
 * A GELF message is a GZIP'd or ZLIB'd JSON string with the following fields:
 * <ul>
 * <li>version: GELF spec version – "1.0" (string); MUST be set by client library.</li>
 * <li>host: the name of the host or application that sent this message (string);
 * MUST be set by client library.</li>
 * <li>short_message: a short descriptive message (string); MUST be set by client library.</li>
 * <li>full_message: a long message that can i.e. contain a backtrace and environment variables
 * (string); optional.</li>
 * <li>timestamp: UNIX microsecond timestamp (decimal); SHOULD be set by client library.</li>
 * <li>level: the level equal to the standard syslog levels (decimal); optional, default is 1
 * (ALERT). </li>
 * <li>facility: (string or decimal) optional, MUST be set by server to GELF if empty.</li>
 * <li>line: the line in a file that caused the error (decimal); optional.</li>
 * <li>file: the file (with path if you want) that caused the error (string); optional.</li>
 * <li>_[additional field]: every other field you send and prefix with a _ (underscore) will be
 * treated as an additional field.</li>
 * </ul>
 *
 * The Gelf class automaticaly sets version, host, timestamp, facility. Level and message must
 * be provided. Short message and full message are set to the same given message. Line and file are
 * not set.
 *
 * More info about GELF format at http://graylog2.org/about/gelf
 */
class Gelf
{
public:

    /**
     * The field class provides an additional (i.e. custom) field for GELF messages
     */
    class Field : public QPair<QString, QString>
    {
    public:
        /**
         * Constructs an empty field.
         */
        Field();

        /**
         * Constructs a field from the given pair \a p.
         */
        Field(const QPair<QString, QString> &p);

        /**
         * Returns the string representation.
         */
        QString toString() const;
    };

    /**
     * The field class provides a list of additional fields for GELF messages
     */
    class FieldList : public QList<Field>
    {
    public:
        /**
         * Returns the string representation.
         */
        QString toString() const;
    };

    /**
     * Log levels
     */
    enum Level
    {
        Emergency,      ///< System is unusable
        Alert,          ///< Action must be taken immediately
        Critical,       ///< Critical conditions
        Error,          ///< Error conditions
        Warning,        ///< Warning conditions
        Notice,         ///< Normal but significant condition
        Informational,  ///< Informational messages
        Debug           ///< Debug-level messages
    };

    /**
     * Constructs an empty GELF message.
     */
    Gelf();

    /**
     * Constructs a GELF message with \a level and message \a msg
     */
    Gelf(Level level, const QString &msg);

    /**
     * Constructs a GELF message with \a level, message \a msg and additional fields \a fields
     */
    Gelf(Level level, const QString &msg, const FieldList &fields);

    /**
     * Returns true is the GELF message is well-formed. Otherwise; returns false.
     */
    bool isValid();

    /**
     * Returns a byte array containing the GELF message
     */
    const QByteArray &data() const
    {
        return m_data;
    }

private:
    QByteArray m_data;

    void buildGelf(Level level, const QString &msg, const FieldList &fields);
};

/// @}

} // namespace Cmn

/// @}

} // namespace Lvk


#endif // LVK_CMN_GELF_H

