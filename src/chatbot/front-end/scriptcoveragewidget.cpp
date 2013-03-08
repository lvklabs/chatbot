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

const QString HTML_SCRIPT_START     = "<html><header><style></style></head><body>";
const QString HTML_SCRIPT_LINE      = "<a href=\"#%6\" style=\"%1\">%2: %3<br/>%4: %5</a><br/>";
const QString HTML_SCRIPT_LINE_OK   = HTML_SCRIPT_LINE.arg("text-decoration:none;color:#000");
const QString HTML_SCRIPT_LINE_FAIL = HTML_SCRIPT_LINE.arg("text-decoration:none;color:#F00");
const QString HTML_SCRIPT_END       = "</body></html>";


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
    : QWidget(parent), ui(new Ui::ScriptCoverageWidget), m_detective(tr("Detective"))
{
    ui->setupUi(this);

    clear();
    setupTables();
    connectSignals();

    ui->splitter->setSizes(QList<int>() << (width()*2/10) << (width()*4/10) << (width()*4/10));

    //ui->scriptView->setStyleSheet();
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
                                                << tr("%"));
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ScriptCoverageWidget::connectSignals()
{
    connect(ui->scriptsTable->selectionModel(),
            SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            SLOT(onScriptRowChanged(QModelIndex,QModelIndex)));

    connect(ui->scriptView,
            SIGNAL(anchorClicked(QUrl)),
            SLOT(onAnchorClicked(QUrl)));
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
        showScript(m_scripts[current.row()]);
    } else {
        ui->scriptView->setText(tr("(No script selected)"));
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ScriptCoverageWidget::onAnchorClicked(const QUrl &url)
{
    // TODO
    ui->groupBox->setTitle("Rule " + url.toString());
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ScriptCoverageWidget::showScript(const Lvk::Clue::AnalyzedScript &script)
{
    if (script.isEmpty()) {
        ui->scriptView->setText(tr("(Empty script)"));
        return;
    }

    QString html = HTML_SCRIPT_START;

    foreach (const Clue::AnalyzedLine &line, script) {
        html += (line.outputIdx != -1 ? HTML_SCRIPT_LINE_OK : HTML_SCRIPT_LINE_FAIL)
                .arg(m_detective, line.question, script.character, line.answer).arg(line.ruleId);
    }

    html += HTML_SCRIPT_END;

    ui->scriptView->setText(html);
}

