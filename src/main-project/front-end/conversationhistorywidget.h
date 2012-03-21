#ifndef CONVERSATIONHISTORYWIDGET_H
#define CONVERSATIONHISTORYWIDGET_H

#include <QSplitter>
#include <QHash>
#include <QList>

#include "conversation.h"

class QTableWidget;
class QModelIndex;

/**
 * \brief The ConversationHistoryWidget provides a widget to display conversation history beetwen
 *        the chatbot and chat users
 */

class ConversationHistoryWidget : public QSplitter
{
    Q_OBJECT
public:
    explicit ConversationHistoryWidget(QWidget *parent = 0);
    explicit ConversationHistoryWidget(const Lvk::BE::Conversation &conv, QWidget *parent = 0);

    void clear();

    void setConversation(const Lvk::BE::Conversation &conv);

    void addConversationEntry(const Lvk::BE::Conversation::Entry &entry);

private:
    QTableWidget *m_dateContactTable;
    QTableWidget *m_conversationTable;

    typedef QList<Lvk::BE::Conversation::Entry> EntryList;
    QHash<QString, EntryList> m_entries;

    void setupWidget();

    void addConversationTableRow(const Lvk::BE::Conversation::Entry &entry);
    void addDateContactTableRow(const Lvk::BE::Conversation::Entry &entry);

private slots:
    void currentRowChanged(const QModelIndex &current, const QModelIndex &previous);
};

#endif // CONVERSATIONHISTORYWIDGET_H
