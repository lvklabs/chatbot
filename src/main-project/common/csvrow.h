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

#ifndef LVK_COMMON_CSVROW_H
#define LVK_COMMON_CSVROW_H

#include <QList>
#include <QString>


namespace Lvk
{

namespace Common
{

/**
 * \brief The CsvRow class represents a row in a CSV document.
 *
 * The CsvRow class represents a row in a CSV document.
 *
 * @see CsvDocument class
 */

class CsvRow
{
public:
    CsvRow();

    CsvRow(const QString &str);

    const QList<QString> & cells() const;

    void append(const QString &cell);

    QString &operator[](int n);

    void fromString(const QString &str);

    QString toString() const;

    void clear();

    bool operator==(const CsvRow &other) const;

    bool operator!=(const CsvRow &other) const;

private:
    QList<QString> m_cells;
};

} // namespace Common

} // namespace Lvk

#endif // LVK_COMMON_CSVROW_H
