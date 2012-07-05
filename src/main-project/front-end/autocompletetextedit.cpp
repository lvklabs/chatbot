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

#include "autocompletetextedit.h"

#include <QListWidget>
#include <QListWidgetItem>
#include <QKeyEvent>
#include <QStringList>
#include <QToolTip>
#include <QVBoxLayout>
#include <QDebug>

//--------------------------------------------------------------------------------------------------
// Helpers
//--------------------------------------------------------------------------------------------------

namespace
{

int find(const QString &token, const QString &text, int from)
{
    int tkSize = token.size();
    int txtSize = text.size();

    for (int i = from; i + (tkSize - 1) < txtSize; ++i) {
        for (int j = 0; j < tkSize; ++j) {
            if (token[j] != text[i + j]) {
                break;
            }
            if (j == tkSize - 1) {
                return i;
            }
        }
    }
    return -1;
}

//--------------------------------------------------------------------------------------------------

int rfind(const QString &token, const QString &text, int from)
{
    int tkSize = token.size();
    int txtSize = text.size();

    for (int i = from; i >= 0; --i) {
        if (i + (tkSize - 1) < txtSize) {
            for (int j = 0; j < tkSize; ++j) {
                if (token[j] != text[i + j]) {
                    break;
                }
                if (j == tkSize - 1) {
                    return i;
                }
            }
        }
    }
    return -1;
}

}  // namespace


//--------------------------------------------------------------------------------------------------
// AutoCompleteTextEdit
//--------------------------------------------------------------------------------------------------

