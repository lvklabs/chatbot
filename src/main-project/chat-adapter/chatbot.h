/*
 * Copyright (C) 2012 Andres Pagliano, Gabriel Miretti, Gonzalo Buteler,
 * Nestor Bustamante, Pablo Perez de Angelis
 *
 * This file is part of LVK Botmaster.
 *
 * LVK Botmaster is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * LVK Botmaster is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LVK Botmaster.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef LVK_CA_CHATBOT_H
#define LVK_CA_CHATBOT_H

#include <QObject>

class QString;

namespace Lvk
{

namespace CA
{

class VirtualUser;

/**
 * \brief Abstract interface for all chatbots
 *
 */

class Chatbot : public QObject
{
    Q_OBJECT

public:
    virtual ~Chatbot();

    virtual void connectToServer(const QString &user, const QString &passwd, const QString &host) = 0;
    virtual void disconnectFromServer() = 0;
    virtual void setVirtualUser(VirtualUser *virtualUser) = 0;
    virtual VirtualUser *virtualUser() = 0;

signals:
    void connected();
    void disconnected();
    void error(int err);
};

} // namespace CA

} // namespace Lvk

#endif // LVK_CA_CHATBOT_H
