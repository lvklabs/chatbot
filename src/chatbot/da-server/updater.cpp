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

#include "da-server/updater.h"
#include "da-server/rest.h"
#include "common/version.h"
#include "da-server/serverconfig.h"

#include <QtDebug>
#include <QDomDocument>

//--------------------------------------------------------------------------------------------------
// Updater
//--------------------------------------------------------------------------------------------------

Lvk::DAS::Updater::Updater()
    : m_rest(new DAS::Rest()), m_curVersion(APP_VERSION_STR)
{
    connect(m_rest, SIGNAL(response(QString)), SLOT(onCfuResponse(QString)));
    connect(m_rest, SIGNAL(error(QNetworkReply::NetworkError)),
            SLOT(onCfuRerror(QNetworkReply::NetworkError)));
}

//--------------------------------------------------------------------------------------------------


Lvk::DAS::Updater::Updater(DAS::Rest *rest)
    : m_rest(rest), m_curVersion(APP_VERSION_STR)
{
    connect(m_rest, SIGNAL(response(QString)), SLOT(onCfuResponse(QString)));
    connect(m_rest, SIGNAL(error(QNetworkReply::NetworkError)),
            SLOT(onCfuRerror(QNetworkReply::NetworkError)));
}

//--------------------------------------------------------------------------------------------------

Lvk::DAS::Updater::~Updater()
{
    delete m_rest;
}

//--------------------------------------------------------------------------------------------------

void Lvk::DAS::Updater::checkForUpdate()
{
    qDebug() << "Updater: Checking for updates...";

    m_rest->request(UPDATER_REST_API_CFU);
}

//--------------------------------------------------------------------------------------------------

void Lvk::DAS::Updater::abort()
{
    qDebug() << "Updater: CFU Aborted";

    m_rest->abort();
}

//--------------------------------------------------------------------------------------------------

void Lvk::DAS::Updater::onCfuResponse(const QString &resp)
{
    qDebug() << "Updater: CFU response";

    if (!verifyCertChain()) {
        qCritical() << "Updater: Invalid certificate chain. Ignoring response.";
        return;
    }

    DAS::UpdateInfo info;

    if (parseResponse(info, resp)) {
        if (m_curVersion < info.version()) {
            qDebug() << "Updater: Found new update!";
            emit update(info);
        } else {
            emit noUpdate();
        }
    } else {
        emit noUpdate();
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::DAS::Updater::onCfuRerror(QNetworkReply::NetworkError err)
{
    qDebug() << "Updater: CFU error" << err;

    // If error we simply emit noUpdate()
    emit noUpdate();
}

//--------------------------------------------------------------------------------------------------

bool Lvk::DAS::Updater::parseResponse(Lvk::DAS::UpdateInfo &info, const QString &response)
{
    /* Response example:

        <update>
          <version>1.1</version>
          <date>28/10/2012</date>
          <critical>yes</critical>
          <url>http://lvklabs.com/builds/chatbot/chatbot-linux32-0.23.zip</url>
          <whatsnew>
            <li>Fixed bug 1</li>
            <li>Fixed bug 2</li>
          </whatsnew>
        </update>
    */

    QDomDocument doc;
    QString err;
    int line = 0;
    int col = 0;
    bool parsingOk = false;

    if (doc.setContent(response, &err, &line, &col)) {
        QDomElement root = doc.documentElement();

        if (root.tagName().compare("update", Qt::CaseInsensitive) == 0) {
            parsingOk = parseUpdateNode(info, root);
        } else {
            qCritical() << "Updater: Invalid root node name" << root.tagName();
        }
    } else {
        qDebug() << "Updater: Error line " << line << ":" << col << err ;
    }

    return parsingOk;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::DAS::Updater::parseUpdateNode(Lvk::DAS::UpdateInfo &info, QDomNode &updateNode)
{
    bool version = false, date = false, severity = false, url = false; // mandatory fields

    for (int i = 0; i < updateNode.childNodes().size(); ++i) {
        QDomNode node = updateNode.childNodes().item(i);

        if (node.childNodes().isEmpty()) {
            continue;
        }

        QString name = node.nodeName().toLower();
        QString value = node.childNodes().at(0).nodeValue().trimmed();

        if (name == "version") {
            info.setVersion(DAS::UpdateVersion(value));
            version = info.version().isValid();
        } else if (name == "date") {
            info.setDate(QDate::fromString(value, "dd/MM/yyyy"));
            date = info.date().isValid();
        } else if (name == "critical") {
            info.setSeverity(value == "yes" ? UpdateInfo::Critical : UpdateInfo::Low);
            severity = true;
        } else if (name == "url") {
            info.setUrl(value);
            url = true;
        } else if (name == "whatsnew") {
            parseWhatsNewNode(info, node);
        } else {
            qWarning() << "Updater: parseUpdateNode: Unknown node" << name;
        }
    }

    return version && date && severity && url;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::DAS::Updater::parseWhatsNewNode(Lvk::DAS::UpdateInfo &info, QDomNode &wnNode)
{
    QStringList whatsNew;

    for (int i = 0; i < wnNode.childNodes().size(); ++i) {
        QDomNode node = wnNode.childNodes().item(i);

        if (node.childNodes().isEmpty()) {
            continue;
        }

        QString name = node.nodeName().toLower();
        QString value = node.childNodes().at(0).nodeValue().trimmed();

        if (name == "li") {
            if (!value.isEmpty()) {
                whatsNew.append(value);
            }
        } else {
            qCritical() << "Updater: parseWhatsNewNode: Unknown node" << name;
        }
    }

    info.setWhatsNew(whatsNew);

    return true;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::DAS::Updater::verifyCertChain()
{
    // TODO duplicated code! Refactor!

    bool valid = false;

    QList<QSslCertificate> certChain = m_rest->peerCertificateChain();

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
            throw QString("wrong issuer name");
        }
        if (cert.subjectInfo(QSslCertificate::CommonName) != PEER_CERT_SUBJECT_NAME) {
            throw QString("wrong subect name");
        }

        valid = true;

    } catch (const QString &err) {
        qCritical() << "Updater: Invalid peer cert chain:" << err;
    }

    return valid;
}
