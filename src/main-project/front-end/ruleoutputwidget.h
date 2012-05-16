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

#ifndef RULEOUTPUTWIDGET_H
#define RULEOUTPUTWIDGET_H

#include <QPlainTextEdit>
#include <QStringList>

/**
 * \brief The RuleInputWidget class provides a widget to edit or display NLP rule outputs
 */

class RuleOutputWidget : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit RuleOutputWidget(QWidget *parent = 0);

    QStringList output();

    void setOutput(const QStringList &output);

public slots:

    void clearHighlight();

    void highlightOuput(int number);

    void setPlainText(const QString &text);

    void clear();

signals:

    void outputTextEdited();

private:
    void connectTextChangedSignal();
    void disconnectTextChangedSignal();
};

#endif // RULEOUTPUTWIDGET_H
