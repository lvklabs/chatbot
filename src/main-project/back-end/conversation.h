#ifndef LVK_BE_CONVERSATION_H
#define LVK_BE_CONVERSATION_H

#include <QDateTime>
#include <QString>
#include <QList>

namespace Lvk
{

namespace BE

{

/**
 * \brief The Conversation Class provides a conversation held between a user and the chatbot
 */

class Conversation
{
public:
    Conversation();
    Conversation(const QString &str);

    /**
     * \brief The Entry Class provides a single conversation entry between a user and the chatbot
     */

    class Entry
    {
    public:
        Entry();
        Entry(const QString &str);
        Entry(const QDateTime &dateTime, const QString &from, const QString &to, const QString &msg,
              const QString &response, bool match);

        QDateTime dateTime;
        QString from;
        QString to;
        QString msg;
        QString response;
        bool match;

        bool isNull();

        QString toString() const;
    };

    void setEntries(const QList<Entry> &entries);
    QList<Entry> &entries();
    const QList<Entry> &entries() const;

    void append(const Entry &entry);

    QString toString() const;

private:
    QList<Entry> m_entries;
};

} // namespace BE

} // namespace Lvk


#endif // LVK_BE_CONVERSATION_H
