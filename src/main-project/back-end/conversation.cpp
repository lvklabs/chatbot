#include "conversation.h"

#include <QStringList>
#include <QRegExp>

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
            .replace("\t", " ")
            .replace("+-->", "+ -->")
            .replace("!-->", "! -->");
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

//--------------------------------------------------------------------------------------------------

Lvk::BE::Conversation::Entry::Entry(const QDateTime &dateTime, const QString &from,
                                    const QString &to, const QString &msg,
                                    const QString &response, bool match)
    : dateTime(dateTime), from(from), to(to), msg(msg), response(response), match(match)
{
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::Conversation::Entry::Entry(const QString &str)
    : match(false)
{
    QRegExp regex("(\\d\\d-\\d\\d-\\d\\d \\d\\d:\\d\\d:\\d\\d)"
                  " (.*) -> (.*): (.*) ([!+])--> (.*)");

    if (regex.exactMatch(str)) {
        dateTime = QDateTime::fromString(regex.cap(1), DATE_TIME_LOG_FORMAT);
        from     = regex.cap(2);
        to       = regex.cap(3);
        msg      = regex.cap(4);
        match    = regex.cap(5) == "+" ? true : false;
        response = regex.cap(6);
    }
}

//--------------------------------------------------------------------------------------------------

QString Lvk::BE::Conversation::Entry::toString() const
{
    return QString("%1 %2 -> %3: %4 %5--> %6")
            .arg(QDateTime::currentDateTime().toString(DATE_TIME_LOG_FORMAT))
            .arg(sanitize(from))
            .arg(sanitize(to))
            .arg(sanitize(msg))
            .arg(match ? "+" : "!")
            .arg(sanitize(response));
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::Conversation::Entry::isNull()
{
    return dateTime.isNull()
            && from.isNull()
            && to.isNull()
            && msg.isNull()
            && response.isNull()
            && !match;
}

//--------------------------------------------------------------------------------------------------
// Conversation
//--------------------------------------------------------------------------------------------------

Lvk::BE::Conversation::Conversation()
{
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::Conversation::Conversation(const QString &str)
{
    QStringList lines = str.split("\n", QString::SkipEmptyParts);

    for (int i = 0; i < lines.size(); ++i) {
        Conversation::Entry entry(lines[i]);
        if (!entry.isNull()) {
            m_entries.append(entry);
        }
    }
}

//--------------------------------------------------------------------------------------------------

QString Lvk::BE::Conversation::toString() const
{
    QString str;

    for (int i = 0; i < m_entries.size(); ++i) {
        str += m_entries[i].toString() + "\n";
    }

    return str;
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::Conversation::setEntries(const QList<Lvk::BE::Conversation::Entry> &entries)
{
    m_entries = entries;
}

//--------------------------------------------------------------------------------------------------

QList<Lvk::BE::Conversation::Entry> & Lvk::BE::Conversation::entries()
{
    return m_entries;
}

//--------------------------------------------------------------------------------------------------

const QList<Lvk::BE::Conversation::Entry> & Lvk::BE::Conversation::entries() const
{
    return m_entries;
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::Conversation::append(const Lvk::BE::Conversation::Entry &entry)
{
    m_entries.append(entry);
}


