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

#include "front-end/linefilteredit.h"

#include <QLabel>
#include <QEvent>

#define ICON_SIZE                   16
#define ICON_RIGHT_MARGIN           5
#define CLEAR_DEFAULT_ICON          ":/icons/clear.png"
#define CLEAR_HOVER_ICON            ":/icons/clear_hover.png"


//--------------------------------------------------------------------------------------------------
// LineFilterEdit
//--------------------------------------------------------------------------------------------------

Lvk::FE::LineFilterEdit::LineFilterEdit(QWidget *parent /*= 0*/)
    : QLineEdit(parent)
{
    setTextMargins(0, 0, ICON_RIGHT_MARGIN + ICON_SIZE, 0);

    m_clearWidget = new QLabel(this);
    m_clearWidget->setPixmap(QPixmap(CLEAR_DEFAULT_ICON));
    m_clearWidget->setCursor(QCursor(Qt::ArrowCursor));
    m_clearWidget->setFocusPolicy(Qt::NoFocus);
    m_clearWidget->installEventFilter(this);
    m_clearWidget->setToolTip(tr("Clear text"));
}

//--------------------------------------------------------------------------------------------------

bool Lvk::FE::LineFilterEdit::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == m_clearWidget) {
        if (event->type() == QEvent::MouseButtonRelease) {
            QLineEdit::clear();
            return true;
        } else if (event->type() == QEvent::Enter) {
            m_clearWidget->setPixmap(QPixmap(isEnabled() ? CLEAR_HOVER_ICON : CLEAR_DEFAULT_ICON));
            return true;
        } else if (event->type() == QEvent::Leave) {
            m_clearWidget->setPixmap(QPixmap(CLEAR_DEFAULT_ICON));
            return true;
        } else {
            return m_clearWidget->eventFilter(obj, event);
        }
    } else {
        return QLineEdit::eventFilter(obj, event);
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::LineFilterEdit::showEvent(QShowEvent *event)
{
    updateClearWidgetPos();

    QLineEdit::showEvent(event);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::LineFilterEdit::resizeEvent(QResizeEvent *event)
{
    updateClearWidgetPos();

    QLineEdit::resizeEvent(event);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::LineFilterEdit::updateClearWidgetPos()
{
    m_clearWidget->move(width() - ICON_SIZE - ICON_RIGHT_MARGIN, (height() - ICON_SIZE)/2);
}


