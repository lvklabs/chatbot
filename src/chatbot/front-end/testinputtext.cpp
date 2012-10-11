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

#include "front-end/testinputtext.h"

#include <QMenu>
#include <QContextMenuEvent>
#include <QInputDialog>
#include <QMessageBox>
#include <algorithm>

//--------------------------------------------------------------------------------------------------
// TestInputText
//--------------------------------------------------------------------------------------------------

namespace
{

inline bool fullnameComp(const Lvk::BE::RosterItem &i1, const Lvk::BE::RosterItem &i2)
{
    return i1.fullname < i2.fullname;
}

}

//--------------------------------------------------------------------------------------------------
// TestInputText
//--------------------------------------------------------------------------------------------------

Lvk::FE::TestInputText::TestInputText(QWidget *parent)
    : QLineEdit(parent), m_anyUser(tr("Any user"))
{
    m_list.append(m_anyUser);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::TestInputText::setRoster(const BE::Roster &roster)
{
    m_roster = roster;

    std::sort(m_roster.begin(), m_roster.end(), fullnameComp);

    m_currentItem.clear();

    m_list.clear();
    m_list.append(m_anyUser);
    foreach (const BE::RosterItem &item, m_roster) {
        m_list.append(item.displayText());
    }
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::RosterItem Lvk::FE::TestInputText::currentItem()
{
    return m_currentItem;
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::TestInputText::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *menu = createStandardContextMenu();
    QAction *simAction = new QAction(tr("Test as user..."), menu);

    menu->addSeparator();
    menu->addAction(simAction);

    connect(simAction, SIGNAL(triggered()), SLOT(onSimulateUser()));

    menu->exec(event->globalPos());

    delete menu;
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::TestInputText::onSimulateUser()
{
    bool ok;

    QString fullname = QInputDialog::getItem(tr("Select user"),
                                             tr("Select the user you want to emulate to test:"),
                                             m_list, 0, true, &ok, this);

    if (ok) {
        m_currentItem.clear();

        if (fullname == m_anyUser) {
            // Nothing to do
        } else {
            foreach (const BE::RosterItem &item, m_roster) {
                if (fullname == item.fullname) {
                    m_currentItem = item;
                    break;
                }
            }

            if (m_currentItem.isNull()) {
                QMessageBox::critical(this, tr("Invalid user"), tr("The selected user is invalid"));
            }
        }

        emit currentItemChanged();
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::TestInputText::clearRoster()
{
    m_roster.clear();
    m_list.clear();
    m_currentItem.clear();
}
