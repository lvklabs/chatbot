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

#include "exportdialog.h"
#include "ui_portdialog.h"

//--------------------------------------------------------------------------------------------------
// ExportDialog
//--------------------------------------------------------------------------------------------------

Lvk::FE::ExportDialog::ExportDialog(QWidget *parent /*= 0*/)
    : PortDialog(parent), m_model(0)
{
    setWindowTitle("");
    ui->label->setText("");
}

//--------------------------------------------------------------------------------------------------

Lvk::FE::ExportDialog::ExportDialog(const QString &title, const QString &msg,
                                    RuleTreeModel *model, QWidget *parent /*= 0*/)
    : PortDialog(parent), m_model(model)
{
    setWindowTitle(title);
    ui->label->setText(msg);
    ui->treeView->setModel(model);
}

//--------------------------------------------------------------------------------------------------

int Lvk::FE::ExportDialog::exec(BE::Rule *container)
{
    int code = PortDialog::exec();

    if (code == QDialog::Accepted) {
        container->clear();
        container->setType(BE::Rule::ContainerRule);

        copyCheckedRules(container, m_model->invisibleRootItem());
    }

    return code;
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ExportDialog::copyCheckedRules(BE::Rule *to, BE::Rule *from)
{
    if (!to || !from) {
        return;
    }

    foreach (BE::Rule *child, from->children()) {
        if (child->checkState() != Qt::Unchecked) {
            BE::Rule *childCopy = new BE::Rule(*child);
            to->appendChild(childCopy);
            copyCheckedRules(childCopy, child);
        }
    }
}

























