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

enum ScriptsTableColumns
{
    ScriptNameColumnn,
    ScriptCoverageColumn,
    ScriptsTableTotalColumns
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

    ui->splitter->setSizes(QList<int>() << (width()*3/10) << (width()*3/10) << (width()*4/10));

    ////////////////////////////// MOCK //////////////////////////////
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
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ScriptCoverageWidget::connectSignals()
{
    connect(ui->scriptsTable->selectionModel(),
            SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            SLOT(onScriptRowChanged(QModelIndex,QModelIndex)));
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ScriptCoverageWidget::clear()
{
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ScriptCoverageWidget::onScriptRowChanged(const QModelIndex &current,
                                                       const QModelIndex &/*previous*/)
{
    QTableWidgetItem *item = ui->scriptsTable->item(current.row(), ScriptNameColumnn);
    if (item) {
        ui->label->setText(item->text());
    } else {
        ui->label->setText("no script selected");
    }
}

