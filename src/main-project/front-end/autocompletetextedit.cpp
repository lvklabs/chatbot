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

#include "autocompletetextedit.h"

#include <QListWidget>
#include <QListWidgetItem>
#include <QKeyEvent>


//--------------------------------------------------------------------------------------------------
// AutoCompleteTextEdit
//--------------------------------------------------------------------------------------------------

Lvk::FE::AutocompleteTextEdit::AutocompleteTextEdit(QWidget *parent) :
    QLineEdit(parent), m_list(new QListWidget(this))
{
    m_list->setWindowFlags(Qt::ToolTip | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);

    m_list->addItem("Andres Pagliano");
    m_list->addItem("Andres Calamaro");
    m_list->addItem("Andrea Prodan");
    m_list->addItem("Luciana Benotti");
    m_list->addItem("Emilia Echeveste");

    connect(this, SIGNAL(textEdited(QString)), SLOT(onTargetTextEdited(QString)));
    connect(this, SIGNAL(lostFocus()),         SLOT(onTargetLostFocus()));
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::AutocompleteTextEdit::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();

    if (key == Qt::Key_Down && m_list->currentRow() + 1 < m_list->count()) {
        m_list->setCurrentRow(m_list->currentRow() + 1);
    }
    if (key == Qt::Key_Up && m_list->currentRow() > 0) {
        m_list->setCurrentRow(m_list->currentRow() - 1);
    }
    if ((key == Qt::Key_Enter || key == Qt::Key_Return) && m_list->currentRow() != -1) {
        setText(m_list->currentItem()->text() + ", ");
        m_list->hide();
    }

    QLineEdit::keyPressEvent(event);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::AutocompleteTextEdit::onTargetTextEdited(QString)
{
    QPoint pos = mapToGlobal(QPoint(0,0));

    m_list->setGeometry(pos.x(), pos.y() + height(), 300, 200);

    int matches = 0;

    if (text().size() > 0) {
        for (int i = 0; i < m_list->count(); ++i) {
            QListWidgetItem *item = m_list->item(i);
            bool match = item->text().contains(text(), false);
            if (match) {
                matches++;
            }
            item->setHidden(!match);
        }
    }

    if (matches > 0) {
        m_list->show();
    } else {
        m_list->hide();
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::AutocompleteTextEdit::onTargetLostFocus()
{
    m_list->hide();
}

