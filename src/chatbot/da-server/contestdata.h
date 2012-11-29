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

#ifndef LVK_DAS_CONTESTDATA_H
#define LVK_DAS_CONTESTDATA_H

#include <QString>
#include "stats/score.h"

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace DAS
{

/// \ingroup Lvk
/// \addtogroup DAS
/// @{

/**
 * \brief The ContestData struct provides a simple structure with all the data that must be sent
 *        to participate in the contest
 */
struct ContestData
{
    QString filename;       ///< Chatbot rules filename
    QString username;       ///< Username
    QString chatbotId;      ///< Chatbot ID
    Stats::Score bestScore; ///< Chatbot best score
};

/// @}

} // namespace DAS

/// @}

} // namespace Lvk


#endif // LVK_DAS_CONTESTDATA_H

