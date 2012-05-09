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

#include "rosterwidget.h"

#include <QListWidget>
#include <QCheckBox>
#include <QLayout>
#include <QVBoxLayout>

//--------------------------------------------------------------------------------------------------
// RosterWidget
//--------------------------------------------------------------------------------------------------

RosterWidget::RosterWidget(QWidget *parent) :
    QWidget(parent)
{
    setupWidget();

    connect(m_allUsersCheckBox, SIGNAL(clicked()), SLOT(onAllUsersClicked()));

    connect(m_rosterList, SIGNAL(itemClicked(QListWidgetItem*)),
            SLOT(onRosterItemClicked(QListWidgetItem*)));
}

//--------------------------------------------------------------------------------------------------

void RosterWidget::setupWidget()
{
    setLayout(new QVBoxLayout());
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_allUsersCheckBox = new QCheckBox();
    m_allUsersCheckBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_allUsersCheckBox->setCheckState(Qt::Checked);
    m_allUsersCheckBox->setText(tr("All users"));

    layout()->addWidget(m_allUsersCheckBox);

    m_rosterList = new QListWidget();
    m_rosterList->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    layout()->addWidget(m_rosterList);
}

//--------------------------------------------------------------------------------------------------

void RosterWidget::setRoster(const Lvk::BE::Roster &roster,
                             Qt::CheckState initialState /*= Qt::Checked*/)
{
    m_roster = roster;

    m_allUsersCheckBox->setCheckState(initialState);

    m_rosterList->clear();

    foreach (Lvk::BE::RosterItem entry, roster) {
        QString itemText = !entry.fullname.isEmpty() ? entry.fullname : entry.username;

        QListWidgetItem *item = new QListWidgetItem(itemText);
        item->setCheckState(initialState);

        m_rosterList->addItem(item);
    }
}

//--------------------------------------------------------------------------------------------------

const Lvk::BE::Roster &RosterWidget::roster()
{
    return m_roster;
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::Roster RosterWidget::checkedRoster()
{
    return filterRosteryBy(Qt::Checked);
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::Roster RosterWidget::uncheckedRoster()
{
    return filterRosteryBy(Qt::Unchecked);
}

//--------------------------------------------------------------------------------------------------

Qt::CheckState RosterWidget::allUsersCheckState()
{
    return m_allUsersCheckBox->checkState();
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::Roster RosterWidget::filterRosteryBy(Qt::CheckState state)
{
    Lvk::BE::Roster filteredRoster;
    for (int i = 0; i < m_rosterList->count(); ++i){
        if (m_rosterList->item(i)->checkState() == state) {
            filteredRoster.append(m_roster[i]);
        }
    }
    return filteredRoster;
}

//--------------------------------------------------------------------------------------------------

void RosterWidget::onAllUsersClicked()
{
    Qt::CheckState state = m_allUsersCheckBox->checkState();

    if (state == Qt::PartiallyChecked) {
        m_allUsersCheckBox->setCheckState(Qt::Checked);
        state = Qt::Checked;
    }

    for (int i = 0; i < m_rosterList->count(); ++i) {
        m_rosterList->item(i)->setCheckState(state);
    }

    emit selectionChanged();
}

//--------------------------------------------------------------------------------------------------

void RosterWidget::onRosterItemClicked(QListWidgetItem *item)
{
    Qt::CheckState itemState = item->checkState();

    // Update allUsersCheckBox check state

    m_allUsersCheckBox->setCheckState(itemState);
    for (int i = 0; i < m_rosterList->count(); ++i) {
        if (m_rosterList->item(i)->checkState() != itemState) {
               m_allUsersCheckBox->setCheckState(Qt::PartiallyChecked);
               break;
        }
    }

    emit selectionChanged();
}


