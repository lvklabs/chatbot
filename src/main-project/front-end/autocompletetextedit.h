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
#include <QStringList>

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

    void setStringList(const QStringList &strList);

    const QStringList &stringList();

    void setDelimiter(const QString &delim);

    const QString &delimiter();

protected:
    virtual void keyPressEvent(QKeyEvent *event);

    virtual void focusOutEvent(QFocusEvent *event);

    virtual void focusInEvent(QFocusEvent *event);

    virtual void mouseReleaseEvent(QMouseEvent *);

    virtual bool event(QEvent *event);

    void paintEvent ( QPaintEvent * event );

private:
    bool m_init;
    QString m_delimiter;
    QPoint m_globalPos;
    QStringList m_strList;
    QListWidget *m_listWidget;
    QString m_head;
    QString m_current;
    QString m_tail;
    QString m_startString;

    void updateListWidgetGeometry();
    void updateTextParts();

private slots:
    void onTargetTextEdited(QString);
    void onListItemSelected();
};

} // namespace FE

} // namespace Lvk

#endif // LVK_FE_AUTOCOMPLETETEXTEDIT_H
