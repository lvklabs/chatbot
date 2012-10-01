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

#ifndef LVK_BE_ACCOUNTVERIFIER_H
#define LVK_BE_ACCOUNTVERIFIER_H

#include <QObject>
#include <QString>

#include "back-end/roster.h"
#include "back-end/chattype.h"
#include "da-server/userauth.h"

class QMutex;

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace CA
{

class Chatbot;

}

namespace BE
{

/// \ingroup Lvk
/// \addtogroup BE
/// @{

/**
 * \brief AccountVerifier class 
 */

class AccountVerifier : public QObject
{
    Q_OBJECT

public:

    /**
     */
    AccountVerifier();

    /**
     */
    ~AccountVerifier();

    /**
     */
    void verify(ChatType type, const QString &user, const QString &passwd);

    /**
     */
    void abort();

    /**
     */
    struct AccountInfo
    {
        ChatType type;
        QString username;
        QString chatUsername;
        Roster roster;
    };

signals:

    /**
     * This signal is emitted after invoking verifyAccount() if the account was verified.
     * Returns the \a roster of the given account.
     */
    void accountOk(const AccountVerifier::AccountInfo &info);

    /**
     * This signal is emitted after invoking verifyAccount() if there was an error while trying
     * to verify the account.
     */
    void accountError(int err, const QString &msg);

private slots:
    void onDASAuthOk(const DAS::UserAuth::AuthInfo &info);
    void onDASAuthError(int code, const QString &msg);
    void onChatbotConnectionOk();
    void onChatbotConnectionError(int err);

private:
    QMutex *m_mutex;
    CA::Chatbot *m_chatbot;
    DAS::UserAuth m_daAuth;
    ChatType m_type;
    QString m_user;
    QString m_passwd;
    QString m_chatUser;

    void stepOne();
    void stepTwo();
    void clear(bool deleteLater);
};

/// @}

} // namespace BE

/// @}

} // namespace Lvk


#endif // LVK_BE_ACCOUNTVERIFIER_H

