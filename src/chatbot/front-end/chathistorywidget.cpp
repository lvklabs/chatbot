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

#include "chathistorywidget.h"
#include "ui_chathistorywidget.h"

#include <QIcon>
#include <QMessageBox>

#define DATE_FORMAT     "dd/MM/yy"
#define TIME_FORMAT     "hh:mm:ss"

#define MATCH_ICON      ":/icons/match_16x16.png"
#define NO_MATCH_ICON   ":/icons/no_match_16x16.png"
#define FB_ICON         ":/icons/facebook_24x24.png"
#define GMAIL_ICON      ":/icons/gmail_24x24.png"
#define LOCAL_TEST_ICON ":/icons/app_icon"

#define USERNAME_START_TOKEN  "<"
#define USERNAME_END_TOKEN    ">"


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
    ConversationKeyRole = Qt::UserRole,
    RuleMatchRole
};


//--------------------------------------------------------------------------------------------------
// Helpers
//--------------------------------------------------------------------------------------------------

namespace
{

// builds a hash key with the given data
QString hashKey(const QString &date, const QString &from)
{
    return date + from;
}

// builds a hash key with the given data
QString hashKey(const Lvk::BE::Conversation::Entry &entry)
{
    return hashKey(entry.dateTime.toString(DATE_FORMAT), entry.from);
}

// gets username from strings with format "FullName <Username>" or "Username"
QString getUsername(const QString &from)
{
    QString username;

    if (from.contains(USERNAME_START_TOKEN)) {
         username = from.split(USERNAME_START_TOKEN).at(1).trimmed();
         username.remove(USERNAME_END_TOKEN);
    } else {
        username = from;
    }
    return username;
}

// gets fullname from strings with format "FullName <Username>" or "Username"
QString getFullname(const QString &from)
{
    QString fullname;

    if (from.contains(USERNAME_START_TOKEN)) {
         fullname = from.split(USERNAME_START_TOKEN).at(0).trimmed();
    }

    return fullname;
}

} // namespace


//--------------------------------------------------------------------------------------------------
// ChatHistoryWidget
//--------------------------------------------------------------------------------------------------

Lvk::FE::ChatHistoryWidget::ChatHistoryWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::ChatHistoryWidget)
{
    ui->setupUi(this);

    setupTables();
}

//--------------------------------------------------------------------------------------------------

Lvk::FE::ChatHistoryWidget::ChatHistoryWidget(const Lvk::BE::Conversation &conv, QWidget *parent)
    : QWidget(parent), ui(new Ui::ChatHistoryWidget)
{
    ui->setupUi(this);

    setupTables();

    setConversation(conv);
}

//--------------------------------------------------------------------------------------------------

