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

#ifndef LVK_FE_SCRIPTCOVERAGEWIDGET_H
#define LVK_FE_SCRIPTCOVERAGEWIDGET_H

#include "da-clue/analyzedscript.h"

#include <QWidget>

class QModelIndex;

namespace Ui {
    class ScriptCoverageWidget;
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
 * \brief The ScriptCoverageWidget class provides a widget to display the coverage of a list
 *        of scripts and the rules used
 */
class ScriptCoverageWidget : public QWidget
{
    Q_OBJECT

public:

    /**
     * Constructs an ScriptCoverageWidget widget which is a child of \a parent.
     */
    explicit ScriptCoverageWidget(QWidget *parent = 0);

    /**
     * Destroys the object.
     */
    ~ScriptCoverageWidget();

    /**
     * Displays the list of analyzed scripts \a ascripts
     */
    void setAnalyzedScripts(const Clue::AnalyzedList &scripts);

    /**
     * Clears the widget.
     */
    void clear();

private slots:

    void onScriptRowChanged(const QModelIndex &, const QModelIndex &);

private:
    Ui::ScriptCoverageWidget *ui;
    Clue::AnalyzedList m_scripts;

    void setupTables();
    void connectSignals();
    void addScriptRow(const QString &filename, float coverage);
};

/// @}

} // namespace FE

/// @}

} // namespace Lvk

#endif // LVK_FE_SCRIPTCOVERAGEWIDGET_H

