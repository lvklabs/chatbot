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

#ifndef LVK_CMN_LOGGER_H
#define LVK_CMN_LOGGER_H

#include <QtGlobal>

class QFile;

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
 * \brief Logger class provides a logging features for the Chatbot application.
 *
 * The logger class logs messages passed to Qt's qDebug(), qWarning(), qCritical() and
 * qFatal(). When a message is passed to any of these functions, the message is persisted
 * in a log file (by default logs/chatbot.log) and it is also printed in the stdout or stderr.
 *
 * If qFatal() is invoked after logging the message the application is terminated.
 *
 * To start using the logger just call init(). To stop the logger call shutdown().
 */
class Logger
{
public:

    /**
     * Initializes the Chatbot application logger.
     */
    static void init();

    /**
     * Shuts down the Chatbot application logger.
     */
    static void shutdown();

    /**
     * Rotates \a logFilename if it is greater than \a maxSize. The rotated filename will be called
     * \a logFilename + ".1"
     */
    static void rotateLog(const QString &logFilename, qint64 maxSize = 1024*1024);

private:
    Logger();
    Logger(Logger&);

    static void msgHandler(QtMsgType type, const char *msg);

    static QFile *m_logFile;
    static QString m_strPid;
};

/// @}

} // namespace Cmn

/// @}

} // namespace Lvk


#endif // LVK_CMN_LOGGER_H

