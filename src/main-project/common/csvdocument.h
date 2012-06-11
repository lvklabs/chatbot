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

#ifndef LVK_CMN_CSVDOCUMENT_H
#define LVK_CMN_CSVDOCUMENT_H

#include "csvrow.h"

#include <QList>
#include <QString>

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
 * \brief The CsvDocument class represents a CSV document.
 *
 * The CsvDocument class represents a CSV document where rows are separated with the new line
 * '\n' and columns are separated with the comma character ','.
 *
 * The CsvDocument does not support rows containing the new line character. If a new line
 * charater is found it will be replaced with character ' '.
 */

class CsvDocument
{
public:

    /**
     * Constructs an emtpy CSV document
     */
    CsvDocument();

    /**
     * Constructs a CSV document parsing string \a csvStr
     */
    CsvDocument(const QString &csvStr);

    /**
     * Returns the list of rows of the document
     */
    const QList<CsvRow> & rows() const;

    /**
     * Returns the row at position \a n
     */
    const CsvRow &operator[](int n) const ;

    /**
     * Appends the given \a row to the document
     */
    void append(const CsvRow &row);

    /**
     * Converts the string \a rowStr to CsvRow and appends it to the document
     */
    void append(const QString &rowStr);


    /**
     * Clears the document by removing all rows.
     */
    void clear();

    /**
     * Returns true if this document is equal to \a other. Otherwise; false.
     */
    bool operator==(const CsvDocument &other) const;

    /**
     * Returns true if this document is not equal to \a other. Otherwise; false.
     */
    bool operator!=(const CsvDocument &other) const;

    /**
     * Returns the string representation of the CSV document.
     */
    QString toString() const;

private:
    QList<CsvRow> m_rows;

    void parseString(const QString &csvStr);
};

/// @}

} // namespace Cmn

/// @}

} // namespace Lvk


#endif // LVK_CMN_CSVDOCUMENT_H
