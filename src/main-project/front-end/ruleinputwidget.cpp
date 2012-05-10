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

#include "ruleinputwidget.h"

#include <QPlainTextEdit>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QMainWindow>
#include <QEvent>

#include <iostream>

//--------------------------------------------------------------------------------------------------
// RuleInputWidget
//--------------------------------------------------------------------------------------------------

RuleInputWidget::RuleInputWidget(QWidget *parent) :
    QWidget(parent),
    m_layout(new QVBoxLayout(this)),
    m_targetLabel(new QLabel(tr("If:"), this)),
    m_target(new QLineEdit(this)),
    m_inputLabel(new QLabel(tr("Writes:"), this)),
    m_input(new QLineEdit(this)),
    m_inputVariantsLabel(new QLabel(tr("Or any of these variants:"), this)),
    m_inputVariants(new QPlainTextEdit(this)),
    m_eventFilter(0)
{
    m_layout->setMargin(0);

    m_layout->addWidget(m_targetLabel);
    m_layout->addWidget(m_target);
    m_layout->addWidget(m_inputLabel);
    m_layout->addWidget(m_input);
    m_layout->addWidget(m_inputVariantsLabel);
    m_layout->addWidget(m_inputVariants);

    setLayout(m_layout);

    connect(m_input, SIGNAL(textEdited(QString)), SIGNAL(inputTextEdited(QString)));
    connectTextChangedSignal();

    connect(m_target, SIGNAL(textEdited(QString)), SLOT(onTargetTextEdited(QString)));
    connect(m_target, SIGNAL(lostFocus()), SLOT(onTargetLostFocus()));

    m_input->installEventFilter(this);
    m_inputVariants->installEventFilter(this);

    ///////////////////////////////////////////////////////////////////

    m_targetLabel->setVisible(false);
    m_target->setVisible(false);

    m_list = new QListWidget(m_target);
    m_list->setWindowFlags(Qt::ToolTip | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
    m_list->addItem("Andres Pagliano");
    m_list->addItem("Andres Calamaro");
    m_list->addItem("Andrea Prodan");
    m_list->addItem("Luciana Benotti");
    m_list->addItem("Emilia Echeveste");
}

void RuleInputWidget::onTargetTextEdited(QString)
{
    QPoint pos = m_target->mapToGlobal(QPoint(0,0));

    m_list->setGeometry(pos.x(), pos.y() + m_target->height(), 300, 200);

    int matches = 0;

    if (m_target->text().size() > 0) {
        for (int i = 0; i < m_list->count(); ++i) {
            QListWidgetItem *item = m_list->item(i);
            bool match = item->text().contains(m_target->text(), false);
            if (match) {
                matches++;
            }
            item->setHidden(!match);
        }
    }

    if (matches > 0) {
        m_list->show();
    } else {
        m_list->hide();
    }
}

//--------------------------------------------------------------------------------------------------

void RuleInputWidget::onTargetLostFocus()
{
    m_list->hide();
}


//--------------------------------------------------------------------------------------------------

RuleInputWidget::~RuleInputWidget()
{
}

//--------------------------------------------------------------------------------------------------

void RuleInputWidget::installEventFilter(QObject *eventFilter)
{
    m_eventFilter = eventFilter;
}

//--------------------------------------------------------------------------------------------------

void RuleInputWidget::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();

    if (key == Qt::Key_Down && m_list->currentRow() + 1 < m_list->count()) {
        m_list->setCurrentRow(m_list->currentRow() + 1);
    }
    if (key == Qt::Key_Up && m_list->currentRow() > 0) {
        m_list->setCurrentRow(m_list->currentRow() - 1);
    }
    if ((key == Qt::Key_Enter || key == Qt::Key_Return) && m_list->currentRow() != -1) {
        m_target->setText(m_list->currentItem()->text() + ", ");
        m_list->hide();
    }

    QWidget::keyPressEvent(event);
}

//--------------------------------------------------------------------------------------------------

bool RuleInputWidget::eventFilter(QObject *object, QEvent *event)
{
    if (m_eventFilter) {
        if (!m_eventFilter->eventFilter(this, event)) {
            return false;
        }
    }

    return QWidget::eventFilter(this, event);
}

//--------------------------------------------------------------------------------------------------

void RuleInputWidget::clear()
{
    m_input->clear();

    // QTBUG-8449: Signal textEdited() is missing in QTextEdit and QPlainTextEdit
    disconnectTextChangedSignal();
    m_inputVariants->clear();
    connectTextChangedSignal();

    clearHighlight();
}

//--------------------------------------------------------------------------------------------------

QStringList RuleInputWidget::inputList()
{
    QStringList inputList = m_inputVariants->toPlainText().split("\n", QString::SkipEmptyParts);
    inputList.prepend(m_input->text());

    return inputList;
}

//--------------------------------------------------------------------------------------------------

void RuleInputWidget::setInputList(const QStringList &inputList)
{
    QString input, inputVariants;

    for (int i = 0; i < inputList.size(); ++i) {
        QString trimmed = inputList[i].trimmed();
        if (!trimmed.isEmpty()) {
            if (i == 0) {
                input = trimmed;
            } else {
                inputVariants += trimmed + "\n";
            }
        }
    }

    m_input->setText(input);

    // QTBUG-8449: Signal textEdited() is missing in QTextEdit and QPlainTextEdit
    disconnectTextChangedSignal();
    m_inputVariants->setPlainText(inputVariants);
    connectTextChangedSignal();
}

//--------------------------------------------------------------------------------------------------

void RuleInputWidget::setFocusOnInput()
{
    m_input->setFocus();
}

//--------------------------------------------------------------------------------------------------

void RuleInputWidget::setFocusOnInputVariants()
{
    m_inputVariants->setFocus();
}

//--------------------------------------------------------------------------------------------------

void RuleInputWidget::highlightInput(int number)
{
    static const QString HIGHLIGHT_INPUT_CSS = "background-color: rgba(0,128,0,128);";

    if (number == 0) {
        m_input->setStyleSheet(HIGHLIGHT_INPUT_CSS);
        m_inputVariants->setStyleSheet("");
    } else {
        m_input->setStyleSheet("");
        m_inputVariants->setStyleSheet(HIGHLIGHT_INPUT_CSS);
    }
}

//--------------------------------------------------------------------------------------------------

void RuleInputWidget::clearHighlight()
{
    m_inputVariants->setStyleSheet("");
    m_input->setStyleSheet("");
}

//--------------------------------------------------------------------------------------------------

void RuleInputWidget::connectTextChangedSignal()
{
    connect(m_inputVariants, SIGNAL(textChanged()), SIGNAL(inputVariantsEdited()));
}

//--------------------------------------------------------------------------------------------------

void RuleInputWidget::disconnectTextChangedSignal()
{
    disconnect(m_inputVariants, SIGNAL(textChanged()), this, SIGNAL(inputVariantsEdited()));
}


