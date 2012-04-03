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

#include "conversationhistorywidget.h"

#include <QSplitter>
#include <QTableWidget>
#include <QHeaderView>
#include <QIcon>

#define DATE_FORMAT     "dd/MM/yy"
#define TIME_FORMAT     "hh:mm:ss"

#define MATCH_ICON      ":/icons/match_16x16.png"
#define NO_MATCH_ICON   ":/icons/no_match_16x16.png"
#define FB_ICON         ":/icons/facebook_24x24.png"
#define GMAIL_ICON      ":/icons/gmail_24x24.png"

enum DateContactTableColumns
{
    DateColumnn,
    UsernameColumn,
    DateContactTableTotalColumns
};

enum ConversationTableColumns
{
    TimeColumnn,
    MessageColumn,
    ResponseColumn,
    StatusColumn,
    ConversationTableTotalColumns
};

enum
{
    ConversationKeyRole = Qt::UserRole
};

//--------------------------------------------------------------------------------------------------
// Helpers
//--------------------------------------------------------------------------------------------------

namespace
{

QString hashKey(const QString &date, const QString &from)
{
    return date + from;
}

QString hashKey(const Lvk::BE::Conversation::Entry &entry)
{
    return hashKey(entry.dateTime.toString(DATE_FORMAT), entry.from);
}

} //namespace

//--------------------------------------------------------------------------------------------------
// ConversationHistoryWidget
//--------------------------------------------------------------------------------------------------

ConversationHistoryWidget::ConversationHistoryWidget(QWidget *parent)
    : QSplitter(Qt::Horizontal, parent)
{
    setupWidget();
}

//--------------------------------------------------------------------------------------------------

ConversationHistoryWidget::ConversationHistoryWidget(const Lvk::BE::Conversation &conv,
                                                     QWidget *parent)
    : QSplitter(Qt::Horizontal, parent)
{
    setupWidget();

    setConversation(conv);
}

//--------------------------------------------------------------------------------------------------

void ConversationHistoryWidget::setupWidget()
{
    // Date-Contact table
    m_dateContactTable = new QTableWidget(this);
    m_dateContactTable->setRowCount(0);
    m_dateContactTable->setColumnCount(DateContactTableTotalColumns);
    m_dateContactTable->setSortingEnabled(true);
    m_dateContactTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_dateContactTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_dateContactTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_dateContactTable->setAlternatingRowColors(true);
    m_dateContactTable->horizontalHeader()->setStretchLastSection(true);
    m_dateContactTable->verticalHeader()->hide();
    m_dateContactTable->setHorizontalHeaderLabels(QStringList()
                                                  << tr("Date")
                                                  << tr("Username"));
    m_dateContactTable->setColumnWidth(DateColumnn, 60);

    // Conversation table
    m_conversationTable = new QTableWidget(this);
    m_conversationTable->setRowCount(0);
    m_conversationTable->setColumnCount(ConversationTableTotalColumns);
    m_conversationTable->setSortingEnabled(true);
    m_conversationTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_conversationTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_conversationTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_conversationTable->setAlternatingRowColors(true);
    m_conversationTable->horizontalHeader()->setStretchLastSection(true);
    m_conversationTable->verticalHeader()->hide();
    m_conversationTable->setHorizontalHeaderLabels(QStringList()
                                                   << tr("Time")
                                                   << tr("Message")
                                                   << tr("Response")
                                                   << tr("Status"));
    m_conversationTable->setColumnWidth(TimeColumnn, 60);

    setSizes(QList<int>() << 50 << 150);

    connect(m_dateContactTable->selectionModel(),
            SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            SLOT(currentRowChanged(QModelIndex,QModelIndex)));
}


//--------------------------------------------------------------------------------------------------

void ConversationHistoryWidget::clear()
{
    m_dateContactTable->clearContents();
    m_dateContactTable->setRowCount(0);
    m_conversationTable->clearContents();
    m_conversationTable->setRowCount(0);
    m_entries.clear();
}

//--------------------------------------------------------------------------------------------------

