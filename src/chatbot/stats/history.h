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

#ifndef LVK_STATS_HISTORY_H
#define LVK_STATS_HISTORY_H

#include <QList>
#include <QPair>
#include <QDate>
#include <QVariant>

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace Stats
{

/// \ingroup Lvk
/// \addtogroup Stats
/// @{

/**
 * \brief The History class provides the history of values of a statistic
 */
class History : public QList< QPair<QDate, QVariant> >
{
public:

    /**
     * Constructs an empty History object
     */
    History() { }

    /**
     * Constructs and appends a pair (date, value)
     */
    void append(const QDate &date, const QVariant &value)
    {
        QList< QPair<QDate, QVariant> >::append(QPair<QDate, QVariant>(date, value));
    }
};

/// @}

} // namespace Stats

/// @}

} // namespace Lvk


#endif // LVK_STATS_HISTORY_H

