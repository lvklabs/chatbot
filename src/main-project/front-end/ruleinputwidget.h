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

#ifndef RULEINPUTWIDGET_H
#define RULEINPUTWIDGET_H

#include <QWidget>
#include <QString>
#include <QStringList>
#include <QHash>

#include "roster.h"
#include "target.h"

class QLineEdit;
class QPlainTextEdit;
class QLayout;
class QLabel;
class QPushButton;

namespace Lvk
{

namespace FE
{

class AutocompleteTextEdit;

} //namespace FE

} //namespace Lvk


/**
 * \brief The RuleInputWidget class provides a widget to edit or display NLP rule inputs
 */

class RuleInputWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RuleInputWidget(QWidget *parent = 0);

    ~RuleInputWidget();

    void clear();

    QStringList input();

    void setInput(const QStringList &input);

    Lvk::BE::TargetList targets();

    void setTargets(const Lvk::BE::TargetList &target);

    void setRoster(const Lvk::BE::Roster &roster);

    void setFocusOnInput();

    void setFocusOnInputVariants();

    void installEventFilter(QObject *eventFilter);

    bool eventFilter(QObject *object, QEvent *event);

public slots:

    void clearHighlight();

    void highlightInput(int number);

signals:
    void inputTextEdited(const QString &);

    void targetTextEdited(const QString &);

    void inputVariantsEdited();

private:
    RuleInputWidget(RuleInputWidget&);
    RuleInputWidget& operator=(RuleInputWidget&);

    QLayout                       *m_layout;
    QLabel                        *m_targetLabel;
    QPushButton                   *m_selectUsersButton;
    Lvk::FE::AutocompleteTextEdit *m_targetTextEdit;
    QLabel                        *m_inputLabel;
    QLineEdit                     *m_input;
    QLabel                        *m_inputVariantsLabel;
    QPlainTextEdit                *m_inputVariants;
    QObject                       *m_eventFilter;

    typedef QHash<QString, Lvk::BE::RosterItem> ReverseRoster;
    ReverseRoster m_reverseRoster;

    void setupUi();
    void connectSignals();
    void connectTextChangedSignal();
    void disconnectTextChangedSignal();

private slots:
    void onSelectUsersButtonClicked();
};

#endif // RULEINPUTWIDGET_H
