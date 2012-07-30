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

#ifndef ROSTERWIDGET_H
#define ROSTERWIDGET_H


#include <QString>
#include <QWidget>
#include <QHash>

#include "back-end/roster.h"

class QListWidget;
class QCheckBox;
class QListWidgetItem;
class QLineEdit;

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
 * \brief The RosterWidget class provides a widget to display, filter and select chat users.
 *
 * In the chat jargon, "roster" means "list of contacts".
 * The RosterWidget is used in the chat connection tab to select which contacts the chatbot can
 * talk to.
 *
 * The widget consists of a list of contacts where each item is checkeable, an input textbox to
 * filter contacts and a global checkbox to check/uncheck all items. The widget provides methods
 * to get and set the check state of the items.
 */
class RosterWidget : public QWidget
{
    Q_OBJECT

public:

    /**
     * Constructs an RosterWidget widget which is a child of \a parent.
     */
    explicit RosterWidget(QWidget *parent = 0);

    /**
     * Sets the given \a roster and sets all items in the list with state \a initialState
     */
    void setRoster(const Lvk::BE::Roster &roster, Qt::CheckState initialState = Qt::Checked);

    /**
     * Sets the given \a oster and sets all items in the list with state \a checked with
     * exception of those in \a uncheckedSubset
     */
    void setRoster(const Lvk::BE::Roster &roster, const Lvk::BE::Roster &uncheckedSubset);

    /**
     * Returns the full roster.
     */
    const Lvk::BE::Roster &roster();

    /**
     * Returns the subset of the roster with state \a checked.
     */
    Lvk::BE::Roster checkedRoster();

    /**
     * Returns the subset of the roster with state \a unchecked.
     */
    Lvk::BE::Roster uncheckedRoster();

    /**
     * Returns the state of the "All users" checkbox.
     */
    Qt::CheckState allUsersCheckState();

    /**
     * Clears the widget.
     */
    void clear();

signals:

    /**
     * This signal is emitted every time the selection of items in the list has changed.
     */
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

/// @}

} // namespace FE

/// @}

} // namespace Lvk

#endif // ROSTERWIDGET_H
