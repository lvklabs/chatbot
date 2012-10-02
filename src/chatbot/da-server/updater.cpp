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

#include <QtDebug>
#include <QDomDocument>

// TODO read from config file
#define REST_HOST        "https://www.daleaceptar.gob.ar"
#define REST_URL_BASE    "/media/chatbot"

#ifdef WIN32
# define REST_API_CFU       REST_HOST REST_URL_BASE "/latest_win32.xml"
#elif defined(Q_WS_MAC)
# define REST_API_CFU       REST_HOST REST_URL_BASE "/latest_mac.xml"
#else
# define REST_API_CFU       REST_HOST REST_URL_BASE "/latest_linux32.xml"
#endif

//--------------------------------------------------------------------------------------------------
// Updater
//--------------------------------------------------------------------------------------------------

Lvk::DAS::Updater::Updater()
    : m_rest(new DAS::Rest())
{
    connect(m_rest, SIGNAL(response(QString)), SLOT(onCfuResponse(QString)));
    connect(m_rest, SIGNAL(error(QNetworkReply::NetworkError)),
            SLOT(onCfuRerror(QNetworkReply::NetworkError)));
}

//--------------------------------------------------------------------------------------------------


Lvk::DAS::Updater::Updater(DAS::Rest *rest)
    : m_rest(rest)
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

    m_rest->request(REST_API_CFU);
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

    DAS::UpdateInfo info;

    if (parseResponse(info, resp)) {
        if (DAS::UpdateVersion(APP_VERSION_STR) < info.version()) {
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
        QDomNode rootNode = doc.documentElement();

        if (rootNode.nodeName().compare("update", Qt::CaseInsensitive) == 0) {
            parsingOk = parseUpdateNode(info, rootNode);
        } else {
            qCritical() << "Updater: Invalid root node name" << rootNode.nodeName();
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
        QString name = node.nodeName().toLower();
        QString value = node.nodeValue().trimmed();

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
        QString name = node.nodeName().toLower();
        QString value = node.nodeValue().trimmed();

        if (name == "li") {
            whatsNew.append(value);
        } else {
            qCritical() << "Updater: parseWhatsNewNode: Unknown node" << name;
        }
    }

    info.setWhatsNew(whatsNew);

    return true;
}
