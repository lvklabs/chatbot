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
    ScriptNameCol,
    ScriptCoverageCol,
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

// coverage string format
inline QString covFormat(float cov)
{
    return "%" + QString::number((int)cov);
}

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
    ui->scriptsTable->setColumnWidth(ScriptNameCol, 120);
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

void Lvk::FE::ScriptCoverageWidget::setAnalyzedScripts(const Clue::AnalyzedList &scripts)
{
    clear();

    m_scripts = scripts;

    float globalCov = 0.0;

    foreach (const Clue::AnalyzedScript &s, scripts) {
        addScriptRow(s.filename, s.coverage);
        globalCov += s.coverage;
    }

    if (scripts.size() > 0) {
        globalCov /= scripts.size();
        ui->coverageLabel->setText(tr("Global coverage: ") + covFormat(globalCov));
    } else {
        ui->coverageLabel->clear();
    }

}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ScriptCoverageWidget::addScriptRow(const QString &filename, float coverage)
{
    int r = ui->scriptsTable->rowCount();
    ui->scriptsTable->insertRow(r);
    ui->scriptsTable->setItem(r, ScriptNameCol,     new QTableWidgetItem(filename));
    ui->scriptsTable->setItem(r, ScriptCoverageCol, new QTableWidgetItem(covFormat(coverage)));
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ScriptCoverageWidget::clear()
{
    m_scripts.clear();
    ui->scriptsTable->clearContents();
    ui->scriptsTable->setRowCount(0);
    ui->coverageLabel->clear();
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ScriptCoverageWidget::onScriptRowChanged(const QModelIndex &current,
                                                       const QModelIndex &/*previous*/)
{
    QTableWidgetItem *item = ui->scriptsTable->item(current.row(), ScriptNameCol);
    if (item) {
        ui->label->setText(item->text());
    } else {
        ui->label->setText("no script selected");
    }
}

