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
    CsvDocument();

    CsvDocument(const QString &csvStr);

    const QList<CsvRow> & rows() const;

    const CsvRow &operator[](int n) const ;

    void append(const CsvRow &row);

    void append(const QString &rowStr);

    void clear();

    bool operator==(const CsvDocument &other) const;

    bool operator!=(const CsvDocument &other) const;

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
