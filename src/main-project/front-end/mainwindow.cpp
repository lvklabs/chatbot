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

    clear();

    m_coreApp->load("FIXME");

    m_ruleTreeModel = new FE::RuleTreeModel(m_coreApp->rootRule(), this);
    m_ruleTreeModel->setHeaderData(0, Qt::Horizontal, QString(tr("Rules")), Qt::DisplayRole);

    ui->categoriesTree->setModel(m_ruleTreeModel);

    m_ruleTreeSelectionModel = ui->categoriesTree->selectionModel();

    connect(ui->addCategoryButton, SIGNAL(clicked()), SLOT(addCategoryWithInputDialog()));
    connect(ui->addRuleButton,     SIGNAL(clicked()), SLOT(addRuleWithInputDialog()));
    connect(ui->rmItemButton,      SIGNAL(clicked()), SLOT(removeSelectedItem()));

    connect(ui->testInputText, SIGNAL(returnPressed()), SLOT(testInputTextEntered()));

    connect(ui->clearTestConversationButton,
            SIGNAL(clicked()),
            ui->testConversationText,
            SLOT(clear()));

    connect(ui->categoriesTree->selectionModel(),
            SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            SLOT(handleRuleSelectionChanged(QItemSelection,QItemSelection)));

    connect(ui->ruleInputText,
            SIGNAL(textEdited(QString)),
            SLOT(handleRuleInputChanged(QString)));


}

