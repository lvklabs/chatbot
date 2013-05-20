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

#include "da-server/userauth.h"
#include "da-server/serverconfig.h"
#include "crypto/keymanagerfactory.h"
#include "common/json.h"

#include <memory>
#include <QtDebug>


#define KEY_UID          "uid"
#define KEY_USERNAME     "username"
#define KEY_EMAIL        "email"
#define KEY_ERR_MSG      "error_message"
#define KEY_ERR_CODE     "error_code"


//--------------------------------------------------------------------------------------------------
// UserAuth
//--------------------------------------------------------------------------------------------------

Lvk::DAS::UserAuth::UserAuth()
{
    connect(&m_rest, SIGNAL(response(const QString &)), SLOT(onRestResponse(const QString&)));

    connect(&m_rest, SIGNAL(error(QNetworkReply::NetworkError)),
            SLOT(onRestError(QNetworkReply::NetworkError)));
}

//--------------------------------------------------------------------------------------------------

void Lvk::DAS::UserAuth::authenticate(const QString &email)
{
    qDebug() << "UserAuth: Authenticating " << email << "...";

    std::auto_ptr<Crypto::KeyManager> keyMgr(Crypto::KeyManagerFactory().create());

    QString passwd = QString::fromUtf8(keyMgr->getKey(Crypto::KeyManager::AuthServerRole));

    if (passwd.size() > 0) {
        passwd.prepend("chatbot:");
        passwd.append("@");
    }

    QString req = QString(USER_AUTH_REST_FB_TRANSL).arg(passwd, email);

    if (!m_rest.request(req)) {
        emit error(ConnectionError, tr("Cannot connect to the server. Please try later."));
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::DAS::UserAuth::abort()
{
    qDebug() << "UserAuth: Manually aborted!";

    m_rest.abort();
}

//--------------------------------------------------------------------------------------------------

void Lvk::DAS::UserAuth::onRestResponse(const QString &response)
{
    qDebug() << "UserAuth: Got response: " << response;

    if (!verifyCertChain()) {
        emit error(SSLHandshakeError, tr("SSL handshake error. Please try later."));
        return;
    }

    Cmn::Json::Object jresp;
    if (Cmn::Json().parse(response, jresp)) {
        if (jresp.contains(KEY_USERNAME)) {
            qDebug() << "UserAuth: Parsed AuthOk msg ";
            handleAuthOk(jresp);
        } else if (jresp.contains(KEY_ERR_MSG)){
            qDebug() << "UserAuth: Parsed AuthError msg ";
            handleAuthError(jresp);
        } else {
            qCritical() << "UserAuth: Unknown response!";
            emit error(UnknownResponseError,
                       tr("Unknown response from server. Please try later."));
        }
    } else {
        qCritical() << "UserAuth: Invalid message format!";
        emit error(InvalidResponseFormatError,
                   tr("Unknown response from server. Please try later."));
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::DAS::UserAuth::onRestError(QNetworkReply::NetworkError err)
{
    qDebug() << "UserAuth: Error " << err;
    emit error(ConnectionError, tr("Cannot connect to the server. Please try later."));
}

//--------------------------------------------------------------------------------------------------

void Lvk::DAS::UserAuth::handleAuthOk(Cmn::Json::Object &jresp)
{
    QString email = jresp[KEY_EMAIL].toString();
    QString username = jresp[KEY_USERNAME].toString();
    QString uid = jresp[KEY_UID].toString();

    emit success(AuthInfo(email, username, uid));
}

//--------------------------------------------------------------------------------------------------

void Lvk::DAS::UserAuth::handleAuthError(Cmn::Json::Object &jresp)
{
    QString msg = jresp[KEY_ERR_MSG].toString();
    int code = jresp[KEY_ERR_CODE].toInt();

    int mappedCode = (code >= 1 && code <= 3) ? (1000 + code) : (UnknownSessionError + code);

    emit error(mappedCode, msg);
}

//--------------------------------------------------------------------------------------------------

bool Lvk::DAS::UserAuth::verifyCertChain()
{
    // TODO duplicated code! Refactor!

    bool valid = false;

    QList<QSslCertificate> certChain = m_rest.peerCertificateChain();

    try {
        // TODO check if all this stuff is really necessary or just check the immediate
        //      subject name

        if (certChain.size() == 0) {
            throw QString("Empty cert chain");
        }

        const QSslCertificate &cert = certChain[0];

        if (!cert.isValid()) {
            throw QString("Invalid peer's immediate cert");
        }
        if (cert.issuerInfo(QSslCertificate::CommonName) != CA2_CERT_ISSUER_NAME) {
            throw QString("wrong issuer name: " + cert.issuerInfo(QSslCertificate::CommonName));
        }
        if (cert.subjectInfo(QSslCertificate::CommonName) != PEER_CERT_SUBJECT_NAME) {
            throw QString("wrong subect name: " + cert.subjectInfo(QSslCertificate::CommonName));
        }

        valid = true;

    } catch (const QString &err) {
        qCritical() << "UserAuth: Invalid peer cert chain:" << err;
    }

    return valid;
}

