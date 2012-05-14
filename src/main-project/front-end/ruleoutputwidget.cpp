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

#include "ruleoutputwidget.h"

//--------------------------------------------------------------------------------------------------
// RuleOutputWidget
//--------------------------------------------------------------------------------------------------

RuleOutputWidget::RuleOutputWidget(QWidget *parent) :
    QPlainTextEdit(parent)
{
    connectTextChangedSignal();
}

//--------------------------------------------------------------------------------------------------

QStringList RuleOutputWidget::output()
{
    return toPlainText().split("\n", QString::SkipEmptyParts);
}

//--------------------------------------------------------------------------------------------------

void RuleOutputWidget::setOutputList(const QStringList &outputList)
{
    QString output;

    for (int i = 0; i < outputList.size(); ++i) {
        QString trimmed = outputList[i].trimmed();
        if (!trimmed.isEmpty()) {
            output += trimmed + "\n";
        }
    }

    setPlainText(output);
}

//--------------------------------------------------------------------------------------------------

void RuleOutputWidget::highlightOuput(int /*number*/)
{
    static const QString HIGHLIGHT_INPUT_CSS = "background-color: rgba(255,128,128,128);";

    setStyleSheet(HIGHLIGHT_INPUT_CSS);
}

//--------------------------------------------------------------------------------------------------

void RuleOutputWidget::clearHighlight()
{
    setStyleSheet("");
}

//--------------------------------------------------------------------------------------------------

void RuleOutputWidget::connectTextChangedSignal()
{
    connect(this, SIGNAL(textChanged()), SIGNAL(outputTextEdited()));
}

//--------------------------------------------------------------------------------------------------

void RuleOutputWidget::disconnectTextChangedSignal()
{
    disconnect(this, SIGNAL(textChanged()), this, SIGNAL(outputTextEdited()));
}

//--------------------------------------------------------------------------------------------------

void RuleOutputWidget::setPlainText(const QString &text)
{
    // QTBUG-8449: Signal textEdited() is missing in QTextEdit and QPlainTextEdit
    disconnectTextChangedSignal();
    QPlainTextEdit::setPlainText(text);
    connectTextChangedSignal();
}

//--------------------------------------------------------------------------------------------------

void RuleOutputWidget::clear()
{
    // QTBUG-8449: Signal textEdited() is missing in QTextEdit and QPlainTextEdit
    disconnectTextChangedSignal();
    QPlainTextEdit::clear();
    connectTextChangedSignal();
}
