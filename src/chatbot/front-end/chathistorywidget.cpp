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
#include <QMenu>

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
    StatusColumn,
    TimeColumnn,
    MessageColumn,
    ResponseColumn,
    ConversationTableTotalColumns
};

enum
{
    HashKeyRole = Qt::UserRole,
    EntryMatchRole,
    EntryFromRole,
    EntryRuleIdRole
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

    clear();
    setupTables();
    setupMenus();
    connectSignals();
}

//--------------------------------------------------------------------------------------------------

Lvk::FE::ChatHistoryWidget::ChatHistoryWidget(const Lvk::BE::Conversation &conv, QWidget *parent)
    : QWidget(parent), ui(new Ui::ChatHistoryWidget)
{
    ui->setupUi(this);

    clear();
    setupTables();
    setupMenus();
    connectSignals();
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
    ui->dateContactTable->setColumnWidth(DateColumnn, 70);
    ui->dateContactTable->setHorizontalHeaderLabels(QStringList()
                                                    << tr("Date")
                                                    << tr("Username"));

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
    ui->conversationTable->setColumnWidth(TimeColumnn, 70);
    ui->conversationTable->setColumnWidth(StatusColumn, 22);
    ui->conversationTable->setColumnWidth(MessageColumn, 170);
    ui->conversationTable->setHorizontalHeaderLabels(QStringList()
                                                     << tr("")
                                                     << tr("Time")
                                                     << tr("Message")
                                                     << tr("Response"));
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ChatHistoryWidget::setupMenus()
{
    QMenu *menu = new QMenu(this);
    menu->addAction(ui->removeSelAction);
    menu->addAction(ui->removeAllAction);
    ui->removeHistoryButton->setMenu(menu);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ChatHistoryWidget::connectSignals()
{
    connect(ui->dateContactTable->selectionModel(),
            SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            SLOT(onDateContactRowChanged(QModelIndex,QModelIndex)));

    connect(ui->conversationTable->selectionModel(),
            SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            SLOT(onConversationRowChanged(QModelIndex,QModelIndex)));

    connect(ui->conversationTable,
            SIGNAL(cellDoubleClicked(int,int)),
            SLOT(onCellDoubleClicked(int,int)));

    connect(ui->filter,
            SIGNAL(textChanged(QString)),
            SLOT(onFilterTextChanged(QString)));

    // Toolbar signals

    connect(ui->teachRuleButton,     SIGNAL(clicked()),   SLOT(onTeachRuleClicked()));
    connect(ui->removeHistoryButton, SIGNAL(clicked()),   SLOT(removeSelectedWithDialog()));
    connect(ui->removeAllAction,     SIGNAL(triggered()), SLOT(removeAllWithDialog()));
    connect(ui->removeSelAction,     SIGNAL(triggered()), SLOT(removeSelectedWithDialog()));
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ChatHistoryWidget::clear()
{
    clearConversations();
    ui->filter->clear();
    ui->teachRuleButton->setEnabled(false);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ChatHistoryWidget::clearConversations()
{
    ui->dateContactTable->clearContents();
    ui->dateContactTable->setRowCount(0);
    ui->conversationTable->clearContents();
    ui->conversationTable->setRowCount(0);
    ui->removeHistoryButton->setEnabled(false);
    ui->filter->setEnabled(false);
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
    } else if (ui->dateContactTable->selectionModel()->hasSelection()) {
        QModelIndex selectedIndex = ui->dateContactTable->selectionModel()->currentIndex();
        const QString &selectedKey = selectedIndex.data(HashKeyRole).toString();

        if (key == selectedKey) {
            addConversationTableRow(entry);
        }
    }

    ui->removeHistoryButton->setEnabled(true);
    ui->filter->setEnabled(true);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ChatHistoryWidget::onDateContactRowChanged(const QModelIndex &current,
                                                         const QModelIndex &/*previous*/)
{
    ui->conversationTable->clearContents();
    ui->conversationTable->setRowCount(0);

    if (current.isValid()) {
        int row = current.row();

        // Fill table with new selected conversation

        QTableWidgetItem *selectedItem = ui->dateContactTable->item(row, current.column());
        QString key = selectedItem->data(HashKeyRole).toString();
        const EntryList &entries = m_entries[key];

        for (int i = 0; i < entries.size(); ++i) {
           addConversationTableRow(entries[i]);
        }

        // Filter table

        if (ui->filter->text().size() > 0) {
            filter(ui->filter->text());
        }

        // Update removeSelAction displayed text

        QString date = ui->dateContactTable->item(row, DateColumnn)->text();
        QString user = ui->dateContactTable->item(row, UsernameColumn)->text();
        QString actionTextFormat = tr("Remove conversation with %1 on %2");
        ui->removeSelAction->setText(actionTextFormat.arg(user, date));
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ChatHistoryWidget::onConversationRowChanged(const QModelIndex &current,
                                                          const QModelIndex &/*previous*/)
{
    if (!current.isValid()) {
        return;
    }

    if (!rowHasMatchStatus(current.row())) {
        ui->teachRuleButton->setEnabled(true);
    } else {
        ui->teachRuleButton->setEnabled(false);
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ChatHistoryWidget::addConversationTableRow(const Lvk::BE::Conversation::Entry &entry)
{
    QString time = entry.dateTime.toString(TIME_FORMAT);
    QString matchStr = entry.match ? tr("Response found") : tr("Response not found");

    int nextRow = ui->conversationTable->rowCount();
    ui->conversationTable->insertRow(nextRow);
    ui->conversationTable->setItem(nextRow, TimeColumnn,    new QTableWidgetItem(time));
    ui->conversationTable->setItem(nextRow, MessageColumn,  new QTableWidgetItem(entry.msg));
    ui->conversationTable->setItem(nextRow, ResponseColumn, new QTableWidgetItem(entry.response));
    ui->conversationTable->setItem(nextRow, StatusColumn,   new QTableWidgetItem(""));

    ui->conversationTable->item(nextRow, MessageColumn)->setData(Qt::ToolTipRole, entry.msg);
    ui->conversationTable->item(nextRow, ResponseColumn)->setData(Qt::ToolTipRole, entry.response);
    ui->conversationTable->item(nextRow, StatusColumn)->setData(Qt::ToolTipRole, matchStr);
    ui->conversationTable->item(nextRow, StatusColumn)->setData(EntryMatchRole, entry.match);
    ui->conversationTable->item(nextRow, StatusColumn)->setData(EntryRuleIdRole, entry.ruleId);

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

    ui->dateContactTable->item(nextRow, DateColumnn)->setData(HashKeyRole, hashKey(entry));
    ui->dateContactTable->item(nextRow, UsernameColumn)->setData(HashKeyRole, hashKey(entry));
    ui->dateContactTable->item(nextRow, UsernameColumn)->setData(EntryFromRole, entry.from);

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
    if (!rowHasMatchStatus(row)) {
        teachRuleWithDialog(row);
    } else {
        QString chatMsg = ui->conversationTable->item(row, MessageColumn)->text();

        QString title = tr("Show rule definition");
        QString text = QString(tr("Show rule definition for message: \"%1\" ?")).arg(chatMsg);

        if (askConfirmation(title, text)) {
            quint64 ruleId = ui->conversationTable->item(row, StatusColumn)->data(EntryRuleIdRole)
                    .toULongLong();

            emit showRule(ruleId);
        }
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ChatHistoryWidget::onTeachRuleClicked()
{
    QModelIndex selectedIndex= ui->conversationTable->selectionModel()->currentIndex();

    if (selectedIndex.isValid()) {
        teachRuleWithDialog(selectedIndex.row());
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ChatHistoryWidget::onFilterTextChanged(const QString &text)
{
    filter(text);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ChatHistoryWidget::teachRuleWithDialog(int row)
{
    QString chatMsg = ui->conversationTable->item(row, MessageColumn)->text();

    QString title = tr("Teach rule");
    QString text = QString(tr("Teach new rule for message: \"%1\" ?")).arg(chatMsg);

    if (askConfirmation(title, text)) {
        emit teachRule(chatMsg);
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ChatHistoryWidget::removeAllWithDialog()
{
    QString title = tr("Remove conversation");
    QString text  = tr("Are you sure you want to remove all conversations?");

    if (askConfirmation(title, text)) {
        clear();
        emit removedAll();
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ChatHistoryWidget::removeSelectedWithDialog()
{
    QModelIndex selectedIndex = ui->dateContactTable->selectionModel()->currentIndex();

    if (!selectedIndex.isValid()) {
        return;
    }

    int row = selectedIndex.row();

    QString date = ui->dateContactTable->item(row, DateColumnn)->text();
    QString user = ui->dateContactTable->item(row, UsernameColumn)->text();
    QString from = ui->dateContactTable->item(row, UsernameColumn)->data(EntryFromRole).toString();

    QString title = tr("Remove conversation");
    QString text  = tr("Are you sure you want to remove the conversation with %1 on %2?");

    if (askConfirmation(title, text.arg(user, date))) {
        removeDateContactRow(row);
        emit removed(QDate::fromString(date, DATE_FORMAT), from);
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ChatHistoryWidget::removeDateContactRow(int row)
{
    QString date = ui->dateContactTable->item(row, DateColumnn)->text();
    QString from = ui->dateContactTable->item(row, UsernameColumn)->data(EntryFromRole).toString();

    m_entries.remove(hashKey(date, from));
    ui->dateContactTable->removeRow(row);

    if (ui->dateContactTable->rowCount() == 0) {
        ui->removeHistoryButton->setEnabled(false);
        ui->filter->setEnabled(false);
    }
    ui->teachRuleButton->setEnabled(false);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ChatHistoryWidget::filter(const QString &text)
{
    for (int i = 0; i < ui->conversationTable->rowCount(); ++i) {
        bool match = false;
        for (int j = 0; j < ui->conversationTable->columnCount() && !match; ++j) {
            if (ui->conversationTable->item(i, j)->text().contains(text, false)) {
                match = true;
            }
        }
        ui->conversationTable->setRowHidden(i, !match);
    }
}

//--------------------------------------------------------------------------------------------------

bool Lvk::FE::ChatHistoryWidget::rowHasMatchStatus(int row)
{
    return ui->conversationTable->item(row, StatusColumn)->data(EntryMatchRole).toBool();
}

//--------------------------------------------------------------------------------------------------

bool Lvk::FE::ChatHistoryWidget::askConfirmation(const QString &title, const QString &text)
{
    int btn = QMessageBox::question(this, title, text, QMessageBox::Yes, QMessageBox::No);

    return btn == QMessageBox::Yes;
}

