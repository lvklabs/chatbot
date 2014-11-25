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

#ifndef LVK_FE_CLUEWIDGET_H
#define LVK_FE_CLUEWIDGET_H

#include "back-end/appfacade.h"

#include <QWidget>

namespace Ui {
    class ClueWidget;
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
 * The ClueWidget class provides the main widget to display in the "Clue" tab
 */
class ClueWidget : public QWidget
{
    Q_OBJECT

public:

    /**
     * Constructs a ClueWidget object.
     */
    explicit ClueWidget(QWidget *parent = 0);

    /**
     * Sets \a appFacade as the new application facade for the widget. The widget does not
     * own the given pointers.
     */
    // TODO define a new interface instead of using AppFacade
    void setAppFacade(BE::AppFacade *appFacade);

    /**
     * Destroys the object
     */
    ~ClueWidget();

public slots:

    /**
     * Refreshes the widget
     */
    void refresh();

    /**
     * Opens a file dialog and imports the selected file
     */
    void import();

    /**
     * Clears the widget
     */
    void clear();

signals:

    /**
     * This signal is emitted whenever the "upload" button is pressed.
     */
    void upload();

    /**
     * This signal is emitted whenever the "Show rule definition" button is pressed.
     */
    void showRule(quint64 ruleId);

private slots:

    void onRemoveScript(const QString &filename);

private:
    Ui::ClueWidget *ui;
    BE::AppFacade  *m_appFacade;

    void showError(const QString &filename);
    void loadSettings();
    void saveSettings();
};

/// @}

} // namespace FE

/// @}

} // namespace Lvk

#endif // LVK_FE_CLUEWIDGET_H
