#ifndef CONVERSATIONHISTORYWIDGET_H
#define CONVERSATIONHISTORYWIDGET_H

#include <QSplitter>

#include "conversation.h"

class QTableWidget;

/**
 * \brief The ConversationHistoryWidget provides a widget to display conversation history beetwen
 *        the chatbot and chat users
 */

class ConversationHistoryWidget : public QSplitter
{
    Q_OBJECT
public:
    explicit ConversationHistoryWidget(QWidget *parent = 0);

    void addConversatioEntry(const Lvk::BE::Conversation::Entry &entry);

private:
    QTableWidget *m_dateContactTable;
    QTableWidget *m_conversationTable;
};

#endif // CONVERSATIONHISTORYWIDGET_H
