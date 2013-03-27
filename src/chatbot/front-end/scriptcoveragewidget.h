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
#include "back-end/rule.h"

#include <QWidget>
#include <QList>

class QModelIndex;
class QUrl;
class QSplitter;

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
     * Returns the widget splitter sizes
     */
    QList<int> splitterSizes() const;

    /**
     * Returns the wiget splitter sizes
     */
    void setSplitterSizes(const QList<int> &sizes);

    /**
     * Displays the list of analyzed scripts \a ascripts
     */
    void setAnalyzedScripts(const Clue::AnalyzedList &scripts, const BE::Rule *root);

    /**
     * Returns the index of the currently selected script
     */
    int currentScript();

    /**
     * Sets the \a i-th script as the currently selected script
     */
    void setCurrentScript(int i);

    /**
     * Sets the current category group box as \a visible. By default is not visible.
     */
    void setCategoryVisible(bool visible);

    /**
     * Clears the widget.
     */
    void clear();

signals:

    /**
     * This signal is emitted whenever the "Show rule definition" button is pressed.
     */
    void showRule(quint64 ruleId);

private slots:

    void onScriptRowChanged(const QModelIndex &, const QModelIndex &);
    void onAnchorClicked(const QUrl &url);
    void onShowRuleDefClicked();
    void onSplitterMoved(int, int);

private:
    ScriptCoverageWidget(const ScriptCoverageWidget&);
    ScriptCoverageWidget & operator=(const ScriptCoverageWidget&);

    Ui::ScriptCoverageWidget *ui;
    Clue::AnalyzedList m_scripts;
    QString m_detective;
    const BE::Rule *m_root;
    QList<int> m_sizes;
    bool m_collapseDef;
    bool m_categoryVisible;

    void setupTables();
    void connectSignals();
    void addScriptRow(const QString &filename, float coverage);
    void showScript(int i);
    void showRuleUsed(int i, int j);
    void showHint(const QString &hint);
    void showRuleUsedColumn(bool show);
    void showCurrentCategory(const QString topic);
    const BE::Rule *findRule(quint64 ruleId);
};

/// @}

} // namespace FE

/// @}

} // namespace Lvk

#endif // LVK_FE_SCRIPTCOVERAGEWIDGET_H

