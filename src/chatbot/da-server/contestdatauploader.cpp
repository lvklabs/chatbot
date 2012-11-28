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

#include "da-server/contestdatauploader.h"
#include "da-server/serverconfig.h"
#include "da-server/remoteloggerfactory.h"
#include "da-server/remoteloggerkeys.h"
#include "crypto/keymanagerfactory.h"
#include "common/version.h"

#include <QtDebug>
#include <QFileInfo>
#include <QMutex>
#include <QMutexLocker>
#include <QDateTime>
#include <memory>


//--------------------------------------------------------------------------------------------------
// ContestDataUploader
//--------------------------------------------------------------------------------------------------

Lvk::DAS::ContestDataUploader::ContestDataUploader()
    : m_mutex(new QMutex(QMutex::Recursive)), m_inProgress(false), m_connection(0)
{
}

//--------------------------------------------------------------------------------------------------

Lvk::DAS::ContestDataUploader::~ContestDataUploader()
{
    qDebug() << "ContestDataUploader: destroyed";

    close();

    delete m_mutex;
}

//--------------------------------------------------------------------------------------------------

void Lvk::DAS::ContestDataUploader::upload(const DAS::ContestData &data)
{
    QMutexLocker locker(m_mutex);

    if (m_inProgress) {
        emit finished(UploadInProgressError);

        return;
    }

    m_inProgress = true;
    m_data = data;

    initFilenames();

    QSsh::SshConnectionParameters params;
    getConnectionParams(params);

    m_connection = new QSsh::SshConnection(params, this);

    connect(m_connection, SIGNAL(connected()), SLOT(onConnected()));
    connect(m_connection, SIGNAL(error(QSsh::SshError)), SLOT(onConnectionError(QSsh::SshError)));

    qDebug() << "ContestDataUploader: Connecting to host...";

    m_connection->connectToHost();
}

//--------------------------------------------------------------------------------------------------

void Lvk::DAS::ContestDataUploader::initFilenames()
{
    QFileInfo info(m_data.filename);

    m_localFilename = info.canonicalFilePath();

    m_remoteFilename = QString("%1/%2_%3_%4")
            .arg(FILE_SERVER_DEST_PATH, QDateTime::currentDateTime().toString(Qt::ISODate),
                 m_data.username, info.fileName());
}

//--------------------------------------------------------------------------------------------------

void Lvk::DAS::ContestDataUploader::getConnectionParams(QSsh::SshConnectionParameters &params)
{
    std::auto_ptr<Crypto::KeyManager> keyMgr(Crypto::KeyManagerFactory().create());
    QString passwd = QString::fromUtf8(keyMgr->getKey(Crypto::KeyManager::FileServerRole));

    params.host = FILE_SERVER_HOST;
    params.port = FILE_SERVER_PORT;
    params.userName = FILE_SERVER_USERNAME;
    params.password = passwd;
    params.authenticationType = QSsh::SshConnectionParameters::AuthenticationByPassword;
    params.timeout = 30;
}

//--------------------------------------------------------------------------------------------------

void Lvk::DAS::ContestDataUploader::onConnected()
{
    qDebug() << "ContestDataUploader: Connected";
    qDebug() << "ContestDataUploader: Creating SFTP channel...";

    QMutexLocker locker(m_mutex);

    m_channel = m_connection->createSftpChannel();

    if (m_channel) {
        connect(m_channel.data(), SIGNAL(initialized()),
                SLOT(onChannelInitialized()));
        connect(m_channel.data(), SIGNAL(initializationFailed(QString)),
                SLOT(onChannelError(QString)));
        connect(m_channel.data(), SIGNAL(finished(QSsh::SftpJobId, QString)),
                SLOT(onOpfinished(QSsh::SftpJobId, QString)));

        m_channel->initialize();

    } else {
        qDebug() << "ContestDataUploader: Error null channel";

        finish(ConnectionError);
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::DAS::ContestDataUploader::onConnectionError(QSsh::SshError err)
{
    qDebug() << "ContestDataUploader: Connection error" << err;

    finish(ConnectionError);
}

//--------------------------------------------------------------------------------------------------

void Lvk::DAS::ContestDataUploader::onChannelInitialized()
{
    qDebug() << "ContestDataUploader: Channel Initialized";
    qDebug() << "ContestDataUploader: Uploading" << m_localFilename << "->" << m_remoteFilename;

    QMutexLocker locker(m_mutex);

    QSsh::SftpJobId job = m_channel->uploadFile(m_localFilename, m_remoteFilename,
                                                QSsh::SftpOverwriteExisting);

    if (job != QSsh::SftpInvalidJob) {
        qDebug() << "ContestDataUploader: Started job #" << job;
    } else {
        qDebug() << "ContestDataUploader: Invalid Job";

        finish(ChannelError);
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::DAS::ContestDataUploader::onChannelError(const QString &err)
{
    qDebug() << "ContestDataUploader: Error: " << err;

    finish(ChannelError);
}

//--------------------------------------------------------------------------------------------------

void Lvk::DAS::ContestDataUploader::onOpfinished(QSsh::SftpJobId job, const QString &err)
{
    bool success = err.isEmpty();

    qDebug() << "ContestDataUploader: Finished job #" << job << ":" << (success ? "Success" : err);

    if (success) {
        if (sendScore()) {
            finish(Success);
        } else {
            finish(SecureLogError);
        }
    } else {
        finish(ChannelError);
    }
}

//--------------------------------------------------------------------------------------------------

bool Lvk::DAS::ContestDataUploader::sendScore()
{
    DAS::RemoteLogger::FieldList fields;
    fields.append(RLOG_KEY_APP_VERSION,    APP_VERSION_STR);
    fields.append(RLOG_KEY_CHATBOT_ID,     m_data.chatbotId);
    fields.append(RLOG_KEY_USER_ID,        m_data.username);
    fields.append(RLOG_KEY_RULES_SCORE,    QString::number(m_data.bestScore.rules));
    fields.append(RLOG_KEY_CONV_SCORE,     QString::number(m_data.bestScore.conversations));
    fields.append(RLOG_KEY_CONTACTS_SCORE, QString::number(m_data.bestScore.contacts));
    fields.append(RLOG_KEY_TOTAL_SCORE,    QString::number(m_data.bestScore.total));

    std::auto_ptr<DAS::RemoteLogger> secureLogger(DAS::RemoteLoggerFactory().createSecureLogger());

    return secureLogger->log("Manually uploaded score", fields) == 0;
}

//--------------------------------------------------------------------------------------------------

void Lvk::DAS::ContestDataUploader::finish(Status status)
{
    close();

    emit finished(status);
}

//--------------------------------------------------------------------------------------------------

void Lvk::DAS::ContestDataUploader::close()
{
    QMutexLocker locker(m_mutex);

    qDebug() << "ContestDataUploader: Closed";

    if (m_inProgress) {
        m_inProgress = false;
        m_channel.clear();
        delete m_connection;
    }
}