Lvk::FE::MainWindow::~MainWindow()
{
    delete ui;
    delete m_coreApp;
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
    ui->ruleInputText->clear();
    ui->ruleInputVariantsText->clear();
    ui->ruleOutputText->clear();
    ui->ifUserWritesLabel->clear();

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

void Lvk::FE::MainWindow::addCategoryWithInputDialog()
{
    bool ok;
    QString name = QInputDialog::getText(this, tr("Add category"), tr("Category name:"),
                                         QLineEdit::Normal, "", &ok);

    if (ok) {
        if (!name.isEmpty()) {
            BE::Rule *category = addCategory(name);

            if (category) {
                QModelIndex categoryIndex = m_ruleTreeModel->indexFromItem(category);
                m_ruleTreeSelectionModel->setCurrentIndex(categoryIndex,
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

    QModelIndex selectedIndex = selectedRows[0];
    BE::Rule *selectedItem = m_ruleTreeModel->itemFromIndex(selectedIndex);
    BE::Rule *parentCategory = 0;

    if (selectedItem->type() == BE::Rule::ContainerRule) {
        parentCategory = selectedItem;
        ui->categoriesTree->setExpanded(selectedIndex, true);
    } else {
        parentCategory =  m_ruleTreeModel->itemFromIndex(selectedIndex.parent());
        ui->categoriesTree->setExpanded(selectedIndex.parent(), true);
    }

    BE::Rule *emptyRule = addRule("", parentCategory);

    if (emptyRule) {
        m_ruleTreeSelectionModel->select(m_ruleTreeModel->indexFromItem(emptyRule),
                                           QItemSelectionModel::ClearAndSelect);
        ui->ruleInputText->setFocus();
    } else {
        QMessageBox msg(QMessageBox::Critical, tr("Internal error"),
                        tr("The rule could not be added because of an internal error"),
                        QMessageBox::Ok, this);
        msg.exec();
    }
}

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

    QString dialogText;
    QString dialogTitle;

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
                            tr("The rule/category could not be removed because of an internal error"),
                            QMessageBox::Ok, this);
            msg.exec();
        }
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::handleRuleInputChanged(const QString &ruleInput)
{
    if (m_ruleTreeSelectionModel->selectedIndexes().size() > 0) {
        QModelIndex selectedIndex = m_ruleTreeSelectionModel->selectedIndexes()[0];
        m_ruleTreeModel->setData(selectedIndex, ruleInput, Qt::EditRole);
    }
}

void Lvk::FE::MainWindow::handleRuleSelectionChanged(const QItemSelection &selected,
                                            const QItemSelection &deselected)
{
    if (deselected.indexes().size() > 0) {
        BE::Rule *item = static_cast<BE::Rule *>(deselected.indexes()[0].internalPointer());

        if (item->type() == BE::Rule::FinalRule) {
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
        BE::Rule *item = static_cast<BE::Rule *>(selected.indexes()[0].internalPointer());

        if (item->type() == BE::Rule::FinalRule) {
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

            setUiMode(EditRuleUiMode);

            ui->ruleInputText->setText(input);
            ui->ruleInputVariantsText->setPlainText(inputVariants);
            ui->ruleOutputText->setPlainText(output);

        } else {
            setUiMode(EditCategoryUiMode);

            ui->ruleInputText->setText(item->name());
            ui->ruleInputVariantsText->setPlainText("");
            ui->ruleOutputText->setPlainText("");
        }
    } else {
        setUiMode(DefaultUiMode);

        ui->ruleInputText->setText("");
        ui->ruleInputVariantsText->setPlainText("");
        ui->ruleOutputText->setPlainText("");
    }
}

void Lvk::FE::MainWindow::setUiMode(UiMode mode)
{
    switch (mode) {
    case DefaultUiMode:
        ui->ruleInputText->setVisible(false);
        ui->ruleInputVariantsText->setVisible(false);
        ui->ruleOutputText->setVisible(false);

        ui->ifUserWritesLabel->setVisible(false);
        ui->orVariantsLabel->setVisible(false);
        ui->chatbotRepliesLabel->setVisible(false);

        ui->ifUserWritesLabel->setText("");
        break;

    case EditCategoryUiMode:
        ui->ruleInputText->setVisible(true);
        ui->ruleInputVariantsText->setVisible(false);
        ui->ruleOutputText->setVisible(false);

        ui->ifUserWritesLabel->setVisible(true);
        ui->orVariantsLabel->setVisible(false);
        ui->chatbotRepliesLabel->setVisible(false);

        ui->ifUserWritesLabel->setText(tr("Category name:"));
        break;

    case EditRuleUiMode:
        ui->ruleInputText->setVisible(true);
        ui->ruleInputVariantsText->setVisible(true);
        ui->ruleOutputText->setVisible(true);

        ui->ifUserWritesLabel->setVisible(true);
        ui->orVariantsLabel->setVisible(true);
        ui->chatbotRepliesLabel->setVisible(true);

        ui->ifUserWritesLabel->setText(tr("If user writes:"));
        break;
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::testInputTextEntered()
{
    m_coreApp->refreshNlpEngine(); // FIXME not performant!

    QString input = ui->testInputText->text();
    QList<BE::Rule *> matched;

    QString response = m_coreApp->getResponse(input, matched);

    appendTestConversation(input, response, matched.size());
    ui->testInputText->setText("");

    highlightMatchedRules(matched);
}

void Lvk::FE::MainWindow::appendTestConversation(const QString &input, const QString &response_,
                                                 bool match)
{
    QString response = response_;

    static const QString START_USER_SPAN = "<span style=\" color:#000088;\">";
    static const QString START_CHATBOT_SPAN = "<span style=\" color:#008800;\">";
    static const QString START_NOT_MATCH_SPAN = "<span style=\" color:#880000;\">";
    static const QString END_SPAN = "</span>";
    static const QString BR = "<br/>";
    static const QString END_P_BODY_HTML = "</p></body></html>";

    if (!match) {
        response = START_NOT_MATCH_SPAN + response + END_SPAN;
    }

    QString conversation = ui->testConversationText->toHtml();

    conversation.remove(END_P_BODY_HTML);
    conversation += START_USER_SPAN + tr("You:") + " " + END_SPAN + input + BR;
    conversation += START_CHATBOT_SPAN + tr("Chatbot:") + " " + END_SPAN + response + BR;
    conversation += END_P_BODY_HTML;

    ui->testConversationText->setHtml(conversation);
}

void Lvk::FE::MainWindow::highlightMatchedRules(const QList<BE::Rule *> &matched)
{
    if (matched.empty()) {
        return;
    }

    // Assuming only one match

    BE::Rule *rule = matched.first();

    QModelIndex ruleIndex = m_ruleTreeModel->indexFromItem(rule);

    if (ruleIndex.isValid()) {
        m_ruleTreeSelectionModel->setCurrentIndex(ruleIndex, QItemSelectionModel::ClearAndSelect);

        ui->categoriesTree->setExpanded(ruleIndex.parent(), true);
    }
}
