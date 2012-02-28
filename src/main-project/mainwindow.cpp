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
#include "ui_mainwindow.h"

#include <QStandardItemModel>
#include <QItemDelegate>
#include <QInputDialog>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), m_categoriesTreeModel(0)
{
    ui->setupUi(this);

    connect(ui->addCategoryButton, SIGNAL(clicked()), SLOT(addCategoryWithInputDialog()));
    connect(ui->addRuleButton,     SIGNAL(clicked()), SLOT(addRuleWithInputDialog()));
    connect(ui->rmItemButton,      SIGNAL(clicked()), SLOT(removeSelectedItem()));

    clear();
    initModels();
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
    ui->ruleOutputText->clear();

    // chat tab widgets
    //ui->fbChatRadio->
    ui->usernameText->clear();
    ui->passwordText->clear();

    // conversation tab widgets
    // TODO clear ui->conversationContactsTable
    // TODO clear ui->conversationTable

    // help tab widgets
    ui->helpLabel->clear();

    // test tab widgets
    ui->testConversationText->clear();
    ui->testInputText->clear();
}

void MainWindow::initModels()
{
    m_categoriesTreeModel = new QStandardItemModel();

    ui->categoriesTree->setModel(m_categoriesTreeModel);

    m_categoriesSelectionModel = ui->categoriesTree->selectionModel();

    ////////////////////////////////////////////////////////////////////////
    // TODO read from file:

    QStandardItem *catGreetings    = addCategory("Saludos");
    QStandardItem *catPersonalInfo = addCategory("Informacion personal");
    QStandardItem *catGoodbye      = addCategory("Despedidas");

    addRule("Hola", catGreetings);
    addRule("Buenas", catGreetings);

    addRule("Quien eres?", catPersonalInfo);
    addRule("Como te llamas?", catPersonalInfo);
    addRule("Donde naciste?", catPersonalInfo);

    addRule("Chau!", catGoodbye);
    addRule("Nos vemos!", catGoodbye);
    addRule("Me voy", catGoodbye);

    ////////////////////////////////////////////////////////////////////////
}

QStandardItem *MainWindow::addCategory(const QString &name)
{
    const QIcon CATEGORY_ICON(":/icons/category_16x16.png");

    QStandardItem *category = new QStandardItem(CATEGORY_ICON, name);

    m_categoriesTreeModel->invisibleRootItem()->appendRow(category);

    return category;
}


QStandardItem *MainWindow::addRule(const QString &name, QStandardItem *category)
{
    const QIcon RULE_ICON(":/icons/rule_16x16.png");

    QStandardItem *rule = new QStandardItem(RULE_ICON, name);

    category->appendRow(rule);

    return rule;
}


void MainWindow::addCategoryWithInputDialog()
{
    bool ok;
    QString name = QInputDialog::getText(this, tr("Add category"), tr("Category name:"),
                                         QLineEdit::Normal, "", &ok);

    if (ok) {
        if (!name.isEmpty()) {
            QStandardItem *category = addCategory(name);
            m_categoriesSelectionModel->setCurrentIndex(category->index(),
                                                        QItemSelectionModel::ClearAndSelect);
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

    if (selectedRows.size() > 0) {
        QModelIndex selectedIndex = selectedRows[0];

        QString dialogText;
        QString dialogTitle;

        // If category or rule
        if (selectedIndex.parent() == m_categoriesTreeModel->invisibleRootItem()->index()) {
            dialogTitle = tr("Remove category");
            dialogText = QString(tr("Are you sure you want to remove the category '%0'?\n"
                                    "All rules belonging to that category will be also removed"))
                    .arg(selectedIndex.data(Qt::DisplayRole).toString());
        } else {
            dialogTitle = tr("Remove rule");
            dialogText = QString(tr("Are you sure you want to remove the rule '%0'?"))
                    .arg(selectedIndex.data(Qt::DisplayRole).toString());
        }

        QMessageBox msg(QMessageBox::Critical, dialogTitle, dialogText,
                        QMessageBox::Yes | QMessageBox::No, this);

        if (msg.exec() == QMessageBox::Yes) {
            m_categoriesTreeModel->removeRow(selectedIndex.row(), selectedIndex.parent());
        }

    } else {
        QMessageBox msg(QMessageBox::Critical, tr("Add rule"),
                        tr("Select the category you want to remove"), QMessageBox::Ok, this);
        msg.exec();
    }}

void MainWindow::removeSelectedItem()
{
    QModelIndexList selectedRows = m_categoriesSelectionModel->selectedRows();

    if (selectedRows.size() > 0) {
        QModelIndex selectedIndex = selectedRows[0];

        QString dialogText;
        QString dialogTitle;

        // If category or rule
        if (selectedIndex.parent() == m_categoriesTreeModel->invisibleRootItem()->index()) {
            dialogTitle = tr("Remove category");
            dialogText = QString(tr("Are you sure you want to remove the category '%0'?\n"
                                    "All rules belonging to that category will be also removed"))
                    .arg(selectedIndex.data(Qt::DisplayRole).toString());
        } else {
            dialogTitle = tr("Remove rule");
            dialogText = QString(tr("Are you sure you want to remove the rule '%0'?"))
                    .arg(selectedIndex.data(Qt::DisplayRole).toString());
        }

        QMessageBox msg(QMessageBox::Critical, dialogTitle, dialogText,
                        QMessageBox::Yes | QMessageBox::No, this);

        if (msg.exec() == QMessageBox::Yes) {
            m_categoriesTreeModel->removeRow(selectedIndex.row(), selectedIndex.parent());
        }

    } else {
        QMessageBox msg(QMessageBox::Critical, tr("Remove rule or category"),
                        tr("Select the rule or category you want to remove"),
                        QMessageBox::Ok, this);
        msg.exec();
    }
}
