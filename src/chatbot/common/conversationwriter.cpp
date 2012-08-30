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

#include "common/conversationwriter.h"
#include "common/csvrow.h"
#include "common/csvdocument.h"
#include "common/globalstrings.h"

#include <QIODevice>
#include <QFile>
#include <QtDebug>

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

void makeCsvRow(Lvk::Cmn::CsvRow & row, const Lvk::Cmn::Conversation::Entry & entry)
{
    row.clear();
    row.append(entry.dateTime.toString(STR_CSV_CONV_DATE_TIME_FORMAT));
    row.append(sanitize(entry.from));
    row.append(sanitize(entry.to));
    row.append(sanitize(entry.msg));
    row.append(sanitize(entry.response));
    row.append(entry.match ? STR_CSV_CONV_ENTRY_MATCH : STR_CSV_CONV_ENTRY_NO_MATCH);
    row.append(QString::number(entry.ruleId));
}

//--------------------------------------------------------------------------------------------------

void makeCsvDoc(Lvk::Cmn::CsvDocument & doc, const Lvk::Cmn::Conversation & conv)
{
    Lvk::Cmn::CsvRow row;

    doc.clear();

    foreach (const Lvk::Cmn::Conversation::Entry &entry, conv.entries()) {
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

Lvk::Cmn::ConversationWriter::ConversationWriter()
    : m_device(0), m_init(false)
{
}

//--------------------------------------------------------------------------------------------------

Lvk::Cmn::ConversationWriter::ConversationWriter(const QString &filename)
    : m_device(new QFile(filename)), m_init(false)
{
}

//--------------------------------------------------------------------------------------------------

Lvk::Cmn::ConversationWriter::ConversationWriter(QIODevice *device)
    : m_device(device), m_init(false)
{
}

//--------------------------------------------------------------------------------------------------

Lvk::Cmn::ConversationWriter::~ConversationWriter()
{
    delete m_device;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::Cmn::ConversationWriter::write(const Lvk::Cmn::Conversation &conv)
{
    if (!init()) {
        return false;
    }

    Cmn::CsvDocument doc;
    makeCsvDoc(doc, conv);

    return writeln(doc.toString().toUtf8()) && flush();
}

//--------------------------------------------------------------------------------------------------

bool Lvk::Cmn::ConversationWriter::write(const Conversation::Entry &entry)
{
    if (!init()) {
        return false;
    }

    Cmn::CsvRow row;
    makeCsvRow(row, entry);

    return writeln(row.toString().toUtf8()) && flush();
}

//--------------------------------------------------------------------------------------------------

bool Lvk::Cmn::ConversationWriter::atEnd()
{
    return !m_device || m_device->atEnd();
}

//--------------------------------------------------------------------------------------------------

inline bool Lvk::Cmn::ConversationWriter::init()
{
    if (!m_init) {
        if (m_device) {
            if (m_device->isOpen() || m_device->open(QIODevice::Append)) {
                m_init = true;
            } else {
                qCritical() << "ConversationWriter: Cannot open IO device with append permissions";
            }
        }
    }

    return m_init;
}

//--------------------------------------------------------------------------------------------------

inline bool Lvk::Cmn::ConversationWriter::writeln(const QByteArray &data)
{
    if (data.isEmpty()) {
        return true;
    } else {
        return m_device->write(data) != -1 && m_device->write("\n") != -1;
    }
}

//--------------------------------------------------------------------------------------------------

inline bool Lvk::Cmn::ConversationWriter::flush()
{
    // flush() is not part of QIODevice interface. We only flush if it's type QFile.

    QFile *file = dynamic_cast<QFile *>(m_device);

    return file ? file->flush() : true;
}


