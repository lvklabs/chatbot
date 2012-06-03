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

#ifndef ROSTERWIDGET_H
#define ROSTERWIDGET_H


#include <QString>
#include <QWidget>
#include <QHash>

#include "roster.h"

class QListWidget;
class QCheckBox;
class QListWidgetItem;
class QLineEdit;

namespace Lvk
{

namespace FE
{

/**
 * \brief The ConversationHistoryWidget provides a widget to display a roster of chat users
 */
class RosterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RosterWidget(QWidget *parent = 0);

    void setRoster(const Lvk::BE::Roster &roster, Qt::CheckState initialState = Qt::Checked);

    void setRoster(const Lvk::BE::Roster &roster, const Lvk::BE::Roster &uncheckedSubset);

    const Lvk::BE::Roster &roster();

    Lvk::BE::Roster checkedRoster();

    Lvk::BE::Roster uncheckedRoster();

    Qt::CheckState allUsersCheckState();

    void clear();

signals:
    void selectionChanged();

private slots:
    void onAllUsersClicked();
    void onRosterItemClicked(QListWidgetItem *item);
    void onFilterTextChanged(const QString &);

private:
    QCheckBox *m_allUsersCheckBox;
    QListWidget *m_rosterListWidget;
    QLineEdit *m_filterText;
    Lvk::BE::Roster m_roster;
    QHash<QString, int> m_rows; // username -> list row

    void setupWidget();
    Lvk::BE::Roster filterRosteryBy(Qt::CheckState);
};

} // namespace FE

} // namespace Lvk

#endif // ROSTERWIDGET_H
