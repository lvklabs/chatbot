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

#ifndef LVK_FE_AUTOCOMPLETETEXTEDIT_H
#define LVK_FE_AUTOCOMPLETETEXTEDIT_H

#include <QLineEdit>

class QListWidget;

namespace Lvk
{

namespace FE
{

/**
 * \brief The AutoCompleteTextEdit class provides a widget that is used to edit text like a
 *        QPlainTextEdit but it also displays a list that suggests words to autocomplete
 */

class AutocompleteTextEdit : public QLineEdit
{
    Q_OBJECT

public:
    explicit AutocompleteTextEdit(QWidget *parent = 0);

    void keyPressEvent(QKeyEvent *event);

private:
    QListWidget *m_list;

private slots:
    void onTargetTextEdited(QString);
    void onTargetLostFocus();
};

} // namespace FE

} // namespace Lvk

#endif // LVK_FE_AUTOCOMPLETETEXTEDIT_H
