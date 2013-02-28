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

#ifndef LVK_CLUE_SCRIPTPARSER_H
#define LVK_CLUE_SCRIPTPARSER_H

#include "da-clue/script.h"
#include "da-clue/scripterror.h"
#include <QString>

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace Clue
{

/// \ingroup Lvk
/// \addtogroup Clue
/// @{

/**
 * \brief The ScriptParser class provides a parser of scripts
 */
class ScriptParser
{
public:

    /**
     * Constructs a StricParser object
     */
    ScriptParser();

    /**
     * Parses \a filename into \a script. Returns true on success; Otherwise returns false.
     */
    bool parse(const QString &filename, Clue::Script &script);

    /**
     * Returns the last error.
     */
    ScriptError error();

private:
    ScriptError m_error;
};

/// @}

} // namespace Clue

/// @}

} // namespace Lvk


#endif // LVK_CLUE_SCRIPTPARSER_H

