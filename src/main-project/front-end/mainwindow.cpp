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
#include "coreapp.h"
#include "ruletreemodel.h"
#include "rule.h"
#include "ui_mainwindow.h"

#include <QStandardItemModel>
#include <QItemDelegate>
#include <QInputDialog>
#include <QMessageBox>

Lvk::FE::MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow), m_coreApp(new BE::CoreApp()), m_ruleTreeModel(0)
{
    ui->setupUi(this);
    ui->teachTabsplitter->setSizes(QList<int>() << 10 << 10);

    clear();

    initCoreAndModels();

    connectSignals();

    ui->testInputText->installEventFilter(this);
    ui->ruleInputWidget->installEventFilter(this);
    ui->ruleOutputWidget->installEventFilter(this);

    selectFirstRule();

    ui->categoriesTree->setFocus();
}

//--------------------------------------------------------------------------------------------------

Lvk::FE::MainWindow::~MainWindow()
{
    delete ui;
    delete m_coreApp;
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::initCoreAndModels()
{
    m_coreApp->load("FIXME");

    m_ruleTreeModel = new FE::RuleTreeModel(m_coreApp->rootRule(), this);
    m_ruleTreeModel->setHeaderData(0, Qt::Horizontal, QString(tr("Rules")), Qt::DisplayRole);

    ui->categoriesTree->setModel(m_ruleTreeModel);

    m_ruleTreeSelectionModel = ui->categoriesTree->selectionModel();
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::connectSignals()
{
    connect(ui->addCategoryButton, SIGNAL(clicked()), SLOT(addCategoryWithInputDialog()));
    connect(ui->addRuleButton,     SIGNAL(clicked()), SLOT(addRuleWithInputDialog()));
    connect(ui->rmItemButton,      SIGNAL(clicked()), SLOT(removeSelectedItem()));

    connect(ui->testInputText, SIGNAL(returnPressed()), SLOT(testInputTextEntered()));

    connect(ui->clearTestConversationButton, SIGNAL(clicked()),
            ui->testConversationText, SLOT(clear()));

    connect(ui->categoriesTree->selectionModel(),
            SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            SLOT(handleRuleSelectionChanged(QItemSelection,QItemSelection)));

    connect(ui->categoryNameTextEdit, SIGNAL(textEdited(QString)),
            SLOT(handleRuleInputEdited(QString)));

    connect(ui->ruleInputWidget, SIGNAL(inputTextEdited(QString)),
            SLOT(handleRuleInputEdited(QString)));
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::selectFirstRule()
{
    if (rootRule()) {
        if (rootRule()->childCount() > 0) {
            BE::Rule *child = rootRule()->child(0);
            if (child->childCount() > 0) {
                selectRule(child->child(0));
            } else {
                selectRule(child);
            }
        }
    }
}

//--------------------------------------------------------------------------------------------------

bool Lvk::FE::MainWindow::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::FocusOut) {
        if (object == ui->testInputText) {
            ui->ruleInputWidget->clearHighlight();
            ui->ruleOutputWidget->clearHighlight();
        }
        else if (object == ui->ruleInputWidget) {
            handleRuleInputEditingFinished();
        }
        else if (object == ui->ruleOutputWidget) {
            handleRuleOutputEditingFinished();
        }
    }

    return QMainWindow::eventFilter(object, event);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::setUiMode(UiMode mode)
{
    switch (mode) {
    case DefaultUiMode:
        ui->categoryNameLabel->setVisible(false);
        ui->categoryNameTextEdit->setVisible(false);
        ui->ruleInputWidget->setVisible(false);
        ui->ruleOutputWidget->setVisible(false);
        ui->chatbotRepliesLabel->setVisible(false);
        break;

    case EditCategoryUiMode:
        ui->categoryNameLabel->setVisible(true);
        ui->categoryNameTextEdit->setVisible(true);
        ui->ruleInputWidget->setVisible(false);
        ui->ruleOutputWidget->setVisible(false);
        ui->chatbotRepliesLabel->setVisible(false);
        break;

    case EditRuleUiMode:
        ui->categoryNameLabel->setVisible(false);
        ui->categoryNameTextEdit->setVisible(false);
        ui->ruleInputWidget->setVisible(true);
        ui->ruleOutputWidget->setVisible(true);
        ui->chatbotRepliesLabel->setVisible(true);
        ui->chatbotRepliesLabel->setText(tr("Chatbot replies:"));
        break;

    case EditEvasivesUiMode:
        ui->categoryNameLabel->setVisible(false);
        ui->categoryNameTextEdit->setVisible(false);
        ui->ruleInputWidget->setVisible(false);
        ui->ruleOutputWidget->setVisible(true);
        ui->chatbotRepliesLabel->setVisible(true);
        ui->chatbotRepliesLabel->setText(tr("If chatbot does not understand, it replies:"));
        break;
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::clear()
{
    setUiMode(DefaultUiMode);

    // reset active tabs
    ui->mainTabWidget->setCurrentIndex(0);
    ui->rightSideTabWidget->setCurrentIndex(0);

    // train tab widgets
    // TODO clear ui->categoriesTree
    ui->ruleInputWidget->clear();
    ui->ruleOutputWidget->clear();

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

//--------------------------------------------------------------------------------------------------

Lvk::BE::Rule *Lvk::FE::MainWindow::addCategory(const QString &name)
{
    BE::Rule *category = new BE::Rule(name, m_ruleTreeModel->invisibleRootItem());

    category->setType(BE::Rule::ContainerRule);

    bool appended = m_ruleTreeModel->appendItem(category);

    if (appended) {
        return category;
    } else {
        delete category;

        return 0;
    }
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::Rule *Lvk::FE::MainWindow::addRule(const QString &name, BE::Rule *category)
{
    BE::Rule *rule = new BE::Rule(name, category);

    bool appended = m_ruleTreeModel->appendItem(rule);

    if (appended) {
        return rule;
    } else {
        delete rule;

        return 0;
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::addCategoryWithInputDialog()
{
    bool ok;
    QString name = QInputDialog::getText(this, tr("Add category"), tr("Category name:"),
                                         QLineEdit::Normal, "", &ok);

    if (ok) {
        if (!name.isEmpty()) {
            BE::Rule *category = addCategory(name);

            if (category) {
                selectRule(category);
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

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::addRuleWithInputDialog()
{
    QModelIndexList selectedRows = m_ruleTreeSelectionModel->selectedRows();

    if (selectedRows.size() <= 0) {
        QMessageBox msg(QMessageBox::Information, tr("Add rule"),
                        tr("Select the category where the rule will belong to"),
                        QMessageBox::Ok, this);
        msg.exec();

        return;
    }

    QModelIndex selectedIndex = selectedRows.first();
    BE::Rule *selectedItem = m_ruleTreeModel->itemFromIndex(selectedIndex);
    BE::Rule *parentCategory = 0;

    if (selectedItem->type() == BE::Rule::ContainerRule) {
        parentCategory = selectedItem;
        ui->categoriesTree->setExpanded(selectedIndex, true);
    } else if (selectedItem->type() == BE::Rule::OrdinaryRule) {
        parentCategory =  m_ruleTreeModel->itemFromIndex(selectedIndex.parent());
        ui->categoriesTree->setExpanded(selectedIndex.parent(), true);
    } else if (selectedItem->type() == BE::Rule::EvasiveRule) {
        QMessageBox msg(QMessageBox::Critical, tr("Add rule"),
                        tr("Evasives is an special category that cannot contain rules"),
                        QMessageBox::Ok, this);
        msg.exec();
    }

    if (parentCategory) {
        BE::Rule *emptyRule = addRule("", parentCategory);

        if (emptyRule) {
            selectRule(emptyRule);
            ui->ruleInputWidget->setFocusOnInput();
        } else {
            QMessageBox msg(QMessageBox::Critical, tr("Internal error"),
                            tr("The rule could not be added because of an internal error"),
                            QMessageBox::Ok, this);
            msg.exec();
        }
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::removeSelectedItem()
{
    QModelIndexList selectedRows = m_ruleTreeSelectionModel->selectedRows();

    if (selectedRows.size() <= 0) {
        QMessageBox msg(QMessageBox::Critical, tr("Remove rule or category"),
                        tr("Select the rule or category you want to remove"),
                        QMessageBox::Ok, this);
        msg.exec();

        return;
    }

    QModelIndex selectedIndex = selectedRows[0];
    BE::Rule *selectedItem = m_ruleTreeModel->itemFromIndex(selectedIndex);
    BE::Rule::RuleType ruleType = selectedItem->type();

    QString dialogText;
    QString dialogTitle;

    if (ruleType == BE::Rule::ContainerRule || ruleType == BE::Rule::OrdinaryRule) {
        if (selectedItem->type() == BE::Rule::ContainerRule) {
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
                                tr("The rule/category could not be removed because of an internal"
                                   "error"), QMessageBox::Ok, this);
                msg.exec();
            }
        }
    } else if (ruleType == BE::Rule::EvasiveRule) {
        dialogTitle = tr("Cannot remove");
        dialogText = tr("The selected category cannot be removed because is mandatory");

        QMessageBox msg(QMessageBox::Information, dialogTitle, dialogText, QMessageBox::Ok, this);

        msg.exec();

        return;
    }
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::Rule * Lvk::FE::MainWindow::rootRule()
{
    return m_ruleTreeModel->invisibleRootItem();
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::Rule * Lvk::FE::MainWindow::evasiveRule()
{
    if (rootRule()) {
        for (int i = 0; i < rootRule()->childCount(); ++i) {
            if (rootRule()->child(i)->type() == BE::Rule::EvasiveRule) {
                return rootRule()->child(i);
            }
        }
    }

    return 0;
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::Rule * Lvk::FE::MainWindow::selectedRule()
{
    if (!m_ruleTreeSelectionModel->selectedIndexes().isEmpty()) {
        QModelIndex selectedIndex = m_ruleTreeSelectionModel->selectedIndexes().first();
        if (selectedIndex.isValid()) {
            return m_ruleTreeModel->itemFromIndex(selectedIndex);
        }
    }
    return 0;
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::selectRule(BE::Rule *rule)
{
    if (!rule) {
        return;
    }

    QModelIndex ruleIndex = m_ruleTreeModel->indexFromItem(rule);

    if (ruleIndex.isValid()) {
        m_ruleTreeSelectionModel->setCurrentIndex(ruleIndex, QItemSelectionModel::ClearAndSelect);

        ui->categoriesTree->setExpanded(ruleIndex.parent(), true);
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::handleRuleInputEdited(const QString &ruleInput)
{
    if (!m_ruleTreeSelectionModel->selectedIndexes().isEmpty()) {
        QModelIndex selectedIndex = m_ruleTreeSelectionModel->selectedIndexes().first();
        m_ruleTreeModel->setData(selectedIndex, ruleInput, Qt::EditRole);
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::handleRuleOutputEditingFinished()
{
    BE::Rule *rule = selectedRule();
    if (rule) {
        rule->setOutput(ui->ruleOutputWidget->outputList());
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::handleRuleInputEditingFinished()
{
    BE::Rule *rule = selectedRule();
    if (rule) {
        rule->setInput(ui->ruleInputWidget->inputList());
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::handleRuleSelectionChanged(const QItemSelection &selected,
                                            const QItemSelection &deselected)
{
    if (!deselected.indexes().isEmpty()) {
        BE::Rule *item = static_cast<BE::Rule *>(deselected.indexes().first().internalPointer());

        if (item->type() == BE::Rule::OrdinaryRule) {
            item->setInput(ui->ruleInputWidget->inputList());
            item->setOutput(ui->ruleOutputWidget->outputList());
        } else if (item->type() == BE::Rule::EvasiveRule) {
            item->setOutput(ui->ruleOutputWidget->outputList());
        } else if (item->type() == BE::Rule::ContainerRule) {
           // nothing to do
        }
    }

    if (!selected.indexes().isEmpty()) {
        BE::Rule *item = static_cast<BE::Rule *>(selected.indexes().first().internalPointer());

        if (item->type() == BE::Rule::OrdinaryRule) {

            setUiMode(EditRuleUiMode);

            ui->categoryNameTextEdit->clear();
            ui->ruleInputWidget->setInputList(item->input());
            ui->ruleOutputWidget->setOutputList(item->output());
        } else if (item->type() == BE::Rule::EvasiveRule) {

            setUiMode(EditEvasivesUiMode);

            ui->categoryNameTextEdit->clear();
            ui->ruleInputWidget->clear();
            ui->ruleOutputWidget->setOutputList(item->output());
        } else if (item->type() == BE::Rule::ContainerRule) {

            setUiMode(EditCategoryUiMode);

            ui->categoryNameTextEdit->setText(item->name());
            ui->ruleInputWidget->clear();
            ui->ruleOutputWidget->clear();
        }
    } else {

        setUiMode(DefaultUiMode);

        ui->categoryNameLabel->clear();
        ui->ruleInputWidget->clear();
        ui->ruleOutputWidget->clear();
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::testInputTextEntered()
{
    m_coreApp->refreshNlpEngine(); // FIXME not performant!

    QString input = ui->testInputText->text();
    BE::CoreApp::MatchList matches;

    QString response = m_coreApp->getResponse(input, matches);

    ui->testConversationText->appendConversation(input, response, !matches.isEmpty());
    ui->testInputText->setText("");

    highlightMatchedRules(matches);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::highlightMatchedRules(const BE::CoreApp::MatchList &matches)
{
    ui->ruleInputWidget->clearHighlight();
    ui->ruleOutputWidget->clearHighlight();

    if (!matches.empty()) {
        // Assuming only one match

        BE::Rule *rule = matches.first().first;
        int ruleNumber = matches.first().second;

        selectRule(rule);

        ui->ruleInputWidget->highlightInput(ruleNumber);
    } else {
        selectRule(evasiveRule());

        ui->ruleOutputWidget->highlightOuput(0); // FIXME hardcoded 0
    }
}




