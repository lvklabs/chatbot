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

#ifndef LVK_FE_PORTDIALOG_H
#define LVK_FE_PORTDIALOG_H

#include "rule.h"
#include "ruletreemodel.h"

#include <QDialog>
#include <QString>


namespace Ui {
    class PortDialog;
}

class QItemSelection;

namespace Lvk
{

namespace FE
{

/**
 * The PortDialog class is the base class of import and export dialogs
 *
 * @see ImportDialog, ExportDialog
 */

class PortDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PortDialog(QWidget *parent = 0);

    PortDialog(const QString &title, const QString &msg, RuleTreeModel *model, QWidget *parent = 0);

    ~PortDialog();

    int exec(BE::Rule *container);

private:
    Ui::PortDialog *ui;
    RuleTreeModel *m_model;
    BE::Rule *m_secondRoot;
    BE::Rule::Status m_rootStatusBak;

    void addExtraRootLevel();
    void removeExtraRootLevel();
    void checkAllRules();
    void copyCheckedRules(BE::Rule *to, BE::Rule *from);

private slots:
    void onRuleSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void onAcceptButtonPressed();
    void onRejectButtonPressed();

};


} // namespace FE

} // namespace Lvk

#endif // LVK_FE_PORTDIALOG_H
