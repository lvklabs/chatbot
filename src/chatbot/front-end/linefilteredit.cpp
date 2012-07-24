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

#include "linefilteredit.h"

#include <QPushButton>

//--------------------------------------------------------------------------------------------------
// LineFilterEdit
//--------------------------------------------------------------------------------------------------

Lvk::FE::LineFilterEdit::LineFilterEdit(QWidget *parent /*= 0*/)
    : QLineEdit(parent)
{
    m_clearButton = new QPushButton(this);
    m_clearButton->setPixmap(QPixmap(":/icons/clear_16x16.png"));
    m_clearButton->setFlat(true);
    m_clearButton->setCursor(QCursor(Qt::ArrowCursor));

    connect(m_clearButton, SIGNAL(clicked()), SLOT(onClearButtonPressed()));
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::LineFilterEdit::showEvent(QShowEvent *)
{
    m_clearButton->setGeometry(width() - m_clearButton->width(),
                       (height() - m_clearButton->height())/2,
                       m_clearButton->width(),
                       m_clearButton->height());
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::LineFilterEdit::onClearButtonPressed()
{
    QLineEdit::clear();
    setFocus();
}

