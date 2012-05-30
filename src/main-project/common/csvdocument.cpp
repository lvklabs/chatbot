/*
 * Copyright (C) 2012 Andres Pagliano, Gabriel Miretti, Gonzalo Buteler,
 * Nestor Bustamante, Pablo Perez de Angelis
 *
 * This file is part of LVK Botmaster.
 *
 * LVK Botmaster is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * LVK Botmaster is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LVK Botmaster.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "csvdocument.h"

#include <QString>
#include <QStringList>

#define EOL   "\n"


//--------------------------------------------------------------------------------------------------
// Helpers
//--------------------------------------------------------------------------------------------------

namespace
{

void sanitize(QString &s)
{
    s.replace(EOL, " ");
}

} // namespace


//--------------------------------------------------------------------------------------------------
// CsvDocument
//--------------------------------------------------------------------------------------------------

Lvk::Common::CsvDocument::CsvDocument()
{
}

//--------------------------------------------------------------------------------------------------

Lvk::Common::CsvDocument::CsvDocument(const QString &csvStr)
{
    fromString(csvStr);
}

//--------------------------------------------------------------------------------------------------

const QList<Lvk::Common::CsvRow> & Lvk::Common::CsvDocument::rows() const
{
    return m_rows;
}

//--------------------------------------------------------------------------------------------------

Lvk::Common::CsvRow & Lvk::Common::CsvDocument::operator[](int n)
{
    return m_rows[n];
}

//--------------------------------------------------------------------------------------------------

void Lvk::Common::CsvDocument::append(const CsvRow &row)
{
    m_rows.append(row);
}

//--------------------------------------------------------------------------------------------------

void Lvk::Common::CsvDocument::append(const QString &rowStr)
{
    m_rows.append(CsvRow(rowStr));
}

//--------------------------------------------------------------------------------------------------

void Lvk::Common::CsvDocument::fromString(const QString &csvStr)
{
    clear();

    if (csvStr.isEmpty()) {
        return;
    }

    QStringList rows = csvStr.split(EOL);

    foreach (const QString &row, rows) {
        append(row);
    }
}

//--------------------------------------------------------------------------------------------------

QString Lvk::Common::CsvDocument::toString() const
{
    QString str;

    foreach (const CsvRow &row, m_rows) {
        if (!str.isEmpty()) {
            str += EOL;
        }

        QString rowStr = row.toString();
        sanitize(rowStr);
        str += rowStr;
    }

    return str;
}

//--------------------------------------------------------------------------------------------------

void Lvk::Common::CsvDocument::clear()
{
    m_rows.clear();
}

//--------------------------------------------------------------------------------------------------

bool Lvk::Common::CsvDocument::operator==(const CsvDocument &other) const
{
    return m_rows == other.m_rows;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::Common::CsvDocument::operator!=(const CsvDocument &other) const
{
    return !this->operator==(other);
}

