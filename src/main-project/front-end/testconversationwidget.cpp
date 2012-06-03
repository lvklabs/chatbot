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

#include "testconversationwidget.h"

#include <QScrollBar>

//--------------------------------------------------------------------------------------------------
// TestConversationWidget
//--------------------------------------------------------------------------------------------------

Lvk::FE::TestConversationWidget::TestConversationWidget(QWidget *parent) :
    QTextEdit(parent)
{
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::TestConversationWidget::appendConversation(const QString &userInput,
                                                         const QString &botOuput_, bool match)
{
    QString botOuput = botOuput_;

    static const QString START_USER_SPAN = "<span style=\" color:#000088;\">";
    static const QString START_CHATBOT_SPAN = "<span style=\" color:#008800;\">";
    //static const QString START_NOT_MATCH_SPAN = "<span style=\" color:#880000;\">";
    static const QString END_SPAN = "</span>";
    static const QString BR = "<br/>";
    static const QString END_P_BODY_HTML = "</p></body></html>";

    if (!match) {
        //botOuput = START_NOT_MATCH_SPAN + botOuput + END_SPAN;
    }

    QString conversation = toHtml();

    conversation.remove(END_P_BODY_HTML);
    conversation += START_USER_SPAN + tr("Me:") + " " + END_SPAN + userInput + BR;

    if (!botOuput_.isEmpty()) {
        conversation += START_CHATBOT_SPAN + tr("Chatbot:") + " " + END_SPAN + botOuput + BR;
    }

    conversation += END_P_BODY_HTML;

    setHtml(conversation);

    verticalScrollBar()->setValue(verticalScrollBar()->maximum());
}

