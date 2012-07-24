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

#ifndef LVK_FE_AUTOCOMPLETETEXTEDIT_H
#define LVK_FE_AUTOCOMPLETETEXTEDIT_H

#include <QLineEdit>
#include <QStringList>

class QListWidget;
class QFrame;

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace FE
{

/// \ingroup Lvk
/// \addtogroup FE
/// @{

/**
 * \brief The AutoCompleteTextEdit class provides a widget that is used to edit text like a
 *        QPlainTextEdit but it also displays a list of suggestion to autocomplete words.
 *
 * By default, the widget does not contain any vocabulary to automplete. In order to set a
 * vocabulary you must invoke setVocabulary(). The autocomplete mechanism is no limited to
 * colloquial words, can be any kind of strings. In that case, consider changing the word delimiter
 * with setDelimiter()
 */
class AutocompleteTextEdit : public QLineEdit
{
    Q_OBJECT

public:

    /**
     * Constructs an AutocompleteTextEdit widget which is a child of \a parent.
     */
    explicit AutocompleteTextEdit(QWidget *parent = 0);

    /**
     * Sets the list of words used to autocomplete text
     */
    void setVocabulary(const QStringList &v);

    /**
     * Returns the list of words used to autocomplete text, by default is empty.
     */
    const QStringList &vocabulary();

    /**
     * Sets the token used to delimit words.
     */
    void setDelimiter(const QString &delim);

    /**
     * Returns the token used to delimit words, by default is one whitespace, i.e. " ".
     */
    const QString &delimiter();

protected:
    virtual void showEvent(QShowEvent *event);

    virtual void keyPressEvent(QKeyEvent *event);

    virtual void focusOutEvent(QFocusEvent *event);

    virtual void focusInEvent(QFocusEvent *event);

    virtual void mouseReleaseEvent(QMouseEvent *event);

    virtual void resizeEvent(QResizeEvent *event);

    virtual void moveEvent(QMoveEvent *event);

private:
    QString m_delimiter;
    QStringList m_vocab;
    QFrame *m_container;
    QListWidget *m_listWidget;
    QString m_head;
    QString m_current;
    QString m_tail;

    void initContainer();
    void updateContainerGeometry();
    void updateTextParts();

private slots:
    void onTargetTextEdited(QString);
    void onListItemSelected();
};

/// @}

} // namespace FE

/// @}

} // namespace Lvk

#endif // LVK_FE_AUTOCOMPLETETEXTEDIT_H
