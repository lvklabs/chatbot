#include "conversationhistorywidget.h"

#include <QSplitter>
#include <QTableWidget>
#include <QHeaderView>

enum DateContactTableColumns
{
    DateColumnn,
    UsernameColumn,
    DateContactTableTotalColumns
};

enum ConversationTableColumns
{
    TimeColumnn,
    MessageColumn,
    ResponseColumn,
    StatusColumn,
    ConversationTableTotalColumns
};


ConversationHistoryWidget::ConversationHistoryWidget(QWidget *parent)
    : QSplitter(Qt::Horizontal, parent)
{
    m_dateContactTable = new QTableWidget(this);
    m_dateContactTable->setRowCount(0);
    m_dateContactTable->setColumnCount(DateContactTableTotalColumns);
    m_dateContactTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_dateContactTable->setAlternatingRowColors(true);
    m_dateContactTable->horizontalHeader()->setStretchLastSection(true);
    m_dateContactTable->verticalHeader()->hide();
    m_dateContactTable->setHorizontalHeaderLabels(QStringList()
                                                  << tr("Date")
                                                  << tr("Username"));

    m_conversationTable = new QTableWidget(this);
    m_conversationTable->setRowCount(0);
    m_conversationTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_conversationTable->setColumnCount(ConversationTableTotalColumns);
    m_conversationTable->setAlternatingRowColors(true);
    m_conversationTable->horizontalHeader()->setStretchLastSection(true);
    m_conversationTable->verticalHeader()->hide();
    m_conversationTable->setHorizontalHeaderLabels(QStringList()
                                                   << tr("Time")
                                                   << tr("Message")
                                                   << tr("Response")
                                                   << tr("Status"));

    setSizes(QList<int>() << 50 << 150);
}

void ConversationHistoryWidget::addConversatioEntry(const Lvk::BE::Conversation::Entry &entry)
{
    int nextRow = 0;

    nextRow = m_dateContactTable->rowCount();
    m_dateContactTable->insertRow(nextRow);
    m_dateContactTable->setItem(nextRow, DateColumnn,    new QTableWidgetItem(entry.dateTime.toString("dd/MM/yy")));
    m_dateContactTable->setItem(nextRow, UsernameColumn, new QTableWidgetItem(entry.from));

    nextRow = m_conversationTable->rowCount();
    m_conversationTable->insertRow(nextRow);
    m_conversationTable->setItem(nextRow, TimeColumnn,    new QTableWidgetItem(entry.dateTime.toString("hh:mm::ss")));
    m_conversationTable->setItem(nextRow, MessageColumn,  new QTableWidgetItem(entry.msg));
    m_conversationTable->setItem(nextRow, ResponseColumn, new QTableWidgetItem(entry.responseMsg));
    m_conversationTable->setItem(nextRow, StatusColumn,   new QTableWidgetItem(entry.match ? "Match" : "No Match"));
}
