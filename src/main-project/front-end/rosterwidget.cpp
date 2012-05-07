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

void RosterWidget::setRoster(const Lvk::BE::Roster &roster)
{
    foreach (Lvk::BE::RosterItem entry, roster) {
        QString itemText = entry.fullname;

        if (itemText.isEmpty()) {
            itemText = entry.username;
        }

        QListWidgetItem *item = new QListWidgetItem(itemText);
        item->setCheckState(Qt::Checked);

        m_rosterList->addItem(item);
    }
}
