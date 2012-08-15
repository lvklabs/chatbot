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

#ifndef LVK_STATS_STATSFILE_H
#define LVK_STATS_STATSFILE_H

#include "stats/id.h"
#include "stats/history.h"

class QString;

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
 * \brief The StatsFile class provides the interface for all files that store statistics.
 *
 * The StatsFile defines an abstract method for each statistic we are interested in. Method names
 * starting with \a set store absolut values, whereas those starting with \a add store cumulative
 * values.
 */
class StatsFile
{
public:

    /**
     * Destroys the object.
     */
    virtual ~StatsFile() { }

    /**
     * Sets \a value as the value for the statistic with the given \a id in the current date
     */
    virtual void setStat(Id id, const QVariant &value) = 0;

    /**
     * Returns the value of the given statistic \a id for current date
     */
    virtual void stat(Id id, QVariant &value) = 0;

    /**
     * Returns the history of values of the given statistic \a id
     */
    virtual void history(Id id, History &h) = 0;

    /**
     * Returns a the history of a new statistic that is the conbination of \a id1 and \a id2
     */
    virtual void combinedHistory(Id id1, Id id2, History &h) = 0;

    /**
     * Loads statistics from \a filename
     */
    virtual void load(const QString &filename) = 0;

    /**
     * Save statistics to the current file.
     */
    virtual void save() = 0;

    /**
     * Closes the current statistics file.
     */
    virtual void close() = 0;
};

/// @}

} // namespace Stats

/// @}

} // namespace Lvk


#endif // LVK_STATS_STATSFILE_H

