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

#include "front-end/scriptcoveragewidget.h"
#include "ui_scriptcoveragewidget.h"

#include <QIcon>
#include <QMessageBox>
#include <QMenu>

#define MATCH_ICON      ":/icons/match.png"
#define NO_MATCH_ICON   ":/icons/no_match.png"


enum ScriptsTableColumns
{
    ScriptNameColumnn,
    ScriptCoverageColumn,
    ScriptsTableTotalColumns
};

enum ScriptDetailsTableColumns
{
    StatusColumn,
    CharacterColumnn,
    MessageColumn,
    RuleColumn,
    ScriptDetailsTableTotalColumns
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

//// builds a hash key with the given data
//QString hashKey(const QString &date, const QString &from)
//{
//    return date + from;
//}

//// builds a hash key with the given data
//QString hashKey(const Lvk::Cmn::Conversation::Entry &entry)
//{
//    return hashKey(entry.dateTime.toString(DATE_FORMAT), entry.from);
//}

//// gets username from strings with format "FullName <Username>" or "Username"
//QString getUsername(const QString &from)
//{
//    QString username;

//    if (from.contains(USERNAME_START_TOKEN)) {
//         username = from.split(USERNAME_START_TOKEN).at(1).trimmed();
//         username.(USERNAME_END_TOKEN);
//    } else {
//        username = from;
//    }
//    return username;
//}

//// gets fullname from strings with format "FullName <Username>" or "Username"
//QString getFullname(const QString &from)
//{
//    QString fullname;

//    if (from.contains(USERNAME_START_TOKEN)) {
//         fullname = from.split(USERNAME_START_TOKEN).at(0).trimmed();
//    }

//    return fullname;
//}

} // namespace


//--------------------------------------------------------------------------------------------------
// ScriptCoverageWidget
//--------------------------------------------------------------------------------------------------

