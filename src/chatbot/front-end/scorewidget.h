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

#ifndef LVK_FE_SCOREWIDGET_H
#define LVK_FE_SCOREWIDGET_H

#include <QWidget>

#include "back-end/score.h"

namespace Ui {
    class ScoreWidget;
}

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
 * \brief The ScoreWidget class provides a widget to display the score of the chatbot
 */
class ScoreWidget : public QWidget
{
    Q_OBJECT

public:

    /**
     * Constructs an ScoreWidget widget which is a child of the given parent.
     */
    explicit ScoreWidget(QWidget *parent = 0);

    /**
     * Destroys the object
     */
    ~ScoreWidget();

    /**
     * Sets the score
     */
    void setScore(const BE::Score &score);

    /**
     * Clears the widget
     */
    void clear();

private:
    Ui::ScoreWidget *ui;
};

/// @}

} // namespace FE

/// @}

} // namespace Lvk

#endif // LVK_FE_SCOREWIDGET_H
