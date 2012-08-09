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

#ifndef LVK_CMN_CSVROW_H
#define LVK_CMN_CSVROW_H

#include <QList>
#include <QString>
#include <QStringList>


namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace Cmn
{

/// \ingroup Lvk
/// \addtogroup Cmn
/// @{

/**
 * \brief The CsvRow class represents a row in a CSV document.
 *
 * The CsvRow class represents a row in a CSV document.
 *
 * \see CsvDocument class
 */

class CsvRow
{
public:

    /**
     * Constructs an empty CSV row
     */
    CsvRow();

    /**
     * Constructs a CSV row with the given \a size
     */
    CsvRow(unsigned size);

    /**
     * Constructs a CSV row with the given \a cells
     */
    CsvRow(const QStringList &cells);

    /**
     * Constructs a CSV row parsing string \a str
     */
    CsvRow(const QString &str);

    /**
     * Returns the list of cells in the row
     */
    const QList<QString> & cells() const;

    /**
     * Appends a new cell to the row
     */
    void append(const QString &cell);

    /**
     * Returns the amount of cells in the row
     */
    int size() const;

    /**
     * Returns a const reference to the cell at position \a n
     */
    const QString &operator[](int n) const;

    /**
     * Returns a reference to the cell at position \a n
     */
    QString &operator[](int n);

    /**
     * Removes all cells
     */
    void clear();

    /**
     * Returns true if this row is equal to \a other. Otherwise; false.
     */
    bool operator==(const CsvRow &other) const;

    /**
     * Returns true if this row is not equal to \a other. Otherwise; false.
     */
    bool operator!=(const CsvRow &other) const;

    /**
     * Returns the string representation of the row.
     */
    QString toString() const;

private:
    QList<QString> m_cells;

    void parseString(const QString &str);
};

/// @}

} // namespace Cmn

/// @}

} // namespace Lvk

#endif // LVK_CMN_CSVROW_H
