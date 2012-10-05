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

#include "rloghelper.h"
#include "da-server/remoteloggerfactory.h"
#include "da-server/remoteloggerkeys.h"
#include "stats/score.h"
#include "stats/statsmanager.h"
#include "common/version.h"
#include "common/settings.h"
#include "common/settingskeys.h"

#include <QtDebug>

//--------------------------------------------------------------------------------------------------
// Helpers
//--------------------------------------------------------------------------------------------------

namespace
{

inline void append(Lvk::DAS::RemoteLogger::FieldList & fields, const Lvk::Stats::Score &s)
{
    fields.append(RLOG_KEY_RULES_SCORE,    QString::number(s.rules));
    fields.append(RLOG_KEY_CONV_SCORE,     QString::number(s.conversations));
    fields.append(RLOG_KEY_CONTACTS_SCORE, QString::number(s.contacts));
    fields.append(RLOG_KEY_TOTAL_SCORE,    QString::number(s.total));
}

//--------------------------------------------------------------------------------------------------

inline QString getMetric(Lvk::Stats::Metric m)
{
    return QString::number(Lvk::Stats::StatsManager::manager()->metric(m).toInt(), 10);
}

}

//--------------------------------------------------------------------------------------------------
// RlogHelper
//--------------------------------------------------------------------------------------------------

Lvk::BE::RlogHelper::RlogHelper()
    : m_fastLogger(DAS::RemoteLoggerFactory().createFastLogger()),
      m_secureLogger(DAS::RemoteLoggerFactory().createSecureLogger()),
      m_statsEnabled(Cmn::Settings().value(SETTING_APP_SEND_STATS).toBool()),
      m_appVersion(APP_VERSION_STR)
{
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::RlogHelper::~RlogHelper()
{
    delete m_secureLogger;
    delete m_fastLogger;
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::RlogHelper::setUsername(const QString &username)
{
    m_username = username;
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::RlogHelper::setChatbotId(const QString &chatbotId)
{
    m_chatbotId = chatbotId;
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::RlogHelper::clear()
{
    m_username.clear();
    m_chatbotId.clear();
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::RlogHelper::logAccountVerified(const QString &username, const QString &domain)
{
    DAS::RemoteLogger::FieldList fields;
    fields.append(RLOG_KEY_USERNAME,  username);
    fields.append(RLOG_KEY_DOMAIN,    domain);

    return remoteLog("Account Verified", fields, false);
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::RlogHelper::logAutoScore(const Stats::Score &s)
{
    DAS::RemoteLogger::FieldList fields;
    append(fields, s);

    return remoteLog("Score", fields, false);
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::RlogHelper::logManualScore(const Stats::Score &s)
{
    DAS::RemoteLogger::FieldList fields;
    fields.append(RLOG_KEY_APP_VERSION,    m_appVersion);
    fields.append(RLOG_KEY_CHATBOT_ID,     m_chatbotId);
    fields.append(RLOG_KEY_USER_ID,        m_username);
    append(fields, s);

    return m_secureLogger->log("Manually uploaded score", fields) == 0;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::RlogHelper::logDefaultMetrics()
{
    // TODO complete metrics!
    DAS::RemoteLogger::FieldList fields;
    fields.append(RLOG_KEY_RULE_COUNT, getMetric(Stats::TotalRules));

    return remoteLog("Metrics", fields, false);
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::RlogHelper::remoteLog(const QString &msg, const DAS::RemoteLogger::FieldList &cfields,
                                    bool secure)
{
    if (!m_statsEnabled) {
        return true;
    }

    DAS::RemoteLogger::FieldList fields = cfields;
    fields.prepend(RLOG_KEY_APP_VERSION, m_appVersion);
    fields.prepend(RLOG_KEY_CHATBOT_ID, m_chatbotId);
    fields.prepend(RLOG_KEY_USER_ID,    m_username);

    if (secure) {
        return m_secureLogger->log(msg, fields) == 0;
    } else {
        return m_fastLogger->log(msg, fields) == 0;
    }
}
