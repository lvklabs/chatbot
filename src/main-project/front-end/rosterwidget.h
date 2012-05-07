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

#include <QWidget>

#include "roster.h"

class QListWidget;
class QCheckBox;

/**
 * \brief The ConversationHistoryWidget provides a widget to display a roster of chat users
 */

class RosterWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RosterWidget(QWidget *parent = 0);

    void setRoster(const Lvk::BE::Roster &roster);

private:
    QCheckBox *m_allUsersCheckBox;
    QListWidget *m_rosterList;

    void setupWidget();
};

#endif // ROSTERWIDGET_H
