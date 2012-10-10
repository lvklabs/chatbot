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

#ifndef LVK_BE_RLOGHELPER_H
#define LVK_BE_RLOGHELPER_H

#include "da-server/remotelogger.h"

#include <QString>
#include <QDateTime>

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace Stats
{
struct Score;
}

namespace BE
{

/// \ingroup Lvk
/// \addtogroup BE
/// @{

/**
 * \brief The RlogHelper class provides helper methods to log information in the remote server
 */

class RlogHelper
{
public:

    /**
     * Creates a RLogHelper object
     */
    RlogHelper();

    /**
     * Destroys the object
     */
    ~RlogHelper();

    /**
     * Sets the current username
     */
    void setUsername(const QString &username);

    /**
     * Sets the current chatbot ID
     */
    void setChatbotId(const QString &chatbotId);

    /**
     * Clears the username and chatbot ID previusly set
     */
    void clear();

    /**
     * Log application launched
     */
    bool logAppLaunched();

    /**
     * Log application closed
     */
    bool logAppClosed();

    /**
     * Log account verified
     */
    bool logAccountVerified(const QString &username, const QString &domain);

    /**
     * Log chatbot connected or disconnected
     */
    bool logChatbotConnected(bool connected);

    /**
     * Log automatically triggered score \s
     */
    bool logAutoScore(const Stats::Score &s);

    /**
     * Log manually triggered score \s
     */
    bool logManualScore(const Stats::Score &s);

    /**
     * Log manually triggered score \s
     */
    bool logDefaultMetrics();

private:
    RlogHelper(const RlogHelper&);
    RlogHelper & operator=(const RlogHelper&);

    DAS::RemoteLogger *m_fastLogger;
    DAS::RemoteLogger *m_secureLogger;
    bool m_statsEnabled;
    QString m_appVersion;
    QString m_username;
    QString m_chatbotId;
    QDateTime m_connectionStart;

    bool remoteLog(const QString &msg, const DAS::RemoteLogger::FieldList &fields, bool secure);
};

/// @}

} // namespace BE

/// @}

} // namespace Lvk


#endif // LVK_BE_RLOGHELPER_H