Lvk::FE::AutocompleteTextEdit::AutocompleteTextEdit(QWidget *parent) :
    QLineEdit(parent),
    m_delimiter(" "),
    m_container(new QFrame(this->window())),
    m_listWidget(new QListWidget())
{
    m_container->setWindowFlags(m_container->windowFlags() | Qt::WindowStaysOnTopHint);
    m_container->setVisible(false);
    //m_container->setFrameShape(QFrame::StyledPanel);
    m_container->setLayout(new QVBoxLayout());
    m_container->setGeometry(QRect());

    m_container->layout()->addWidget(m_listWidget);
    m_container->layout()->setMargin(0);
    m_listWidget->setFocusPolicy(Qt::NoFocus);

    setText(m_defaultString);

    connect(this,         SIGNAL(textEdited(QString)),  SLOT(onTargetTextEdited(QString)));
    connect(m_listWidget, SIGNAL(clicked(QModelIndex)), SLOT(onListItemSelected()));
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::AutocompleteTextEdit::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();

    if (m_container->isVisible()) {
        if (key == Qt::Key_Down && m_listWidget->currentRow() + 1 < m_listWidget->count()) {
            m_listWidget->setCurrentRow(m_listWidget->currentRow() + 1);
        }
        if (key == Qt::Key_Up && m_listWidget->currentRow() > 0) {
            m_listWidget->setCurrentRow(m_listWidget->currentRow() - 1);
        }
        if ((key == Qt::Key_Enter || key == Qt::Key_Return) && m_listWidget->currentRow() != -1) {
            onListItemSelected();
        }
    }

    if (key == Qt::Key_Escape) {
        m_container->setVisible(!m_container->isVisible());
    }

    QLineEdit::keyPressEvent(event);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::AutocompleteTextEdit::focusOutEvent(QFocusEvent *event)
{
    if (text().trimmed().isEmpty()) {
        QLineEdit::setText(m_defaultString);
    }

    m_container->hide();

    QLineEdit::focusOutEvent(event);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::AutocompleteTextEdit::focusInEvent(QFocusEvent *event)
{
    updateContainerGeometry();

    if (text().trimmed() == m_defaultString) {
        QLineEdit::setText("");
    }

    // If empty, display full list
    if (text().isEmpty()) {
        onTargetTextEdited("");
    }

    QLineEdit::focusInEvent(event);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::AutocompleteTextEdit::mouseReleaseEvent(QMouseEvent *)
{
    // If empty, display full list
    if (text().isEmpty()) {
        onTargetTextEdited("");
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::AutocompleteTextEdit::resizeEvent(QResizeEvent *event)
{
    updateContainerGeometry();

    QLineEdit::resizeEvent(event);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::AutocompleteTextEdit::moveEvent(QMoveEvent *event)
{
    updateContainerGeometry();

    QLineEdit::moveEvent(event);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::AutocompleteTextEdit::setStringList(const QStringList &strList)
{
    m_strList = strList;
}

//--------------------------------------------------------------------------------------------------

const QStringList & Lvk::FE::AutocompleteTextEdit::stringList()
{
    return m_strList;
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::AutocompleteTextEdit::setDelimiter(const QString &delim)
{
    m_delimiter = delim;
}

//--------------------------------------------------------------------------------------------------

const QString & Lvk::FE::AutocompleteTextEdit::delimiter()
{
    return m_delimiter;
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::AutocompleteTextEdit::setDefaultText(const QString &text)
{
    m_defaultString = text;
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::AutocompleteTextEdit::setText(const QString &text)
{
    if (!text.isEmpty()) {
        QLineEdit::setText(text);
    } else {
        QLineEdit::setText(m_defaultString);
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::AutocompleteTextEdit::onTargetTextEdited(QString)
{
    updateTextParts();

    m_listWidget->clear();

    if (m_strList.isEmpty()) {
        // FIXME Add new class methods to set the tooltip
        QToolTip::showText(mapToGlobal(QPoint(0, height()/2)),
                           tr("Tip: Go to the 'Connect to chat' tab and connect using your\n"
                              "Facebook or Gmail account to get a list of contacts"), this);
    } else {
        QString current = m_current.trimmed();

        //if (current.size() > 0) {
            foreach (const QString &str, m_strList) {
                if (str.contains(current, false)) {
                    m_listWidget->addItem(str);
                }
                if (m_listWidget->count() == 1) {
                    m_listWidget->setCurrentRow(0);
                }
            }
        //}


        if (m_listWidget->count() > 0) {
            m_container->show();
        } else {
            m_container->hide();
        }
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::AutocompleteTextEdit::onListItemSelected()
{
    if (m_listWidget->currentRow() == -1) {
        return;
    }

    QString t1; // text before cursor position
    QString t2; // text after cursor position

    if (m_tail.isEmpty()) {
        t1 = m_head + m_listWidget->currentItem()->text() + m_delimiter;
        //t2 = "";
    } else {
        t1 = m_head + m_listWidget->currentItem()->text();
        t2 = m_tail;
    }
    setText(t1 + t2);
    setCursorPosition(t1.size());

    emit textEdited(text());

    m_container->hide();
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::AutocompleteTextEdit::updateContainerGeometry()
{
    QPoint p = mapTo(m_container->parentWidget(), pos());
    m_container->setGeometry(p.x(), p.y() + 3, std::min(300, width()), 200);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::AutocompleteTextEdit::updateTextParts()
{
    // Update head, current and tail parts. Example:
    //   splitToken = " "
    //   text       = "word1 word2 word3 word4"
    //                               |_____________ CursorPos
    // It returns:
    //   head    = "word1 word2 "
    //   current = "word3"
    //   tail    = " word4"

    int cursorPos = cursorPosition();
    int delimSize = m_delimiter.size();
    QString text = this->text();

    int prevDelimPos = ::rfind(m_delimiter, text, cursorPos - 1);
    int nextDelimPos = ::find(m_delimiter, text, cursorPos);

    if (nextDelimPos == -1) {
        nextDelimPos = text.size();
    }

    m_head    = text.mid(0, prevDelimPos+1);
    m_current = text.mid(prevDelimPos+delimSize, nextDelimPos - (prevDelimPos+delimSize));
    m_tail    = text.mid(nextDelimPos);

	//    std::cout << "[" << m_head.toStdString()    << "] "
	//              << "[" << m_current.toStdString() << "] "
	//              << "[" << m_tail.toStdString()    << "]"
    //              << std::endl;
}




