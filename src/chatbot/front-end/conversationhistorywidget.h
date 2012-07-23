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

#ifndef CONVERSATIONHISTORYWIDGET_H
#define CONVERSATIONHISTORYWIDGET_H

#include <QSplitter>
#include <QHash>
#include <QList>

#include "conversation.h"

class QTableWidget;
class QModelIndex;
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
 * \brief The ConversationHistoryWidget class provides a widget to display conversation history
 *        beetwen the chatbot and chat users
 *
 * The ConversationHistoryWidget is used in the "History" tab.
 *
 * \see BE::Conversation
 */
class ConversationHistoryWidget : public QSplitter
{
    friend class ::TestMainWindow;

    Q_OBJECT

public:

    /**
     * Constructs an ConversationHistoryWidget widget which is a child of \a parent.
     */
    explicit ConversationHistoryWidget(QWidget *parent = 0);

    /**
     * Constructs an ConversationHistoryWidget widget which is a child of \a parent and
     * it is initialized with conversation \a conv
     */
    explicit ConversationHistoryWidget(const Lvk::BE::Conversation &conv, QWidget *parent = 0);

    /**
     * Clears the widget
     */
    void clear();

    /**
     * Sets conversation \a conv
     */
    void setConversation(const Lvk::BE::Conversation &conv);

    /**
     * Adds a single conversation entry \a conv
     */
    void addConversationEntry(const Lvk::BE::Conversation::Entry &entry);

signals:

    /**
     * This signal is emitted if the user wants to teach a new rule from history with rule input
     * \a input
     */
    void teachRule(const QString &input);

private:

    QTableWidget *m_dateContactTable;
    QTableWidget *m_conversationTable;

    typedef QList<Lvk::BE::Conversation::Entry> EntryList;
    QHash<QString, EntryList> m_entries;

    void setupWidget();

    void addConversationTableRow(const Lvk::BE::Conversation::Entry &entry);
    void addDateContactTableRow(const Lvk::BE::Conversation::Entry &entry);

private slots:
    void onCurrentRowChanged(const QModelIndex &current, const QModelIndex &previous);
    void onCellDoubleClicked(int row, int col);

};

/// @}

} // namespace FE

/// @}

} // namespace Lvk

#endif // CONVERSATIONHISTORYWIDGET_H
