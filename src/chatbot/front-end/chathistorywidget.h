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

#ifndef CHATHISTORYWIDGET_H
#define CHATHISTORYWIDGET_H

#include <QWidget>
#include <QHash>
#include <QList>

#include "conversation.h"

namespace Ui {
    class ChatHistoryWidget;
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
 * \brief The ChatHistoryWidget class provides a widget to display the chat history
 *        beetwen the chatbot and chat users
 *
 * The ChatHistoryWidget is used in the "History" tab.
 *
 * \see BE::Conversation
 */
class ChatHistoryWidget : public QWidget
{
    friend class ::TestMainWindow;

    Q_OBJECT

public:

    /**
     * Constructs an ChatHistoryWidget widget which is a child of \a parent.
     */
    explicit ChatHistoryWidget(QWidget *parent = 0);

    /**
     * Constructs an ChatHistoryWidget widget which is a child of \a parent and
     * it is initialized with conversation \a conv
     */
    explicit ChatHistoryWidget(const Lvk::BE::Conversation &conv, QWidget *parent = 0);

    /**
     * Destroys the object.
     */
    ~ChatHistoryWidget();

    /**
     * Clears the widget.
     */
    void clear();

    /**
     * Clears only conversations leaving the filter text unchanged.
     */
    void clearConversations();

    /**
     * Sets conversation \a conv
     */
    void setConversation(const Lvk::BE::Conversation &conv);

    /**
     * Adds a single conversation entry \a conv.
     */
    void addConversationEntry(const Lvk::BE::Conversation::Entry &entry);

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

    /**
     * This signal is emitted if the user has removed all the conversations
     */
    void removedAll();

    /**
     * This signal is emitted if the user has removed the conversation held with \a
     * username on \a date
     */
    void removed(const QDate &date, const QString &username);

private:

    Ui::ChatHistoryWidget *ui;

    typedef QList<Lvk::BE::Conversation::Entry> EntryList;
    QHash<QString, EntryList> m_entries;

    void setupTables();
    void setupMenus();
    void connectSignals();

    void addConversationTableRow(const Lvk::BE::Conversation::Entry &entry);
    void addDateContactTableRow(const Lvk::BE::Conversation::Entry &entry);
    void removeDateContactRow(int row);
    void filter(const QString &text);
    bool rowHasMatchStatus(int row);
    bool askConfirmation(const QString &title, const QString &text);

private slots:
    void onDateContactRowChanged(const QModelIndex &current, const QModelIndex &previous);
    void onConversationRowChanged(const QModelIndex &current, const QModelIndex &previous);
    void onCellDoubleClicked(int row, int col);
    void onFilterTextChanged(const QString &text);
    void onTeachRuleClicked();
    void onShowRuleClicked();

    void teachRuleWithDialog(int row);
    void showRuleWithDialog(int row);
    void removeAllWithDialog();
    void removeSelectedWithDialog();
};

/// @}

} // namespace FE

/// @}

} // namespace Lvk

#endif // CHATHISTORYWIDGET_H

