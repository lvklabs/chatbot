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
#include <QStringList>


//--------------------------------------------------------------------------------------------------
// AutoCompleteTextEdit
//--------------------------------------------------------------------------------------------------

Lvk::FE::AutocompleteTextEdit::AutocompleteTextEdit(QWidget *parent) :
    QLineEdit(parent), m_listWidget(new QListWidget(this))
{
    m_listWidget->setWindowFlags(Qt::ToolTip | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);

    connect(this, SIGNAL(textEdited(QString)), SLOT(onTargetTextEdited(QString)));
    connect(this, SIGNAL(lostFocus()),         SLOT(onTargetLostFocus()));

    ///////////////////////////////////////
    // TODO remove!
    m_strList.append("Andres Pagliano");
    m_strList.append("Andres Calamaro");
    m_strList.append("Andrea Prodan");
    m_strList.append("Luciana Benotti");
    m_strList.append("Emilia Echeveste");
    ///////////////////////////////////////
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::AutocompleteTextEdit::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();

    if (key == Qt::Key_Down && m_listWidget->currentRow() + 1 < m_listWidget->count()) {
        m_listWidget->setCurrentRow(m_listWidget->currentRow() + 1);
    }
    if (key == Qt::Key_Up && m_listWidget->currentRow() > 0) {
        m_listWidget->setCurrentRow(m_listWidget->currentRow() - 1);
    }
    if ((key == Qt::Key_Enter || key == Qt::Key_Return) && m_listWidget->currentRow() != -1) {
        setText(m_listWidget->currentItem()->text() + ", ");
        m_listWidget->hide();
    }

    QLineEdit::keyPressEvent(event);
}


//--------------------------------------------------------------------------------------------------

void Lvk::FE::AutocompleteTextEdit::setStringList(const QStringList &strList)
{
    m_strList = strList;
}

//--------------------------------------------------------------------------------------------------

const QStringList & Lvk::FE::AutocompleteTextEdit::stringList()
{
    return m_strList;
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::AutocompleteTextEdit::onTargetTextEdited(QString)
{
    QPoint pos = mapToGlobal(QPoint(0,0));

    m_listWidget->setGeometry(pos.x(), pos.y() + height(), 300, 200);

    m_listWidget->clear();

    QString target = text();

    if (target.size() > 0) {
        foreach (const QString &str, m_strList) {
            if (str.contains(target, false)) {
                m_listWidget->addItem(str);
            }
        }
    }

    if (m_listWidget->count() > 0) {
        m_listWidget->show();
    } else {
        m_listWidget->hide();
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::AutocompleteTextEdit::onTargetLostFocus()
{
    m_listWidget->hide();
}

