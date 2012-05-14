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
#include <QLineEdit>
#include <QLabel>

//--------------------------------------------------------------------------------------------------
// RosterWidget
//--------------------------------------------------------------------------------------------------

RosterWidget::RosterWidget(QWidget *parent) :
    QWidget(parent), m_allUsersCheckBox(new QCheckBox()), m_rosterListWidget(new QListWidget()),
    m_filterText(new QLineEdit())
{
    setupWidget();

    connect(m_allUsersCheckBox, SIGNAL(clicked()), SLOT(onAllUsersClicked()));

    connect(m_rosterListWidget, SIGNAL(itemClicked(QListWidgetItem*)),
            SLOT(onRosterItemClicked(QListWidgetItem*)));

    connect(m_filterText,       SIGNAL(textChanged(QString)), SLOT(onFilterTextChanged(QString)));
}

//--------------------------------------------------------------------------------------------------

void RosterWidget::setupWidget()
{
    setLayout(new QVBoxLayout());
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_allUsersCheckBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_allUsersCheckBox->setCheckState(Qt::Checked);
    m_allUsersCheckBox->setText(tr("All users"));

    layout()->addWidget(m_allUsersCheckBox);

    m_rosterListWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    layout()->addWidget(m_rosterListWidget);

    layout()->addWidget(new QLabel(tr("Find:")));

    layout()->addWidget(m_filterText);
}

//--------------------------------------------------------------------------------------------------

void RosterWidget::setRoster(const Lvk::BE::Roster &roster,
                             Qt::CheckState initialState /*= Qt::Checked*/)
{
    m_roster = roster;

    m_allUsersCheckBox->setCheckState(initialState);

    m_rosterListWidget->clear();

    foreach (Lvk::BE::RosterItem rosterItem, roster) {
        QString text = !rosterItem.fullname.isEmpty() ? rosterItem.fullname : rosterItem.username;

        QListWidgetItem *listItem = new QListWidgetItem(text);
        listItem->setCheckState(initialState);

        m_rows[rosterItem.username] = m_rosterListWidget->count();

        m_rosterListWidget->addItem(listItem);
    }
}

//--------------------------------------------------------------------------------------------------

void RosterWidget::setRoster(const Lvk::BE::Roster &roster, const Lvk::BE::Roster &uncheckedSubset)
{
    setRoster(roster, Qt::Checked);

    // uncheck subset

    foreach (const Lvk::BE::RosterItem &item, uncheckedSubset) {
        QHash<QString, int>::const_iterator it = m_rows.find(item.username);
        if (it != m_rows.end()) {
            m_rosterListWidget->item(m_rows[item.username])->setCheckState(Qt::Unchecked);
        }
    }

    // update all-users check box state

    for (int i = 0; i < m_rosterListWidget->count(); ++i) {
        Qt::CheckState itemCheckState = m_rosterListWidget->item(i)->checkState();
        if (i == 0) {
            m_allUsersCheckBox->setCheckState(itemCheckState);
        } else if (itemCheckState != m_allUsersCheckBox->checkState()) {
           m_allUsersCheckBox->setCheckState(Qt::PartiallyChecked);
           break;
        }
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

void RosterWidget::clear()
{
    m_rosterListWidget->clear();
    m_roster.clear();
    m_rows.clear();
    m_filterText->clear();
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::Roster RosterWidget::filterRosteryBy(Qt::CheckState state)
{
    Lvk::BE::Roster filteredRoster;
    for (int i = 0; i < m_rosterListWidget->count(); ++i){
        if (m_rosterListWidget->item(i)->checkState() == state) {
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

    for (int i = 0; i < m_rosterListWidget->count(); ++i) {
        m_rosterListWidget->item(i)->setCheckState(state);
    }

    emit selectionChanged();
}

//--------------------------------------------------------------------------------------------------

void RosterWidget::onRosterItemClicked(QListWidgetItem *item)
{
    Qt::CheckState itemState = item->checkState();

    // Update allUsersCheckBox check state

    m_allUsersCheckBox->setCheckState(itemState);
    for (int i = 0; i < m_rosterListWidget->count(); ++i) {
        if (m_rosterListWidget->item(i)->checkState() != itemState) {
               m_allUsersCheckBox->setCheckState(Qt::PartiallyChecked);
               break;
        }
    }

    emit selectionChanged();
}

//--------------------------------------------------------------------------------------------------

void RosterWidget::onFilterTextChanged(const QString &)
{
    for (int i = 0; i < m_rosterListWidget->count(); ++i) {
        QListWidgetItem *item = m_rosterListWidget->item(i);
        if (item->text().contains(m_filterText->text(), false)) {
            item->setHidden(false);
        } else {
            item->setHidden(true);
        }
    }
}


