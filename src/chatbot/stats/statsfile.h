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
     * Sets \a size as the rules lexicon size, i.e. the amount of different words used to
     * define rules.
     */
    virtual void setLexiconSize(unsigned size) = 0;

    /**
     * Sets \a count as the total amount of words used to define rules.
     */
    virtual void setTotalWords(unsigned count) = 0;

    /**
     * Sets \a count as the total amount of rules.
     */
    virtual void setTotalRules(unsigned count) = 0;

    /**
     * Sets \a points as the total amount of rule points.
     */
    virtual void setTotalRulePoints(unsigned points) = 0;

    /**
     * Adds \a secs seconds to the total connection time.
     */
    virtual void addConnectionTime(unsigned secs) = 0;

    /**
     * Sets \a count as the total amount of lines in the chat history.
     */
    virtual void setHistoryLines(unsigned count) = 0;

    /**
     * Sets \a size as the history lexicon size, i.e. the total amount of different words used
     * in the chat history.
     */
    virtual void setHistoryLexiconSize(unsigned size) = 0;

    /**
     * Sets \a size as the roster size, i.e. the amounts of chat contacts.
     */
    virtual void setRosterSize(unsigned size) = 0;

    /**
     * Sets \a size as the enabled roster size, i.e. the amounts of chat contacts allowed to chat
     * with the chatbot.
     */
    virtual void setEnabledRosterSize(unsigned size) = 0;

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

