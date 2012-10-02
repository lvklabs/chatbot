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

#ifndef LVK_DAS_UPDATER_H
#define LVK_DAS_UPDATER_H

#include "da-server/updateinfo.h"

#include <QObject>
#include <QNetworkReply>

class QDomNode;
class UpdaterUnitTest;

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace DAS
{

class Rest;

/// \ingroup Lvk
/// \addtogroup DAS
/// @{

/**
 * \brief The Updater class provides functionality related with application udpates
 */
class Updater : public QObject
{
    Q_OBJECT

    friend class ::UpdaterUnitTest;

public:

    /**
     * Creates an Updater object
     */
    Updater();

    /**
     * Destroys the object
     */
    ~Updater();

    /**
     * Checks for updates. Emits update() if an update was found. Otherwise; emits noUpdate();
     */
    void checkForUpdate();

    /**
     * Aborts the check for update currently in progress (if any)
     */
    void abort();

signals:

    /**
     * This signal is emitted whenever checkForUpdate() was called and there is no update.
     */
    void noUpdate();

    /**
     * This signal is emitted whenever checkForUpdate() was called and there is an update available
     */
    void update(const DAS::UpdateInfo &info);

private slots:
    void onCfuResponse(const QString &resp);
    void onCfuRerror(QNetworkReply::NetworkError err);

private:
    Rest *m_rest;
    UpdateVersion m_curVersion;

    Updater(Rest *rest);

    bool parseResponse(UpdateInfo &info, const QString &response);
    bool parseVersion(UpdateInfo &info, const QString &strVer);
    bool parseUpdateNode(UpdateInfo &info, QDomNode &updateElem);
    bool parseWhatsNewNode(UpdateInfo &info, QDomNode &wnNode);
};

/// @}

} // namespace DAS

/// @}

} // namespace Lvk


#endif // LVK_DAS_UPDATER_H