void ConversationHistoryWidget::addConversationEntry(const Lvk::BE::Conversation::Entry &entry)
{
    QString key = hashKey(entry);

    if (!m_entries.contains(key)) {
        m_entries[key] = EntryList();

        addDateContactTableRow(entry);

        if (m_dateContactTable->rowCount() == 1) {
            m_dateContactTable->selectRow(0);
        }
    }

    m_entries[key].append(entry);

    if (m_dateContactTable->rowCount() == 1) {
        addConversationTableRow(entry);
    } else if (m_dateContactTable->selectionModel()->selectedIndexes().size() > 0) {
        int selectedRow = m_dateContactTable->selectionModel()->selectedIndexes().first().row();
        const QString &date = m_dateContactTable->item(selectedRow, DateColumnn)->text();
        const QString &user = m_dateContactTable->item(selectedRow, UsernameColumn)->text();

        if (key == hashKey(date, user)) {
            addConversationTableRow(entry);
        }
    }

}

//--------------------------------------------------------------------------------------------------

void ConversationHistoryWidget::currentRowChanged(const QModelIndex &current,
                                                  const QModelIndex &/*previous*/)
{
    m_conversationTable->clearContents();
    m_conversationTable->setRowCount(0);

    if (current.isValid()) {
        QTableWidgetItem *selectedItem = m_dateContactTable->item(current.row(), current.column());
        QString key = selectedItem->data(ConversationKeyRole).toString();
        const EntryList &entries = m_entries[key];

        for (int i = 0; i < entries.size(); ++i) {
           addConversationTableRow(entries[i]);
        }
    }
}

//--------------------------------------------------------------------------------------------------

void ConversationHistoryWidget::addConversationTableRow(const Lvk::BE::Conversation::Entry &entry)
{
    QString time = entry.dateTime.toString(TIME_FORMAT);
    QString match = entry.match ? tr("Ok") : tr("Fail!");

    int nextRow = m_conversationTable->rowCount();
    m_conversationTable->insertRow(nextRow);
    m_conversationTable->setItem(nextRow, TimeColumnn,    new QTableWidgetItem(time));
    m_conversationTable->setItem(nextRow, MessageColumn,  new QTableWidgetItem(entry.msg));
    m_conversationTable->setItem(nextRow, ResponseColumn, new QTableWidgetItem(entry.response));
    m_conversationTable->setItem(nextRow, StatusColumn,   new QTableWidgetItem(match));

    if (entry.match) {
        m_conversationTable->item(nextRow, StatusColumn)->setIcon(QIcon(MATCH_ICON));
    } else {
        m_conversationTable->item(nextRow, StatusColumn)->setIcon(QIcon(NO_MATCH_ICON));
    }
}

//--------------------------------------------------------------------------------------------------

void ConversationHistoryWidget::addDateContactTableRow(const Lvk::BE::Conversation::Entry &entry)
{
    QString date = entry.dateTime.toString(DATE_FORMAT);
    QString user = entry.from.split("@").at(0);

    int nextRow = m_dateContactTable->rowCount();
    m_dateContactTable->insertRow(nextRow);
    m_dateContactTable->setItem(nextRow, DateColumnn,    new QTableWidgetItem(date));
    m_dateContactTable->setItem(nextRow, UsernameColumn, new QTableWidgetItem(user));

    m_dateContactTable->item(nextRow, DateColumnn)->setData(ConversationKeyRole, hashKey(entry));
    m_dateContactTable->item(nextRow, UsernameColumn)->setData(ConversationKeyRole, hashKey(entry));

    if (entry.from.contains("@gmail.com")) {
        m_dateContactTable->item(nextRow, UsernameColumn)->setIcon(QIcon(GMAIL_ICON));
    } else if (entry.from.contains("@chat.facebook.com")) {
        m_dateContactTable->item(nextRow, UsernameColumn)->setIcon(QIcon(FB_ICON));
    }
}

//--------------------------------------------------------------------------------------------------

void ConversationHistoryWidget::setConversation(const Lvk::BE::Conversation &conv)
{
    clear();

    for (int i = 0; i < conv.entries().size(); ++i) {
        addConversationEntry(conv.entries()[i]);
    }
}


