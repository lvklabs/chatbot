#ifndef TESTCONVERSATIONWIDGET_H
#define TESTCONVERSATIONWIDGET_H

#include <QTextEdit>
#include <QString>

/**
 * Custom widget to display test local conversations with the bot
 */

class TestConversationWidget : public QTextEdit
{
    Q_OBJECT

public:
    explicit TestConversationWidget(QWidget *parent = 0);

    void appendConversation(const QString &userInput, const QString &botOuput, bool match);

};

#endif // TESTCONVERSATIONWIDGET_H
