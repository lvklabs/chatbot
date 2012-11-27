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

#include "da-server/sftpuploader.h"
#include "da-server/serverconfig.h"
#include "crypto/keymanagerfactory.h"

#include <QtDebug>
#include <QFileInfo>
#include <QMutex>
#include <QMutexLocker>
#include <QDateTime>
#include <memory>


//--------------------------------------------------------------------------------------------------
// SftpUploader
//--------------------------------------------------------------------------------------------------

Lvk::DAS::SftpUploader::SftpUploader()
    : m_mutex(new QMutex(QMutex::Recursive)), m_inProgress(false), m_connection(0)
{
}

//--------------------------------------------------------------------------------------------------

Lvk::DAS::SftpUploader::~SftpUploader()
{
    qDebug() << "SftpUploader: destroyed";

    {
        QMutexLocker locker(m_mutex);
        close();
    }

    delete m_mutex;
}

//--------------------------------------------------------------------------------------------------

void Lvk::DAS::SftpUploader::upload(const QString &filename, const QString &username)
{
    QMutexLocker locker(m_mutex);

    if (m_inProgress) {
        return;
    }

    m_inProgress = true;

    initFilenames(filename, username);

    QSsh::SshConnectionParameters params;
    getConnectionParams(params);

    m_connection = new QSsh::SshConnection(params, this);

    connect(m_connection, SIGNAL(connected()), SLOT(onConnected()));
    connect(m_connection, SIGNAL(error(QSsh::SshError)), SLOT(onConnectionError(QSsh::SshError)));

    qDebug() << "SftpUploader: Connecting to host...";

    m_connection->connectToHost();
}

//--------------------------------------------------------------------------------------------------

void Lvk::DAS::SftpUploader::initFilenames(const QString &filename, const QString &username)
{
    QFileInfo info(filename);

    m_localFilename = info.canonicalFilePath();

    m_remoteFilename = QString("%1/%2_%3_%4").arg(FILE_SERVER_DEST_PATH,
                                                  QDateTime::currentDateTime().toString(Qt::ISODate),
                                                  username, info.fileName());
}

//--------------------------------------------------------------------------------------------------

void Lvk::DAS::SftpUploader::getConnectionParams(QSsh::SshConnectionParameters &params)
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

void Lvk::DAS::SftpUploader::onConnected()
{
    qDebug() << "SftpUploader: Connected";
    qDebug() << "SftpUploader: Creating SFTP channel...";

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
        qDebug() << "SftpUploader: Error null channel";

        close();
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::DAS::SftpUploader::onConnectionError(QSsh::SshError err)
{
    qDebug() << "SftpUploader: Connection error" << err;

    QMutexLocker locker(m_mutex);

    close();
}

//--------------------------------------------------------------------------------------------------

void Lvk::DAS::SftpUploader::onChannelInitialized()
{
    qDebug() << "SftpUploader: Channel Initialized";
    qDebug() << "SftpUploader: Uploading" << m_localFilename << "->" << m_remoteFilename;

    QMutexLocker locker(m_mutex);

    QSsh::SftpJobId job = m_channel->uploadFile(m_localFilename, m_remoteFilename,
                                                QSsh::SftpOverwriteExisting);

    if (job != QSsh::SftpInvalidJob) {
        qDebug() << "SftpUploader: Started job #" << job;
    } else {
        qDebug() << "SftpUploader: Invalid Job";

        close();
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::DAS::SftpUploader::onChannelError(const QString &err)
{
    qDebug() << "SftpUploader: Error: " << err;

    QMutexLocker locker(m_mutex);

    close();
}

//--------------------------------------------------------------------------------------------------

void Lvk::DAS::SftpUploader::onOpfinished(QSsh::SftpJobId job, const QString &err)
{
    qDebug() << "SftpUploader: Finished job #" << job << ":" << (err.isEmpty() ? "OK" : err);

    QMutexLocker locker(m_mutex);

    close();
}

//--------------------------------------------------------------------------------------------------

void Lvk::DAS::SftpUploader::close()
{
    qDebug() << "SftpUploader: Closed";

    if (m_inProgress) {
        m_inProgress = false;
        m_channel.clear();
        delete m_connection;
    }
}






