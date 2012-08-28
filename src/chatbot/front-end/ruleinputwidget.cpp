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

#include "front-end/ruleinputwidget.h"
#include "front-end/autocompletetextedit.h"

#include <QPlainTextEdit>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QEvent>
#include <QPushButton>
#include <QToolButton>
#include <QSpacerItem>

#define TARGET_SPLIT_TOKEN  ","

//--------------------------------------------------------------------------------------------------
// RuleInputWidget
//--------------------------------------------------------------------------------------------------

Lvk::FE::RuleInputWidget::RuleInputWidget(QWidget *parent) :
    QWidget(parent),
    m_layout(new QVBoxLayout(this)),
    m_targetLabel(new QLabel(tr("If: Any user"), this)),
    m_selectUsersButton(new QToolButton(this)),
    m_targetTextEdit(new Lvk::FE::AutocompleteTextEdit(this)),
    m_inputLabel(new QLabel(tr("Writes:"), this)),
    m_input(new QLineEdit(this)),
    m_inputVariantsLabel(new QLabel(tr("Or any of these variants:"), this)),
    m_inputVariants(new QPlainTextEdit(this)),
    m_eventFilter(0),
    m_strAnyUser(tr("Any user"))
{
    setupUi();

    connectSignals();
}

//--------------------------------------------------------------------------------------------------

Lvk::FE::RuleInputWidget::~RuleInputWidget()
{
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::RuleInputWidget::setupUi()
{
    setLayout(m_layout);

    m_layout->setMargin(0);

    QHBoxLayout *selectUsersInnerLayout = new QHBoxLayout(m_layout);

    m_layout->addWidget(m_targetTextEdit);
    m_layout->addWidget(m_inputLabel);
    m_layout->addWidget(m_input);
    m_layout->addWidget(m_inputVariantsLabel);
    m_layout->addWidget(m_inputVariants);

    selectUsersInnerLayout->addWidget(m_targetLabel);
    selectUsersInnerLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Maximum));
    selectUsersInnerLayout->addWidget(m_selectUsersButton);

    m_input->installEventFilter(this);
    m_inputVariants->installEventFilter(this);
    m_inputVariants->setTabChangesFocus(true);

    m_targetTextEdit->setDelimiter(TARGET_SPLIT_TOKEN);
    m_targetTextEdit->setVisible(false);
    m_targetTextEdit->setPlaceholderText(m_strAnyUser);

    m_selectUsersButton->setIcon(QIcon(":/icons/users.png"));
    m_selectUsersButton->setText(tr("Select users"));
    m_selectUsersButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

#ifndef USE_SELECT_USERS_BUTTON
    onSelectUsersButtonClicked();
#endif

}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::RuleInputWidget::connectSignals()
{
    connect(m_selectUsersButton, SIGNAL(clicked()), SLOT(onSelectUsersButtonClicked()));

    connect(m_input, SIGNAL(textEdited(QString)), SIGNAL(inputTextEdited(QString)));

    connect(m_targetTextEdit, SIGNAL(textEdited(QString)), SIGNAL(targetTextEdited(QString)));

    connectTextChangedSignal();
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::RuleInputWidget::installEventFilter(QObject *eventFilter)
{
    m_eventFilter = eventFilter;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::FE::RuleInputWidget::eventFilter(QObject */*object*/, QEvent *event)
{
    if (m_eventFilter) {
        if (!m_eventFilter->eventFilter(this, event)) {
            return false;
        }
    }

    return QWidget::eventFilter(this, event);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::RuleInputWidget::clear()
{
    m_input->clear();
    m_targetTextEdit->clear();
    m_reverseRoster.clear();

    // QTBUG-8449: Signal textEdited() is missing in QTextEdit and QPlainTextEdit
    disconnectTextChangedSignal();
    m_inputVariants->clear();
    connectTextChangedSignal();

    clearHighlight();
}

//--------------------------------------------------------------------------------------------------

QStringList Lvk::FE::RuleInputWidget::input()
{
    QStringList inputList = m_inputVariants->toPlainText().split("\n", QString::SkipEmptyParts);
    inputList.prepend(m_input->text());

    return inputList;
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::RuleInputWidget::setInput(const QStringList &inputList)
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

Lvk::BE::TargetList Lvk::FE::RuleInputWidget::targets()
{
    Lvk::BE::TargetList targets;

    QStringList dispTexts =
            m_targetTextEdit->text().split(TARGET_SPLIT_TOKEN, QString::SkipEmptyParts);

    foreach (QString dispText, dispTexts) {
        dispText = dispText.trimmed();

        if (dispText == m_strAnyUser) {
            continue;
        }

        ReverseRoster::const_iterator it = m_reverseRoster.find(dispText);
        if (it != m_reverseRoster.end()) {
            targets.append(*it);
        } else {
            targets.append(Lvk::BE::Target(dispText, dispText));
        }
    }

    return targets;
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::RuleInputWidget::setTargets(const Lvk::BE::TargetList &targets)
{
    QString targetText;

    foreach (const Lvk::BE::Target &target, targets) {
        if (!targetText.isEmpty()) {
            targetText.append(TARGET_SPLIT_TOKEN);
        }
        targetText.append(target.displayText());
    }

    m_targetTextEdit->setText(targetText);

#ifdef USE_SELECT_USERS_BUTTON
    if (targetText.isEmpty()) {
        m_targetLabel->setText(tr("If: Any user"));
    } else {
        m_targetLabel->setText(QString(tr("If: %1")).arg(targetText));
    }
#endif //USE_SELECT_USERS_BUTTON
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::RuleInputWidget::setRoster(const Lvk::BE::Roster &roster)
{
    m_reverseRoster.clear();

    QStringList strList;

    foreach (const Lvk::BE::RosterItem &item, roster) {
        m_reverseRoster.insert(item.displayText(), item);
        strList.append(item.displayText());
    }

    m_targetTextEdit->setVocabulary(strList);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::RuleInputWidget::setFocusOnInput()
{
    m_input->setFocus();
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::RuleInputWidget::setFocusOnInputVariants()
{
    m_inputVariants->setFocus();
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::RuleInputWidget::highlightInput(int number)
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

void Lvk::FE::RuleInputWidget::clearHighlight()
{
    m_inputVariants->setStyleSheet("");
    m_input->setStyleSheet("");
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::RuleInputWidget::connectTextChangedSignal()
{
    connect(m_inputVariants, SIGNAL(textChanged()), SIGNAL(inputVariantsEdited()));
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::RuleInputWidget::disconnectTextChangedSignal()
{
    disconnect(m_inputVariants, SIGNAL(textChanged()), this, SIGNAL(inputVariantsEdited()));
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::RuleInputWidget::onSelectUsersButtonClicked()
{
    m_targetLabel->setText(tr("If:"));
    m_selectUsersButton->setVisible(false);
    m_targetTextEdit->setVisible(true);
    m_targetTextEdit->setFocus();
}




