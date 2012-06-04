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

#include "conversationwriter.h"
#include "csvrow.h"
#include "csvdocument.h"
#include "globalstrings.h"

#include <QIODevice>
#include <QFile>

//--------------------------------------------------------------------------------------------------
// Helpers
//--------------------------------------------------------------------------------------------------

namespace
{

//--------------------------------------------------------------------------------------------------

inline QString sanitize(const QString &str)
{
    return str.simplified();
}

//--------------------------------------------------------------------------------------------------

void makeCsvRow(Lvk::Cmn::CsvRow & row, const Lvk::BE::Conversation::Entry & entry)
{
    row.clear();
    row.append(entry.dateTime.toString(STR_CSV_CONV_DATE_TIME_FORMAT));
    row.append(sanitize(entry.from));
    row.append(sanitize(entry.to));
    row.append(sanitize(entry.msg));
    row.append(sanitize(entry.response));
    row.append(entry.match ? STR_CSV_CONV_ENTRY_MATCH : STR_CSV_CONV_ENTRY_NO_MATCH);
}

//--------------------------------------------------------------------------------------------------

void makeCsvDoc(Lvk::Cmn::CsvDocument & doc, const Lvk::BE::Conversation & conv)
{
    Lvk::Cmn::CsvRow row;

    doc.clear();

    foreach (const Lvk::BE::Conversation::Entry &entry, conv.entries()) {
        if (!entry.isNull()) {
            makeCsvRow(row, entry);
            doc.append(row);
        }
    }
}

} // namespace


//--------------------------------------------------------------------------------------------------
// ConversationWriter
//--------------------------------------------------------------------------------------------------

Lvk::BE::ConversationWriter::ConversationWriter()
    : m_device(0)
{
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::ConversationWriter::ConversationWriter(const QString &filename)
    : m_device(new QFile(filename))
{
    m_device->open(QIODevice::Append);
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::ConversationWriter::ConversationWriter(QIODevice *device)
    : m_device(device)
{
    if (!device->isOpen()) {
        device->open(QIODevice::Append);
    }
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::ConversationWriter::~ConversationWriter()
{
    delete m_device;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::ConversationWriter::write(const Lvk::BE::Conversation &conv)
{
    if (!m_device) {
        return false;
    }

    Cmn::CsvDocument doc;
    makeCsvDoc(doc, conv);

    return writeln(doc.toString().toUtf8()) && flush();
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::ConversationWriter::write(const Conversation::Entry &entry)
{
    if (!m_device) {
        return false;
    }

    Cmn::CsvRow row;
    makeCsvRow(row, entry);

    return writeln(row.toString().toUtf8()) && flush();
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::ConversationWriter::atEnd()
{
    return !m_device || m_device->atEnd();
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::ConversationWriter::writeln(const QByteArray &data)
{
    return m_device->write(data) != -1 && m_device->write("\n") != -1;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::ConversationWriter::flush()
{
    // flush() is not part of QIODevice interface. We only flush if it's type QFile.

    QFile *file = dynamic_cast<QFile *>(m_device);

    return file ? file->flush() : true;
}
