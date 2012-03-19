#include "conversation.h"

//--------------------------------------------------------------------------------------------------
// Helpers
//--------------------------------------------------------------------------------------------------

namespace
{

QString sanitize(const QString &str)
{
    return QString(str)
            .replace("\n", " ")
            .replace("\r", " ")
            .replace("\t", " ");
}

} //namespace


//--------------------------------------------------------------------------------------------------
// Conversation::Entry
//--------------------------------------------------------------------------------------------------

#define DATE_TIME_LOG_FORMAT    "dd-MM-yy hh:mm:ss"

Lvk::BE::Conversation::Entry::Entry()
    : match(false)
{
}

Lvk::BE::Conversation::Entry::Entry(const QDateTime &dateTime, const QString &from,
                                    const QString &to, const QString &msg,
                                    const QString &response, bool match)
    : dateTime(dateTime), from(from), to(to), msg(msg), responseMsg(response), match(match)
{
}

Lvk::BE::Conversation::Entry::Entry(const QString &/*str*/)
{
    // TODO
}

QString Lvk::BE::Conversation::Entry::toString() const
{
    return QString("%1 %2 -> %3: %4 %5--> %6")
            .arg(QDateTime::currentDateTime().toString(DATE_TIME_LOG_FORMAT))
            .arg(sanitize(from))
            .arg(sanitize(to))
            .arg(sanitize(msg))
            .arg(match ? "+" : "!")
            .arg(sanitize(responseMsg));
}

//--------------------------------------------------------------------------------------------------
// Conversation
//--------------------------------------------------------------------------------------------------

Lvk::BE::Conversation::Conversation()
{
}

Lvk::BE::Conversation::Conversation(const QString &/*str*/)
{
    // TODO
}

QString Lvk::BE::Conversation::toString() const
{
    QString str;

    for (int i = 0; i < m_entries.size(); ++i) {
        str += m_entries[i].toString() + "\n";
    }

    return str;
}

void Lvk::BE::Conversation::setEntries(const QList<Lvk::BE::Conversation::Entry> &entries)
{
    m_entries = entries;
}

QList<Lvk::BE::Conversation::Entry> & Lvk::BE::Conversation::entries()
{
    return m_entries;
}

const QList<Lvk::BE::Conversation::Entry> & Lvk::BE::Conversation::entries() const
{
    return m_entries;
}


