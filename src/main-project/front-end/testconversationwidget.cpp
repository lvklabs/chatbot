#include "testconversationwidget.h"

#include <QScrollBar>


TestConversationWidget::TestConversationWidget(QWidget *parent) :
    QTextEdit(parent)
{
}

void TestConversationWidget::appendConversation(const QString &userInput, const QString &botOuput_,
                                                bool match)
{
    QString botOuput = botOuput_;

    static const QString START_USER_SPAN = "<span style=\" color:#000088;\">";
    static const QString START_CHATBOT_SPAN = "<span style=\" color:#008800;\">";
    static const QString START_NOT_MATCH_SPAN = "<span style=\" color:#880000;\">";
    static const QString END_SPAN = "</span>";
    static const QString BR = "<br/>";
    static const QString END_P_BODY_HTML = "</p></body></html>";

    if (!match) {
        botOuput = START_NOT_MATCH_SPAN + botOuput + END_SPAN;
    }

    QString conversation = toHtml();

    conversation.remove(END_P_BODY_HTML);
    conversation += START_USER_SPAN + tr("You:") + " " + END_SPAN + userInput + BR;
    conversation += START_CHATBOT_SPAN + tr("Chatbot:") + " " + END_SPAN + botOuput + BR;
    conversation += END_P_BODY_HTML;

    setHtml(conversation);

    verticalScrollBar()->setValue(verticalScrollBar()->maximum());
}