Lvk::FE::ScriptCoverageWidget::ScriptCoverageWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::ScriptCoverageWidget)
{
    ui->setupUi(this);

    clear();
    setupTables();
    connectSignals();

    ////////////////////////////// MOCK //////////////////////////////
//    ui->teachRuleButton->setVisible(false);
//    ui->showRuleButton->setVisible(false);
//    ui->filter->setVisible(false);
//    ui->filterLabel->setVisible(false);

    int nextRow;

    nextRow = ui->scriptsTable->rowCount();
    ui->scriptsTable->insertRow(nextRow);
    ui->scriptsTable->setItem(nextRow, ScriptNameColumnn,    new QTableWidgetItem("pedro_1.txt"));
    ui->scriptsTable->setItem(nextRow, ScriptCoverageColumn, new QTableWidgetItem("%70"));
    nextRow = ui->scriptsTable->rowCount();
    ui->scriptsTable->insertRow(nextRow);
    ui->scriptsTable->setItem(nextRow, ScriptNameColumnn,    new QTableWidgetItem("pedro_2.txt"));
    ui->scriptsTable->setItem(nextRow, ScriptCoverageColumn, new QTableWidgetItem("%90"));
    nextRow = ui->scriptsTable->rowCount();
    ui->scriptsTable->insertRow(nextRow);
    ui->scriptsTable->setItem(nextRow, ScriptNameColumnn,    new QTableWidgetItem("pedro_3.txt"));
    ui->scriptsTable->setItem(nextRow, ScriptCoverageColumn, new QTableWidgetItem("%20"));

    nextRow = ui->scriptDetailsTable->rowCount();
    ui->scriptDetailsTable->insertRow(nextRow);
    ui->scriptDetailsTable->setItem(nextRow, StatusColumn,     new QTableWidgetItem(""));
    ui->scriptDetailsTable->setItem(nextRow, CharacterColumnn, new QTableWidgetItem(""));
    ui->scriptDetailsTable->setItem(nextRow, MessageColumn,    new QTableWidgetItem("Por que estaban tus huellas en el arma homicida?"));
    ui->scriptDetailsTable->setItem(nextRow, RuleColumn,    new QTableWidgetItem("Por que estaban tus huellas en el arma homicida?"));
    ui->scriptDetailsTable->item(nextRow, StatusColumn)->setIcon(QIcon(MATCH_ICON));
    ui->scriptDetailsTable->item(nextRow, CharacterColumnn)->setIcon(QIcon(":/icons/detective.png"));

    nextRow = ui->scriptDetailsTable->rowCount();
    ui->scriptDetailsTable->insertRow(nextRow);
    ui->scriptDetailsTable->setItem(nextRow, StatusColumn,     new QTableWidgetItem(""));
    ui->scriptDetailsTable->setItem(nextRow, CharacterColumnn, new QTableWidgetItem(""));
    ui->scriptDetailsTable->setItem(nextRow, MessageColumn,    new QTableWidgetItem("* cena *"));
    ui->scriptDetailsTable->setItem(nextRow, RuleColumn,    new QTableWidgetItem("Porque durante la cena..."));
    ui->scriptDetailsTable->item(nextRow, StatusColumn)->setIcon(QIcon(MATCH_ICON));
    ui->scriptDetailsTable->item(nextRow, CharacterColumnn)->setIcon(QIcon(":/icons/suspect.png"));

    nextRow = ui->scriptDetailsTable->rowCount();
    ui->scriptDetailsTable->insertRow(nextRow);
    ui->scriptDetailsTable->setItem(nextRow, StatusColumn,     new QTableWidgetItem(""));
    ui->scriptDetailsTable->setItem(nextRow, CharacterColumnn, new QTableWidgetItem(""));
    ui->scriptDetailsTable->setItem(nextRow, MessageColumn,    new QTableWidgetItem("Quien mas estaba durante la cena?"));
    ui->scriptDetailsTable->setItem(nextRow, RuleColumn,    new QTableWidgetItem("Quien mas estaba durante la cena?"));
    ui->scriptDetailsTable->item(nextRow, StatusColumn)->setIcon(QIcon(MATCH_ICON));
    ui->scriptDetailsTable->item(nextRow, CharacterColumnn)->setIcon(QIcon(":/icons/detective.png"));

    nextRow = ui->scriptDetailsTable->rowCount();
    ui->scriptDetailsTable->insertRow(nextRow);
    ui->scriptDetailsTable->setItem(nextRow, StatusColumn,     new QTableWidgetItem(""));
    ui->scriptDetailsTable->setItem(nextRow, CharacterColumnn, new QTableWidgetItem(""));
    ui->scriptDetailsTable->setItem(nextRow, MessageColumn,    new QTableWidgetItem("* Florencia *"));
    ui->scriptDetailsTable->setItem(nextRow, RuleColumn,    new QTableWidgetItem("Estaban Florencia y ..."));
    ui->scriptDetailsTable->item(nextRow, StatusColumn)->setIcon(QIcon(MATCH_ICON));
    ui->scriptDetailsTable->item(nextRow, CharacterColumnn)->setIcon(QIcon(":/icons/suspect.png"));

    nextRow = ui->scriptDetailsTable->rowCount();
    ui->scriptDetailsTable->insertRow(nextRow);
    ui->scriptDetailsTable->setItem(nextRow, StatusColumn,     new QTableWidgetItem(""));
    ui->scriptDetailsTable->setItem(nextRow, CharacterColumnn, new QTableWidgetItem(""));
    ui->scriptDetailsTable->setItem(nextRow, MessageColumn,    new QTableWidgetItem("Florencia no dice lo mismo"));
    ui->scriptDetailsTable->setItem(nextRow, RuleColumn,    new QTableWidgetItem("-"));
    ui->scriptDetailsTable->item(nextRow, StatusColumn)->setIcon(QIcon(NO_MATCH_ICON));
    ui->scriptDetailsTable->item(nextRow, CharacterColumnn)->setIcon(QIcon(":/icons/detective.png"));

    ui->coverageLabel->setText(tr("Global coverage: 60%"));
    //////////////////////////////////////////////////////////////////
}

//--------------------------------------------------------------------------------------------------

