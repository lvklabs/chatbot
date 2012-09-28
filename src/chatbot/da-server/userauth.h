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

#ifndef LVK_DAS_USERAUTH_H
#define LVK_DAS_USERAUTH_H

#include <QObject>
#include <QString>

#include "da-server/rest.h"
#include "common/json.h"

class UserAuthTest;

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace DAS
{

/// \ingroup Lvk
/// \addtogroup DAS
/// @{

/**
 * \brief The UserAuth class provides user authentification against the 'Dale Aceptar' contest
 */
class UserAuth : public QObject
{
    Q_OBJECT

    friend class ::UserAuthTest;

public:

    /**
     * Authentication error codes
     */
    enum Error
    {
        ConnectionError = 1,
        InvalidResponseFormatError = 500,
        UserNotRegistered = 1001,
        UserNotMigrated = 1002,
        TemporalSessionError = 1003,
        UnknownResponseError = 1009,
        UnknownSessionError = 1010
    };

    /**
     * Authentication information
     */
    struct AuthInfo
    {
        AuthInfo() { }

        AuthInfo(const QString &email, const QString &username, const QString &uid)
            : email(email), username(username), uid(uid) { }

        QString email;      ///< e-mail
        QString username;   ///< username
        QString uid;        ///< user id
    };

    /**
     * Constructs a UserAuth object
     */
    UserAuth();

    /**
     * Authenticates \a email. On success signal \a success() is emitted. Otherwise; signal
     * \a error() is emitted.
     */
    void authenticate(const QString &email);

    /**
     * Aborts the authentication in progress (if any)
     */
    void abort();

signals:

    /**
     * This signal is emitted on successful authentication after calling authenticate()
     */
    void success(const DAS::UserAuth::AuthInfo &info);

    /**
     * This signal is emitted on failed authentication after calling authenticate()
     */
    void error(int code, const QString &msg);

private slots:
    void onRestResponse(const QString &response);
    void onRestError(QNetworkReply::NetworkError err);

private:
    Rest m_rest;

    void handleAuthOk(Cmn::Json::Object &jresp);
    void handleAuthError(Cmn::Json::Object &jresp);
};

/// @}

} // namespace DAS

/// @}

} // namespace Lvk


#endif // LVK_DAS_USERAUTH_H

