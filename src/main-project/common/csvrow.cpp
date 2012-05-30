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

#include "csvrow.h"

#include <QStringList>

#define QUOTE   "\""
#define COMMA   ","

//--------------------------------------------------------------------------------------------------
// Helpers
//--------------------------------------------------------------------------------------------------

namespace
{

//--------------------------------------------------------------------------------------------------

void escape(QString &s)
{
    if (s.contains(COMMA)) {
        if (s.contains(QUOTE)) {
            s.replace(QUOTE, QUOTE QUOTE);
        }
        s = QUOTE + s + QUOTE;
    }
}

//--------------------------------------------------------------------------------------------------

void unescape(QString &s)
{
    if (s.size() >= 2 && s.startsWith(QUOTE) && s.endsWith(QUOTE)) {
        s = s.mid(1, s.size() - 2);

        s.replace(QUOTE QUOTE, QUOTE);
    }
}

//--------------------------------------------------------------------------------------------------

bool hasStartQuote(QString &s)
{
    return s.startsWith(QUOTE);
}

//--------------------------------------------------------------------------------------------------

bool hasEndQuote(QString &s)
{
    return s.endsWith(QUOTE) && (!s.endsWith(QUOTE QUOTE) || s.size() == 2);
}

//--------------------------------------------------------------------------------------------------

bool isAllSpace(const QString &s)
{
    for (int i = 0; i < s.size(); ++i) {
        if (!s[i].isSpace()) {
            return false;
        }
    }
    return true;
}

} // namespace


//--------------------------------------------------------------------------------------------------
// CsvRow
//--------------------------------------------------------------------------------------------------

Lvk::Common::CsvRow::CsvRow()
{
}

//--------------------------------------------------------------------------------------------------

Lvk::Common::CsvRow::CsvRow(const QStringList &cells)
{
    foreach (const QString &cell, cells) {
        append(cell);
    }
}

//--------------------------------------------------------------------------------------------------

Lvk::Common::CsvRow::CsvRow(const QString &str)
{
    fromString(str);
}

//--------------------------------------------------------------------------------------------------

void Lvk::Common::CsvRow::fromString(const QString &str)
{
    clear();

    if (isAllSpace(str)) {
        return;
    }

    QStringList cells = str.split(COMMA);

    for (int i = 0; i < cells.size(); ++i) {
        QString &cell = cells[i];

        // Check if we splitted up a scaped comma
        if (hasStartQuote(cell) && !hasEndQuote(cell)) {
            // Join cells until we find the end quote
            while (i+1 < cells.size() && !hasEndQuote(cell)) {
                cell += COMMA;
                cell += cells[++i];
            }
        }

        unescape(cell);
        append(cell);
    }
}

//--------------------------------------------------------------------------------------------------

QString Lvk::Common::CsvRow::toString() const
{
    QString str;

    for (int i = 0; i < m_cells.size(); ++i) {
        if (!str.isEmpty()) {
            str += COMMA;
        }
        QString cell = m_cells[i];

        escape(cell);
        str += cell;
    }

    return str;
}

//--------------------------------------------------------------------------------------------------

const QList<QString> & Lvk::Common::CsvRow::cells() const
{
    return m_cells;
}

//--------------------------------------------------------------------------------------------------

void Lvk::Common::CsvRow::append(const QString &cell)
{
    if (!cell.contains('\n')) {
        m_cells.append(cell);
    } else {
        m_cells.append(QString(cell).remove('\n'));
    }
}

//--------------------------------------------------------------------------------------------------

QString & Lvk::Common::CsvRow::operator[](int n)
{
    return m_cells[n];
}

//--------------------------------------------------------------------------------------------------

void Lvk::Common::CsvRow::clear()
{
    m_cells.clear();
}

//--------------------------------------------------------------------------------------------------

bool Lvk::Common::CsvRow::operator==(const Lvk::Common::CsvRow &other) const
{
    return m_cells == other.m_cells;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::Common::CsvRow::operator!=(const Lvk::Common::CsvRow &other) const
{
    return !this->operator ==(other);
}