Lvk::FE::ChatHistoryWidget::~ChatHistoryWidget()
{
    delete ui;
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ChatHistoryWidget::setupTables()
{
    ui->splitter->setSizes(QList<int>() << (width()*1/3) << (width()*2/3));

    // Date-Contact table
    ui->dateContactTable->setRowCount(0);
    ui->dateContactTable->setColumnCount(DateContactTableTotalColumns);
    ui->dateContactTable->setSortingEnabled(true);
    ui->dateContactTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->dateContactTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->dateContactTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->dateContactTable->setAlternatingRowColors(true);
    ui->dateContactTable->horizontalHeader()->setStretchLastSection(true);
    ui->dateContactTable->verticalHeader()->hide();
    ui->dateContactTable->setHorizontalHeaderLabels(QStringList()
                                                    << tr("Date")
                                                    << tr("Username"));
    ui->dateContactTable->setColumnWidth(DateColumnn, 70);

    // Conversation table
    ui->conversationTable->setRowCount(0);
    ui->conversationTable->setColumnCount(ConversationTableTotalColumns);
    ui->conversationTable->setSortingEnabled(true);
    ui->conversationTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->conversationTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->conversationTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->conversationTable->setAlternatingRowColors(true);
    ui->conversationTable->horizontalHeader()->setStretchLastSection(true);
    ui->conversationTable->verticalHeader()->hide();
    ui->conversationTable->setHorizontalHeaderLabels(QStringList()
                                                     << tr("Time")
                                                     << tr("Message")
                                                     << tr("Response")
                                                     << tr("Status"));
    ui->conversationTable->setColumnWidth(TimeColumnn, 70);


    connect(ui->dateContactTable->selectionModel(),
            SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            SLOT(onCurrentRowChanged(QModelIndex,QModelIndex)));

    connect(ui->conversationTable,
            SIGNAL(cellDoubleClicked(int,int)),
            SLOT(onCellDoubleClicked(int,int)));
}


//--------------------------------------------------------------------------------------------------

void Lvk::FE::ChatHistoryWidget::clear()
{
    ui->dateContactTable->clearContents();
    ui->dateContactTable->setRowCount(0);
    ui->conversationTable->clearContents();
    ui->conversationTable->setRowCount(0);
    m_entries.clear();
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ChatHistoryWidget::addConversationEntry(const Lvk::BE::Conversation::Entry &entry)
{
    QString key = hashKey(entry);

    if (!m_entries.contains(key)) {
        m_entries[key] = EntryList();

        addDateContactTableRow(entry);

        if (ui->dateContactTable->rowCount() == 1) {
            ui->dateContactTable->selectRow(0);
        }
    }

    m_entries[key].append(entry);

    if (ui->dateContactTable->rowCount() == 1) {
        addConversationTableRow(entry);
    } else if (ui->dateContactTable->selectionModel()->selectedIndexes().size() > 0) {
        QModelIndex selectedIndex = ui->dateContactTable->selectionModel()->selectedIndexes().first();
        const QString &selectedKey = selectedIndex.data(ConversationKeyRole).toString();

        if (key == selectedKey) {
            addConversationTableRow(entry);
        }
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ChatHistoryWidget::onCurrentRowChanged(const QModelIndex &current,
                                                     const QModelIndex &/*previous*/)
{
    ui->conversationTable->clearContents();
    ui->conversationTable->setRowCount(0);

    if (current.isValid()) {
        QTableWidgetItem *selectedItem = ui->dateContactTable->item(current.row(), current.column());
        QString key = selectedItem->data(ConversationKeyRole).toString();
        const EntryList &entries = m_entries[key];

        for (int i = 0; i < entries.size(); ++i) {
           addConversationTableRow(entries[i]);
        }
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ChatHistoryWidget::addConversationTableRow(const Lvk::BE::Conversation::Entry &entry)
{
    QString time = entry.dateTime.toString(TIME_FORMAT);
    QString match = entry.match ? tr("Ok") : tr("No response found!");

    int nextRow = ui->conversationTable->rowCount();
    ui->conversationTable->insertRow(nextRow);
    ui->conversationTable->setItem(nextRow, TimeColumnn,    new QTableWidgetItem(time));
    ui->conversationTable->setItem(nextRow, MessageColumn,  new QTableWidgetItem(entry.msg));
    ui->conversationTable->setItem(nextRow, ResponseColumn, new QTableWidgetItem(entry.response));
    ui->conversationTable->setItem(nextRow, StatusColumn,   new QTableWidgetItem(match));

    ui->conversationTable->item(nextRow, MessageColumn)->setData(Qt::ToolTipRole, entry.msg);
    ui->conversationTable->item(nextRow, ResponseColumn)->setData(Qt::ToolTipRole, entry.response);
    ui->conversationTable->item(nextRow, StatusColumn)->setData(RuleMatchRole, entry.match);

    if (entry.match) {
        ui->conversationTable->item(nextRow, StatusColumn)->setIcon(QIcon(MATCH_ICON));
    } else {
        ui->conversationTable->item(nextRow, StatusColumn)->setIcon(QIcon(NO_MATCH_ICON));
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ChatHistoryWidget::addDateContactTableRow(const Lvk::BE::Conversation::Entry &entry)
{
    QString date = entry.dateTime.toString(DATE_FORMAT);
    QString username = getUsername(entry.from);
    QString fullname = getFullname(entry.from);

    if (fullname.isEmpty()) {
        fullname = username;
    }

    int nextRow = ui->dateContactTable->rowCount();
    ui->dateContactTable->insertRow(nextRow);
    ui->dateContactTable->setItem(nextRow, DateColumnn,    new QTableWidgetItem(date));
    ui->dateContactTable->setItem(nextRow, UsernameColumn, new QTableWidgetItem(fullname));

    ui->dateContactTable->item(nextRow, DateColumnn)->setData(ConversationKeyRole, hashKey(entry));
    ui->dateContactTable->item(nextRow, UsernameColumn)->setData(ConversationKeyRole, hashKey(entry));

    if (username.contains("@gmail.com")) {
        ui->dateContactTable->item(nextRow, UsernameColumn)->setIcon(QIcon(GMAIL_ICON));
    } else if (username.contains("@chat.facebook.com")) {
        ui->dateContactTable->item(nextRow, UsernameColumn)->setIcon(QIcon(FB_ICON));
    } else if (username.contains(tr("(test)"))) {
        ui->dateContactTable->item(nextRow, UsernameColumn)->setIcon(QIcon(LOCAL_TEST_ICON));
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ChatHistoryWidget::setConversation(const Lvk::BE::Conversation &conv)
{
    clear();

    for (int i = 0; i < conv.entries().size(); ++i) {
        addConversationEntry(conv.entries()[i]);
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ChatHistoryWidget::onCellDoubleClicked(int row, int /*col*/)
{
    if (!ui->conversationTable->item(row, StatusColumn)->data(RuleMatchRole).toBool()) {
        QString msg = ui->conversationTable->item(row, MessageColumn)->text();
        QString dialogTitle = tr("Teach rule");
        QString dialogText = QString(tr("Teach new rule for message: \"%1\" ?")).arg(msg);

        int button = QMessageBox::question(this, dialogTitle, dialogText, QMessageBox::Yes,
                                           QMessageBox::No);

        if (button == QMessageBox::Yes) {
            emit teachRule(msg);
        }
    }
}


