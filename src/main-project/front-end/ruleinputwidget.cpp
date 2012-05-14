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
#include "autocompletetextedit.h"

#include <QPlainTextEdit>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QEvent>
#include <QPushButton>
#include <QSpacerItem>

//--------------------------------------------------------------------------------------------------
// RuleInputWidget
//--------------------------------------------------------------------------------------------------

RuleInputWidget::RuleInputWidget(QWidget *parent) :
    QWidget(parent),
    m_layout(new QVBoxLayout(this)),
    m_targetLabel(new QLabel(tr("If: Any user"), this)),
    m_selectUsersButton(new QPushButton(tr("Edit users"), this)),
    m_target(new Lvk::FE::AutocompleteTextEdit(this)),
    m_inputLabel(new QLabel(tr("Writes:"), this)),
    m_input(new QLineEdit(this)),
    m_inputVariantsLabel(new QLabel(tr("Or any of these variants:"), this)),
    m_inputVariants(new QPlainTextEdit(this)),
    m_eventFilter(0)
{
    setupUi();

    connect(m_selectUsersButton, SIGNAL(clicked()), SLOT(onSelectUsersButtonClicked()));

    connect(m_input, SIGNAL(textEdited(QString)), SIGNAL(inputTextEdited(QString)));

    connectTextChangedSignal();
}

//--------------------------------------------------------------------------------------------------

RuleInputWidget::~RuleInputWidget()
{
}

//--------------------------------------------------------------------------------------------------

void RuleInputWidget::setupUi()
{
    setLayout(m_layout);

    m_layout->setMargin(0);

    QHBoxLayout *selectUsersInnerLayout = new QHBoxLayout(m_layout);

    m_layout->addWidget(m_target);
    m_layout->addWidget(m_inputLabel);
    m_layout->addWidget(m_input);
    m_layout->addWidget(m_inputVariantsLabel);
    m_layout->addWidget(m_inputVariants);

    selectUsersInnerLayout->addWidget(m_targetLabel);
    selectUsersInnerLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Maximum));
    selectUsersInnerLayout->addWidget(m_selectUsersButton);

    m_input->installEventFilter(this);
    m_inputVariants->installEventFilter(this);

    m_target->setDelimiter(",");
    m_target->setVisible(false);

    m_selectUsersButton->setIcon(QIcon(":/icons/users_32x32.png"));

    ///////////////////////////////////////
    // FIXME not using select users button
    onSelectUsersButtonClicked();
    ///////////////////////////////////////
}

//--------------------------------------------------------------------------------------------------

void RuleInputWidget::installEventFilter(QObject *eventFilter)
{
    m_eventFilter = eventFilter;
}

//--------------------------------------------------------------------------------------------------

bool RuleInputWidget::eventFilter(QObject */*object*/, QEvent *event)
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

QString RuleInputWidget::targets()
{
    return m_target->text();
}

//--------------------------------------------------------------------------------------------------

void RuleInputWidget::setTargets(const QString &targets)
{
    m_target->setText(targets);
}

//--------------------------------------------------------------------------------------------------

void RuleInputWidget::setRoster(const Lvk::BE::Roster &roster)
{
    QStringList strList;
    foreach (const Lvk::BE::RosterItem &item, roster) {
        strList.append(!item.fullname.isEmpty() ? item.fullname : item.username);
    }
    m_target->setStringList(strList);
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

//--------------------------------------------------------------------------------------------------

void RuleInputWidget::onSelectUsersButtonClicked()
{
    m_targetLabel->setText(tr("If:"));
    m_selectUsersButton->setVisible(false);
    m_target->setVisible(true);
    m_target->setFocus();
}




