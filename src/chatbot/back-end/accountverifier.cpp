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

#include "back-end/accountverifier.h"
#include "back-end/chatbotfactory.h"
#include "chat-adapter/chatbot.h"

#include <QMutex>
#include <QMutexLocker>

namespace
{

// TODO this is duplicated code from AppFace. Refactor!
inline Lvk::BE::Roster toBERoster(const Lvk::CA::ContactInfoList &infoList)
{
    Lvk::BE::Roster roster;

    foreach (const Lvk::CA::ContactInfo &info, infoList) {
        roster.append(Lvk::BE::RosterItem(info.username, info.fullname));
    }

    return roster;
}

}

//--------------------------------------------------------------------------------------------------
// AccountVerifier
//--------------------------------------------------------------------------------------------------

Lvk::BE::AccountVerifier::AccountVerifier()
    : m_mutex(new QMutex(QMutex::Recursive)), m_chatbot(0)
{
    connect(&m_daAuth, SIGNAL(success(DAS::UserAuth::AuthInfo)),
            SLOT(onDASAuthOk(DAS::UserAuth::AuthInfo)));

    connect(&m_daAuth, SIGNAL(error(int,QString)), SLOT(onDASAuthError(int,QString)));

}

//--------------------------------------------------------------------------------------------------

Lvk::BE::AccountVerifier::~AccountVerifier()
{
    {
        QMutexLocker locker(m_mutex);
        delete m_chatbot;
    }

    delete m_mutex;
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AccountVerifier::clear(bool deleteLater)
{
    if (m_chatbot) {
        if (deleteLater) {
            m_chatbot->deleteLater();
        } else {
            delete m_chatbot;
        }
        m_chatbot = 0;
    }

    m_user.clear();
    m_passwd.clear();
    m_chatUser.clear();
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AccountVerifier::verify(BE::ChatType type, const QString &user, const QString &passwd)
{
    QMutexLocker locker(m_mutex);

    if (m_chatbot) {
        m_daAuth.abort();
        clear(false);
    }

    m_type = type;
    m_user = user;
    m_passwd = passwd;

    // For Dale Aceptar contest we have a two-step verification process.
    // 1. Go to the Dale Aceptar server and verify if user is registered.
    //    If registered, we will get the username to connect to the chat server
    // 2. Connect to the chat server to verify if user/password is OK

#ifdef DA_CONTEST
    stepOne();
#else
    m_chatUser = m_user;
    stepTwo();
#endif
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AccountVerifier::abort()
{
    QMutexLocker locker(m_mutex);

    m_daAuth.abort();
    clear(false);
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AccountVerifier::stepOne()
{
    m_daAuth.authenticate(m_user);
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AccountVerifier::stepTwo()
{
    m_chatbot = BE::ChatbotFactory().createChatbot(m_type);

    connect(m_chatbot, SIGNAL(connected()), SLOT(onChatbotConnectionOk()));
    connect(m_chatbot, SIGNAL(error(int)), SLOT(onChatbotConnectionError(int)));

    m_chatbot->connectToServer(m_chatUser, m_passwd);
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AccountVerifier::onDASAuthOk(const DAS::UserAuth::AuthInfo &info)
{
    QMutexLocker locker(m_mutex);

    m_chatUser = info.username;

    stepTwo();
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AccountVerifier::onDASAuthError(int code, const QString &msg)
{
    {
        QMutexLocker locker(m_mutex);
        clear(false);
    }

    emit accountError(code, msg);
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AccountVerifier::onChatbotConnectionOk()
{
    AccountInfo info;

    {
        QMutexLocker locker(m_mutex);

        info.type = m_type;
        info.username = m_user;
        info.chatUsername = m_chatUser;
        info.roster = toBERoster(m_chatbot->roster());

        clear(true);
    }

    emit accountOk(info);

}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AccountVerifier::onChatbotConnectionError(int err)
{
    {
        QMutexLocker locker(m_mutex);
        clear(true);
    }

    emit accountError(err, "");
}


