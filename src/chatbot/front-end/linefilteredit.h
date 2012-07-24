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

#ifndef LVK_FE_LINEFILTEREDIT_H
#define LVK_FE_LINEFILTEREDIT_H

#include <QLineEdit>

class QLabel;

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
 * \brief The LineFilterEdit class provides a widget with the same functionality as QLineEdit
 *        but contains a button to clear the text.
 *
 * This widget is very useful to implement text filters. For instance, it is used in RosterWidget
 * and ChatHistoryWidget classes.
 */
class LineFilterEdit : public QLineEdit
{
    Q_OBJECT

public:

    /**
     * Constructs an LineFilterEdit widget which is a child of \a parent.
     */
    explicit LineFilterEdit(QWidget *parent = 0);

protected:
    bool eventFilter(QObject *obj, QEvent *event);

    void showEvent(QShowEvent *);

    void resizeEvent(QResizeEvent *);

private:
    QLabel *m_clearWidget;

    void updateClearWidgetPos();
};

/// @}

} // namespace FE

/// @}

} // namespace Lvk


#endif // LVK_FE_LINEFILTEREDIT_H

