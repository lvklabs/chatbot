/*
 * Copyright (C) 2012 Andres Pagliano, Gabriel Miretti, Gonzalo Buteler,
 * Nestor Bustamante, Pablo Perez de Angelis
 *
 * This file is part of LVK Botmaster.
 *
 * LVK Botmaster is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * LVK Botmaster is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LVK Botmaster.  If not, see <http://www.gnu.org/licenses/>.
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
    explicit ConversationHistoryWidget(QWidget *parent = 0);

    explicit ConversationHistoryWidget(const Lvk::BE::Conversation &conv, QWidget *parent = 0);

    void clear();

    void setConversation(const Lvk::BE::Conversation &conv);

    void addConversationEntry(const Lvk::BE::Conversation::Entry &entry);

private:

    QTableWidget *m_dateContactTable;
    QTableWidget *m_conversationTable;

    typedef QList<Lvk::BE::Conversation::Entry> EntryList;
    QHash<QString, EntryList> m_entries;

    void setupWidget();

    void addConversationTableRow(const Lvk::BE::Conversation::Entry &entry);
    void addDateContactTableRow(const Lvk::BE::Conversation::Entry &entry);

private slots:
    void currentRowChanged(const QModelIndex &current, const QModelIndex &previous);
};

/// @}

} // namespace FE

/// @}

} // namespace Lvk

#endif // CONVERSATIONHISTORYWIDGET_H
