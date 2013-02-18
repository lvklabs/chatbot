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

#include "common/crashhandler.h"
#include "common/settings.h"
#include "common/settingskeys.h"
#include "common/version.h"
#include "da-server/remoteloggerfactory.h"

#include <csignal>
#include <cstdlib>
#include <QFile>
#include <QDateTime>
#include <QtDebug>
#include <QSysInfo>

//--------------------------------------------------------------------------------------------------
// Helpers
//--------------------------------------------------------------------------------------------------

namespace
{

// TODO refactor duplicated code
inline QString getOSType()
{
#ifdef Q_WS_X11
    return QString("Linux");
#elif defined(Q_WS_WIN)
    return QString("Windows 0x%1").arg(QString::number(QSysInfo::WindowsVersion,16));
#elif defined(Q_WS_MAC)
    return QString("Mac 0x%1").arg(QString::number(QSysInfo::MacintoshVersion,16));
#endif
}

} // namespace


//--------------------------------------------------------------------------------------------------
// CrashHandler
//--------------------------------------------------------------------------------------------------

QString Lvk::Cmn::CrashHandler::m_crashFilename = "";
QString Lvk::Cmn::CrashHandler::m_username = "";

//--------------------------------------------------------------------------------------------------

void Lvk::Cmn::CrashHandler::init()
{
    m_crashFilename = Cmn::Settings().value(SETTING_LOGS_PATH).toString() + "/crash.log";

    checkForCrash();

    ::signal(SIGSEGV, &CrashHandler::handler);
    ::signal(SIGFPE,  &CrashHandler::handler);
}

//--------------------------------------------------------------------------------------------------

void Lvk::Cmn::CrashHandler::handler(int sig)
{
    QFile file(m_crashFilename);

    if (file.open(QFile::WriteOnly)) {
        QString data = QString("%1 Chatbot %2 crashed. Signal: %3. OS: %4. User: %5. BT: %6")
                .arg(QDateTime::currentDateTime().toString(Qt::ISODate))
                .arg(APP_VERSION_STR)
                .arg(sig)
                .arg(getOSType())
                .arg((m_username))
                .arg(""); // TODO add backtrace

        file.write(data.toUtf8());
        file.close();
    }

    qFatal("Application crash!!! signal %d", sig);

    ::exit(1);
}

//--------------------------------------------------------------------------------------------------

void Lvk::Cmn::CrashHandler::checkForCrash()
{
#ifdef DA_CONTEST
    QFile file(m_crashFilename);

    if (file.exists()) {
        qDebug() << "Last time application has crashed! Sending event...";

        if (file.open(QFile::ReadOnly)) {
            QString msg = QString::fromUtf8(file.readAll());
            file.close();

            DAS::RemoteLogger *logger = DAS::RemoteLoggerFactory().createReliableLogger();

            if (logger->log(msg) == 0) {
                qDebug() << "Event sent!";

                if (!QFile::remove(m_crashFilename)) {
                    qCritical() << "Could not remove file" << m_crashFilename;
                }
            } else {
                qDebug() << "Connection failed. Retrying next time...";
            }

            delete logger;
        } else {
            qCritical() << "Could not read crash file" << m_crashFilename;
        }
    }
#endif
}

//--------------------------------------------------------------------------------------------------

void Lvk::Cmn::CrashHandler::setUsername(const QString &username)
{
    m_username = username;
}

