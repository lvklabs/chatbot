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

#include "mainwindow.h"
#include "ruletreemodel.h"
#include "ui_mainwindow.h"

#include <QStandardItemModel>
#include <QItemDelegate>
#include <QInputDialog>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), m_ruleTreeModel(0)
{
    ui->setupUi(this);

    clear();

    connect(ui->addCategoryButton, SIGNAL(clicked()), SLOT(addCategoryWithInputDialog()));
    connect(ui->addRuleButton,     SIGNAL(clicked()), SLOT(addRuleWithInputDialog()));
    connect(ui->rmItemButton,      SIGNAL(clicked()), SLOT(removeSelectedItem()));

    initModels();

    connect(ui->categoriesTree->selectionModel(),
            SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            SLOT(handleRuleSelectionChanged(QItemSelection,QItemSelection)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::clear()
{
    // reset active tabs
    ui->mainTabWidget->setCurrentIndex(0);
    ui->rightSideTabWidget->setCurrentIndex(0);

    // train tab widgets
    // TODO clear ui->categoriesTree
    ui->ruleInputText->clear();
    ui->ruleInputVariantsText->clear();
    ui->ruleOutputText->clear();

    // chat tab widgets
    //ui->fbChatRadio->
    ui->usernameText->clear();
    ui->passwordText->clear();

    // conversation tab widgets
    // TODO clear ui->conversationContactsTable
    // TODO clear ui->conversationTable

    // test tab widgets
    ui->testConversationText->clear();
    ui->testInputText->clear();
}

void MainWindow::initModels()
{
    m_ruleTreeModel = new Lvk::RuleTreeModel("FIXME", this);

    m_ruleTreeModel->setHeaderData(0, Qt::Horizontal, QString(tr("Rules")), Qt::DisplayRole);

    ui->categoriesTree->setModel(m_ruleTreeModel);

    m_categoriesSelectionModel = ui->categoriesTree->selectionModel();
}

Lvk::RuleItem *MainWindow::addCategory(const QString &name)
{
    Lvk::RuleItem *category = new Lvk::RuleItem(name, m_ruleTreeModel->invisibleRootItem());

    category->setType(Lvk::RuleItem::CategoryRule);

    bool appended = m_ruleTreeModel->appendItem(category);

    if (appended) {
        return category;
    } else {
        delete category;

        return 0;
    }
}

Lvk::RuleItem *MainWindow::addRule(const QString &name, Lvk::RuleItem *category)
{
    Lvk::RuleItem *rule = new Lvk::RuleItem(name, category);

    bool appended = m_ruleTreeModel->appendItem(rule);

    if (appended) {
        return rule;
    } else {
        delete rule;

        return 0;
    }
}

void MainWindow::addCategoryWithInputDialog()
{
    bool ok;
    QString name = QInputDialog::getText(this, tr("Add category"), tr("Category name:"),
                                         QLineEdit::Normal, "", &ok);

    if (ok) {
        if (!name.isEmpty()) {
            Lvk::RuleItem *category = addCategory(name);

            if (category) {
                QModelIndex categoryIndex = m_ruleTreeModel->indexFromItem(category);
                m_categoriesSelectionModel->setCurrentIndex(categoryIndex,
                                                            QItemSelectionModel::ClearAndSelect);
            } else {
                QMessageBox msg(QMessageBox::Critical, tr("Internal error"),
                                tr("The category could not be added because of an internal error"),
                                QMessageBox::Ok, this);
                msg.exec();
            }
        } else {
            QMessageBox msg(QMessageBox::Critical, tr("Add category"),
                            tr("The category name cannot be empty"), QMessageBox::Ok, this);
            msg.exec();
        }
    }
}

void MainWindow::addRuleWithInputDialog()
{
    QModelIndexList selectedRows = m_categoriesSelectionModel->selectedRows();

    if (selectedRows.size() <= 0) {
        QMessageBox msg(QMessageBox::Information, tr("Add rule"),
                        tr("Select the category where the rule will belong to"),
                        QMessageBox::Ok, this);
        msg.exec();

        return;
    }

    QModelIndex selectedIndex = selectedRows[0];
    Lvk::RuleItem *selectedItem = m_ruleTreeModel->itemFromIndex(selectedIndex);
    Lvk::RuleItem *parentCategory = 0;

    if (selectedItem->type() == Lvk::RuleItem::CategoryRule) {
        parentCategory = selectedItem;
        ui->categoriesTree->setExpanded(selectedIndex, true);
    } else {
        parentCategory =  m_ruleTreeModel->itemFromIndex(selectedIndex.parent());
        ui->categoriesTree->setExpanded(selectedIndex.parent(), true);
    }

    Lvk::RuleItem *emptyRule = addRule("", parentCategory);

    if (emptyRule) {
        m_categoriesSelectionModel->select(m_ruleTreeModel->indexFromItem(emptyRule),
                                           QItemSelectionModel::ClearAndSelect);
        ui->ruleInputText->setFocus();
    } else {
        QMessageBox msg(QMessageBox::Critical, tr("Internal error"),
                        tr("The rule could not be added because of an internal error"),
                        QMessageBox::Ok, this);
        msg.exec();
    }
}

void MainWindow::removeSelectedItem()
{
    QModelIndexList selectedRows = m_categoriesSelectionModel->selectedRows();

    if (selectedRows.size() <= 0) {
        QMessageBox msg(QMessageBox::Critical, tr("Remove rule or category"),
                        tr("Select the rule or category you want to remove"),
                        QMessageBox::Ok, this);
        msg.exec();

        return;
    }

    QModelIndex selectedIndex = selectedRows[0];
    Lvk::RuleItem *selectedItem = m_ruleTreeModel->itemFromIndex(selectedIndex);

    QString dialogText;
    QString dialogTitle;

    if (selectedItem->type() == Lvk::RuleItem::CategoryRule) {
        dialogTitle = tr("Remove category");
        dialogText = QString(tr("Are you sure you want to remove the category '%0'?\n"
                                "All rules belonging to that category will be also removed"))
                .arg(selectedIndex.data(Qt::DisplayRole).toString());
    } else {
        dialogTitle = tr("Remove rule");
        dialogText = QString(tr("Are you sure you want to remove the rule '%0'?"))
                .arg(selectedIndex.data(Qt::DisplayRole).toString());
    }

    QMessageBox msg(QMessageBox::Question, dialogTitle, dialogText,
                    QMessageBox::Yes | QMessageBox::No, this);

    if (msg.exec() == QMessageBox::Yes) {
        bool removed = m_ruleTreeModel->removeRow(selectedIndex.row(), selectedIndex.parent());

        if (!removed) {
            QMessageBox msg(QMessageBox::Critical, tr("Internal error"),
                            tr("The rule/category could not be removed because of an internal error"),
                            QMessageBox::Ok, this);
            msg.exec();
        }
    }
}

void MainWindow::handleRuleSelectionChanged(const QItemSelection &selected,
                                            const QItemSelection &deselected)
{
    if (deselected.indexes().size() > 0) {
        Lvk::RuleItem *item = static_cast<Lvk::RuleItem *>(deselected.indexes()[0].internalPointer());

        if (item->type() == Lvk::RuleItem::Rule) {
            QStringList input = ui->ruleInputVariantsText->toPlainText().split("\n", QString::SkipEmptyParts);
            QStringList output = ui->ruleOutputText->toPlainText().split("\n", QString::SkipEmptyParts);

            input.prepend(ui->ruleInputText->text());

            item->setInput(input);
            item->setOutput(output);
        } else {
            // nothing to do
        }
    }

    if (selected.indexes().size() > 0) {
        Lvk::RuleItem *item = static_cast<Lvk::RuleItem *>(selected.indexes()[0].internalPointer());

        if (item->type() == Lvk::RuleItem::Rule) {
            ui->ruleInputText->setVisible(true);
            ui->ruleInputVariantsText->setVisible(true);
            ui->ruleOutputText->setVisible(true);

            ui->ifUserWritesLabel->setVisible(true);
            ui->orVariantsLabel->setVisible(true);
            ui->chatbotRepliesLabel->setVisible(true);

            QString input, inputVariants, output;

            for (int i = 0; i < item->input().size(); ++i) {
                QString trimmed = item->input()[i].trimmed();
                if (!trimmed.isEmpty()) {
                    if (i == 0) {
                        input = trimmed;
                    } else {
                        inputVariants += trimmed + "\n";
                    }
                }
            }

            for (int i = 0; i < item->output().size(); ++i) {
                QString trimmed = item->output()[i].trimmed();
                if (!trimmed.isEmpty()) {
                    output += trimmed + "\n";
                }
            }

            ui->ruleInputText->setText(input);
            ui->ruleInputVariantsText->setPlainText(inputVariants);
            ui->ruleOutputText->setPlainText(output);

        } else {
            ui->ruleInputText->setText("");
            ui->ruleInputVariantsText->setPlainText("");
            ui->ruleOutputText->setPlainText("");

            ui->ruleInputText->setVisible(false);
            ui->ruleInputVariantsText->setVisible(false);
            ui->ruleOutputText->setVisible(false);

            ui->ifUserWritesLabel->setVisible(false);
            ui->orVariantsLabel->setVisible(false);
            ui->chatbotRepliesLabel->setVisible(false);
        }
    }
}
