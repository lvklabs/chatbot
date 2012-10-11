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

#ifndef LVK_FE_TESTINPUTTEXT_H
#define LVK_FE_TESTINPUTTEXT_H

#include "back-end/roster.h"

#include <QLineEdit>
#include <QStringList>

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace FE
{

/// \ingroup Lvk
/// \addtogroup FE
/// @{

/**
 * \brief TestInputText class 
 */

class TestInputText : public QLineEdit
{
    Q_OBJECT

public:
    explicit TestInputText(QWidget *parent = 0);

    void setRoster(const BE::Roster &roster);

    void clearRoster();

    BE::RosterItem currentItem();

signals:
    void currentItemChanged();

protected:
    void contextMenuEvent(QContextMenuEvent *event);

private slots:
    void onSimulateUser();

private:
    QString m_anyUser;
    BE::Roster m_roster;
    BE::RosterItem m_currentItem;
    QStringList m_list;

};

/// @}

} // namespace FE

/// @}

} // namespace Lvk


#endif // LVK_FE_TESTINPUTTEXT_H