Lvk::FE::ScriptCoverageWidget::~ScriptCoverageWidget()
{
    delete ui;
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ScriptCoverageWidget::setupTables()
{
    ui->splitter->setSizes(QList<int>() << (width()*2/10) << (width()*8/10));

    // Date-Contact table
    ui->scriptsTable->setRowCount(0);
    ui->scriptsTable->setColumnCount(ScriptsTableTotalColumns);
    ui->scriptsTable->setSortingEnabled(true);
    ui->scriptsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->scriptsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->scriptsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->scriptsTable->setAlternatingRowColors(true);
    ui->scriptsTable->horizontalHeader()->setStretchLastSection(true);
    ui->scriptsTable->verticalHeader()->hide();
    ui->scriptsTable->setColumnWidth(ScriptNameColumnn, 120);
    ui->scriptsTable->setHorizontalHeaderLabels(QStringList()
                                                << tr("File")
                                                << tr("Coverage"));

    // Conversation table
    ui->scriptDetailsTable->setRowCount(0);
    ui->scriptDetailsTable->setColumnCount(ScriptDetailsTableTotalColumns);
    ui->scriptDetailsTable->setSortingEnabled(true);
    ui->scriptDetailsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->scriptDetailsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->scriptDetailsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->scriptDetailsTable->setAlternatingRowColors(true);
    ui->scriptDetailsTable->horizontalHeader()->setStretchLastSection(true);
    ui->scriptDetailsTable->verticalHeader()->hide();
    ui->scriptDetailsTable->setColumnWidth(CharacterColumnn, 22);
    ui->scriptDetailsTable->setColumnWidth(StatusColumn, 22);
    ui->scriptDetailsTable->setColumnWidth(MessageColumn, 330);
    ui->scriptDetailsTable->setHorizontalHeaderLabels(QStringList()
                                                      << tr("")
                                                      << tr("")
                                                      << tr("Script")
                                                      << tr("Rule"));
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ScriptCoverageWidget::connectSignals()
{
    connect(ui->scriptsTable->selectionModel(),
            SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            SLOT(onDateContactRowChanged(QModelIndex,QModelIndex)));

    connect(ui->scriptDetailsTable->selectionModel(),
            SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            SLOT(onConversationRowChanged(QModelIndex,QModelIndex)));

    connect(ui->scriptDetailsTable,
            SIGNAL(cellDoubleClicked(int,int)),
            SLOT(onCellDoubleClicked(int,int)));

    connect(ui->filter,
            SIGNAL(textChanged(QString)),
            SLOT(onFilterTextChanged(QString)));

    // Toolbar signals

    connect(ui->teachRuleButton,     SIGNAL(clicked()),   SLOT(onTeachRuleClicked()));
    connect(ui->showRuleButton,      SIGNAL(clicked()),   SLOT(onShowRuleClicked()));
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ScriptCoverageWidget::clear()
{
    //clearConversations();
    ui->filter->clear();
}

//--------------------------------------------------------------------------------------------------

//void Lvk::FE::ScriptCoverageWidget::clearConversations()
//{
//    ui->scriptsTable->clearContents();
//    ui->scriptsTable->setRowCount(0);
//    ui->scriptDetailsTable->clearContents();
//    ui->scriptDetailsTable->setRowCount(0);

//    ui->teachRuleButton->setEnabled(false);
//    ui->showRuleButton->setEnabled(false);
//    ui->filter->setEnabled(false);

//    m_entries.clear();
//}

//--------------------------------------------------------------------------------------------------

//void Lvk::FE::ScriptCoverageWidget::addConversationEntry(const Lvk::Cmn::Conversation::Entry &entry)
//{
//    // Do not display Facebook's own messages
//    if (entry.from.startsWith(OWN_MESSAGE_TOKEN)) {
//        return;
//    }

//    QString key = hashKey(entry);

//    if (!m_entries.contains(key)) {
//        m_entries[key] = EntryList();

//        addscriptsTableRow(entry);

//        if (ui->scriptsTable->rowCount() == 1) {
//            ui->scriptsTable->selectRow(0);
//        }
//    }

//    m_entries[key].append(entry);

//    if (ui->scriptsTable->rowCount() == 1) {
//        addscriptDetailsTableRow(entry);
//    } else if (ui->scriptsTable->selectionModel()->hasSelection()) {
//        QModelIndex selectedIndex = ui->scriptsTable->selectionModel()->currentIndex();
//        const QString &selectedKey = selectedIndex.data(HashKeyRole).toString();

//        if (key == selectedKey) {
//            addscriptDetailsTableRow(entry);
//        }
//    }

//    ui->filter->setEnabled(true);
//}

////--------------------------------------------------------------------------------------------------

//void Lvk::FE::ScriptCoverageWidget::onDateContactRowChanged(const QModelIndex &current,
//                                                         const QModelIndex &/*previous*/)
//{
//    ui->scriptDetailsTable->clearContents();
//    ui->scriptDetailsTable->setRowCount(0);

//    if (current.isValid()) {
//        int row = current.row();

//        // Fill table with new selected conversation

//        QTableWidgetItem *selectedItem = ui->scriptsTable->item(row, current.column());
//        QString key = selectedItem->data(HashKeyRole).toString();
//        const EntryList &entries = m_entries[key];

//        for (int i = 0; i < entries.size(); ++i) {
//           addscriptDetailsTableRow(entries[i]);
//        }

//        // Filter table

//        if (ui->filter->text().size() > 0) {
//            filter(ui->filter->text());
//        }

//    }
//}

////--------------------------------------------------------------------------------------------------

//void Lvk::FE::ScriptCoverageWidget::onConversationRowChanged(const QModelIndex &current,
//                                                          const QModelIndex &/*previous*/)
//{
//    if (!current.isValid()) {
//        ui->teachRuleButton->setEnabled(false);
//        ui->showRuleButton->setEnabled(false);
//    } else {
//        ui->teachRuleButton->setEnabled(true);

//        if (rowHasMatchStatus(current.row())) {
//            ui->showRuleButton->setEnabled(true);
//        } else {
//            ui->showRuleButton->setEnabled(false);
//        }
//    }
//}

//--------------------------------------------------------------------------------------------------

//void Lvk::FE::ScriptCoverageWidget::addScriptDetailsTableRow(const Lvk::Cmn::Conversation::Entry &entry)
//{
//    QString time = entry.dateTime.toString(TIME_FORMAT);
//    QString matchStr = entry.match ? tr("Response found") : tr("Response not found");

//    int nextRow = ui->scriptDetailsTable->rowCount();
//    ui->scriptDetailsTable->insertRow(nextRow);
//    ui->scriptDetailsTable->setItem(nextRow, CharacterColumnn,    new QTableWidgetItem(time));
//    ui->scriptDetailsTable->setItem(nextRow, MessageColumn,  new QTableWidgetItem(entry.msg));
//    ui->scriptDetailsTable->setItem(nextRow, StatusColumn,   new QTableWidgetItem(""));

//    ui->scriptDetailsTable->item(nextRow, MessageColumn)->setData(Qt::ToolTipRole, entry.msg);
//    ui->scriptDetailsTable->item(nextRow, StatusColumn)->setData(Qt::ToolTipRole, matchStr);
//    ui->scriptDetailsTable->item(nextRow, StatusColumn)->setData(EntryMatchRole, entry.match);
//    ui->scriptDetailsTable->item(nextRow, StatusColumn)->setData(EntryRuleIdRole, entry.ruleId);

//    if (entry.match) {
//        ui->scriptDetailsTable->item(nextRow, StatusColumn)->setIcon(QIcon(MATCH_ICON));
//    } else {
//        ui->scriptDetailsTable->item(nextRow, StatusColumn)->setIcon(QIcon(NO_MATCH_ICON));
//    }
//}

////--------------------------------------------------------------------------------------------------

//void Lvk::FE::ScriptCoverageWidget::addScriptsTableRow(const Lvk::Cmn::Conversation::Entry &entry)
//{
//    QString date = entry.dateTime.toString(DATE_FORMAT);
//    QString username = getUsername(entry.from);
//    QString fullname = getFullname(entry.from);

//    if (fullname.isEmpty()) {
//        fullname = username;
//    }

//    int nextRow = ui->scriptsTable->rowCount();
//    ui->scriptsTable->insertRow(nextRow);
//    ui->scriptsTable->setItem(nextRow, ScriptNameColumnn,    new QTableWidgetItem(date));
//    ui->scriptsTable->setItem(nextRow, ScriptCoverageColumn, new QTableWidgetItem(fullname));

//    ui->scriptsTable->item(nextRow, ScriptNameColumnn)->setData(HashKeyRole, hashKey(entry));
//    ui->scriptsTable->item(nextRow, ScriptCoverageColumn)->setData(HashKeyRole, hashKey(entry));
//    ui->scriptsTable->item(nextRow, ScriptCoverageColumn)->setData(EntryFromRole, entry.from);

//    if (username.contains("@gmail.com")) {
//        ui->scriptsTable->item(nextRow, ScriptCoverageColumn)->setIcon(QIcon(GMAIL_ICON));
//    } else if (username.contains("@chat.facebook.com")) {
//        ui->scriptsTable->item(nextRow, ScriptCoverageColumn)->setIcon(QIcon(FB_ICON));
//    } else if (username.contains(tr("(test)"))) {
//        ui->scriptsTable->item(nextRow, ScriptCoverageColumn)->setIcon(QIcon(LOCAL_TEST_ICON));
//    }
//}

////--------------------------------------------------------------------------------------------------

//void Lvk::FE::ScriptCoverageWidget::setConversation(const Lvk::Cmn::Conversation &conv)
//{
//    clear();

//    for (int i = 0; i < conv.entries().size(); ++i) {
//        addConversationEntry(conv.entries()[i]);
//    }
//}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ScriptCoverageWidget::onCellDoubleClicked(int row, int /*col*/)
{
//    if (!rowHasMatchStatus(row)) {
//        teachRuleWithDialog(row);
//    } else {
//        showRuleWithDialog(row);
//    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ScriptCoverageWidget::onTeachRuleClicked()
{
//    QModelIndex selectedIndex = ui->scriptDetailsTable->selectionModel()->currentIndex();

//    if (selectedIndex.isValid()) {
//        teachRuleWithDialog(selectedIndex.row());
//    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ScriptCoverageWidget::onShowRuleClicked()
{
//    QModelIndex selectedIndex = ui->scriptDetailsTable->selectionModel()->currentIndex();

//    if (selectedIndex.isValid()) {
//        showRuleWithDialog(selectedIndex.row());
//    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ScriptCoverageWidget::onFilterTextChanged(const QString &text)
{
    filter(text);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ScriptCoverageWidget::teachRuleWithDialog(int row)
{
//    QString chatMsg = ui->scriptDetailsTable->item(row, MessageColumn)->text();

//    QString title = tr("Teach rule");
//    QString text = QString(tr("Teach new rule for message: \"%1\" ?")).arg(chatMsg);

//    if (askConfirmation(title, text)) {
//        emit teachRule(chatMsg);
//    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ScriptCoverageWidget::showRuleWithDialog(int row)
{
//    QString chatMsg = ui->scriptDetailsTable->item(row, MessageColumn)->text();

//    QString title = tr("Show rule definition");
//    QString text = QString(tr("Show rule definition for message: \"%1\" ?")).arg(chatMsg);

//    if (askConfirmation(title, text)) {
//        quint64 ruleId = ui->scriptDetailsTable->item(row, StatusColumn)->data(EntryRuleIdRole)
//                .toULongLong();

//        emit showRule(ruleId);
//    }
}


//--------------------------------------------------------------------------------------------------

void Lvk::FE::ScriptCoverageWidget::filter(const QString &text)
{
//    for (int i = 0; i < ui->scriptDetailsTable->rowCount(); ++i) {
//        bool match = false;
//        for (int j = 0; j < ui->scriptDetailsTable->columnCount() && !match; ++j) {
//            if (ui->scriptDetailsTable->item(i, j)->text().contains(text, false)) {
//                match = true;
//            }
//        }
//        ui->scriptDetailsTable->setRowHidden(i, !match);
//    }
}

//--------------------------------------------------------------------------------------------------

bool Lvk::FE::ScriptCoverageWidget::rowHasMatchStatus(int row)
{
    return ui->scriptDetailsTable->item(row, StatusColumn)->data(EntryMatchRole).toBool();
}

//--------------------------------------------------------------------------------------------------

bool Lvk::FE::ScriptCoverageWidget::askConfirmation(const QString &title, const QString &text)
{
    int btn = QMessageBox::question(this, title, text, QMessageBox::Yes, QMessageBox::No);

    return btn == QMessageBox::Yes;
}


