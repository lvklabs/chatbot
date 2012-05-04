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

#include "portdialog.h"
#include "ui_portdialog.h"

//--------------------------------------------------------------------------------------------------
// Helpers
//--------------------------------------------------------------------------------------------------

namespace
{

QString rulesOfCategoryToString(const Lvk::BE::Rule *rule)
{
    QString str;
    foreach (Lvk::BE::Rule *r, rule->children()) {
        if (!r->input().isEmpty()) {
            if (!str.isEmpty()) {
                str.append("<br/>");
            }
            str.append(r->input().first());
        }
    }
    return str;
}

QString ruleToString(const Lvk::BE::Rule *rule)
{
    if (rule && rule->type() == Lvk::BE::Rule::OrdinaryRule) {
        return QString(QObject::tr("<b>If user writes:</b><br/>%1<br/><br/>"
                                   "<b>Chatbot replies:</b><br/>%2"))
                .arg(rule->input().join("<br/>"))
                .arg(rule->output().join("<br/>"));
    } else if (rule && rule->type() == Lvk::BE::Rule::ContainerRule) {
        return QString(QObject::tr("<b>Category:</b><br/>%1<br/><br/>"
                                   "<b>Rules:</b><br/>%2"))
                .arg(rule->name())
                .arg(rulesOfCategoryToString(rule));
    } else if (rule && rule->type() == Lvk::BE::Rule::EvasiveRule) {
        return QString(QObject::tr("<b>If Chatbot does not understand:</b><br/>%1"))
                .arg(rule->output().join("<br/>"));
    } else {
        return "";
    }
}

} // namespace

//--------------------------------------------------------------------------------------------------
// PortDialog
//--------------------------------------------------------------------------------------------------

Lvk::FE::PortDialog::PortDialog(QWidget *parent /*= 0*/)
    : QDialog(parent), ui(new Ui::PortDialog), m_model(0)
{
    ui->setupUi(this);
    ui->label->setText("");
    setWindowTitle("");
}

//--------------------------------------------------------------------------------------------------

Lvk::FE::PortDialog::PortDialog(const QString &title, const QString &msg,
                                RuleTreeModel *model, QWidget *parent /*= 0*/)
    : QDialog(parent), ui(new Ui::PortDialog), m_model(model)
{
    model->setIsUserCheckable(true);
    ui->setupUi(this);
    ui->label->setText(msg);
    ui->treeView->setModel(model);
    ui->treeView->setHeaderHidden(true);
    setWindowTitle(title);

    connect(ui->treeView->selectionModel(),
            SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            SLOT(onRuleSelectionChanged(QItemSelection,QItemSelection)));
}

//--------------------------------------------------------------------------------------------------

Lvk::FE::PortDialog::~PortDialog()
{
    delete ui;
}

//--------------------------------------------------------------------------------------------------

int Lvk::FE::PortDialog::exec(BE::Rule *container)
{
    int code = QDialog::exec();

    if (code == QDialog::Accepted) {
        container->clear();
        container->setType(BE::Rule::ContainerRule);

        copyCheckedRules(container, m_model->invisibleRootItem());
    }

    return code;
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::PortDialog::copyCheckedRules(BE::Rule *to, BE::Rule *from)
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

//--------------------------------------------------------------------------------------------------

void Lvk::FE::PortDialog::onRuleSelectionChanged(const QItemSelection &selected,
                                                 const QItemSelection &/*deselected*/)
{
    if (!selected.indexes().isEmpty()) {
        const BE::Rule *rule =
                static_cast<const BE::Rule *>(selected.indexes().first().internalPointer());

        ui->rulePreview->setText(ruleToString(rule));
    }
}






