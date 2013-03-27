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

#include "front-end/cluewidget.h"
#include "front-end/detailsdialog.h"
#include "da-clue/scriptmanager.h"
#include "da-clue/clueengine.h"
#include "da-clue/scripterror.h"
#include "common/globalstrings.h"
#include "common/settings.h"
#include "common/settingskeys.h"
#include "ui_cluewidget.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QSplitter>

//--------------------------------------------------------------------------------------------------
// ClueWidget
//--------------------------------------------------------------------------------------------------

Lvk::FE::ClueWidget::ClueWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::ClueWidget), m_appFacade(0)
{
    ui->setupUi(this);

    connect(ui->refreshButton, SIGNAL(clicked()),         SLOT(refresh()));
    connect(ui->importButton,  SIGNAL(clicked()),         SLOT(import()));
    connect(ui->uploadButton,  SIGNAL(clicked()),         SIGNAL(upload()));
    connect(ui->scripts,       SIGNAL(showRule(quint64)), SIGNAL(showRule(quint64)));

    loadSettings();

    refresh();
}

//--------------------------------------------------------------------------------------------------

Lvk::FE::ClueWidget::~ClueWidget()
{
    saveSettings();

    delete ui;
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ClueWidget::setAppFacade(BE::AppFacade *appFacade)
{
    m_appFacade = appFacade;

    refresh();
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ClueWidget::refresh()
{
    if (m_appFacade && !m_appFacade->currentCharacter().isEmpty()) {
        ui->charaterBox->setTitle(tr("Character: ") + m_appFacade->currentCharacter());

        m_appFacade->reloadScripts();

        int i = ui->scripts->currentScript();
        ui->scripts->setAnalyzedScripts(m_appFacade->analyzedScripts(), m_appFacade->rootRule());
        ui->scripts->setCurrentScript(i);

        bool catVisible = m_appFacade->nlpEngineOptions() & BE::AppFacade::PreferCurCategory;
        ui->scripts->setCategoryVisible(catVisible);
    } else {
        clear();
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ClueWidget::import()
{
    QString caption = tr("Select the scripts files to import");
    QString dir = ".";
    QString filter = "*." SCRIPT_FILE_EXT ";;*.*";

    QStringList files = QFileDialog::getOpenFileNames(this, caption, dir, filter);

    int count = 0;

    foreach (const QString &file, files) {
        if (m_appFacade->importScript(file)) {
            ++count;
        } else {
            showError(file);
        }
    }

    if (count > 0) {
        refresh();
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ClueWidget::clear()
{
    ui->charaterBox->setTitle(tr("Character: (none)"));
    ui->scripts->clear();
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ClueWidget::showError(const QString &filename)
{
    QString details;
    int err = m_appFacade->error(&details);

    QString title = tr("Error %1").arg(err);
    QString msg = tr("Cannot import file '%1'.").arg(QFileInfo(filename).fileName());

    if (err == Clue::CharacterMismatchError) {
        msg.append(tr(" The character does not match."));
    }

    FE::DetailsDialog dialog(msg, tr("Details"), details, this);
    dialog.setWindowTitle(title);
    dialog.setCancelButtonVisible(false);
    dialog.setPixmap(QStyle::SP_MessageBoxCritical);
    dialog.exec();
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ClueWidget::loadSettings()
{
    QString colsw = Cmn::Settings().value(SETTING_CLUE_WIDGET_COLS_W).toString();

    if (colsw.isEmpty()) {
        return;
    }

    QStringList ws = colsw.split(",", QString::SkipEmptyParts);

    QList<int> sizes;
    foreach (const QString &w, ws) {
        sizes.append(w.toInt());
    }

    if (!sizes.isEmpty()) {
        ui->scripts->setSplitterSizes(sizes);
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ClueWidget::saveSettings()
{
    QString colsw;

    foreach (int size, ui->scripts->splitterSizes()) {
        colsw += QString::number(size) + ",";
    }

    Cmn::Settings().setValue(SETTING_CLUE_WIDGET_COLS_W, colsw);
}
