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

#include <QWidget>
#include <QHash>
#include <QList>

#include "common/conversation.h"

namespace Ui {
    class ScriptCoverageWidget;
}

class QTableWidget;
class QModelIndex;
class QSplitter;
class TestMainWindow;

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
 * \brief The ScriptCoverageWidget class provides a widget to display ...
 */
class ScriptCoverageWidget : public QWidget
{
    friend class ::TestMainWindow;

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
     * Clears the widget.
     */
    void clear();

signals:

    /**
     * This signal is emitted if the user wants to teach a new rule from history with rule input
     * \a input
     */
    void teachRule(const QString &input);

    /**
     * This signal is emitted if the user wants to see the definition of a rule that has matched.
     */
    void showRule(quint64 ruleId);

private:

    Ui::ScriptCoverageWidget *ui;

    typedef QList<Lvk::Cmn::Conversation::Entry> EntryList;
    QHash<QString, EntryList> m_entries;

    void setupTables();
    void connectSignals();

    void addConversationTableRow(const Lvk::Cmn::Conversation::Entry &entry);
    void addDateContactTableRow(const Lvk::Cmn::Conversation::Entry &entry);
    void removeDateContactRow(int row);
    void filter(const QString &text);
    bool rowHasMatchStatus(int row);
    bool askConfirmation(const QString &title, const QString &text);

private slots:
//    void onDateContactRowChanged(const QModelIndex &current, const QModelIndex &previous);
//    void onConversationRowChanged(const QModelIndex &current, const QModelIndex &previous);
    void onCellDoubleClicked(int row, int col);
    void onFilterTextChanged(const QString &text);
    void onTeachRuleClicked();
    void onShowRuleClicked();

    void teachRuleWithDialog(int row);
    void showRuleWithDialog(int row);
};

/// @}

} // namespace FE

/// @}

} // namespace Lvk

#endif // LVK_FE_SCRIPTCOVERAGEWIDGET_H

