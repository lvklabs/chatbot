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

#ifndef LVK_BE_CHATBOTTEMPFILE_H
#define LVK_BE_CHATBOTTEMPFILE_H

#include <QString>

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace BE
{

class ChatbotRulesFile;

/// \ingroup Lvk
/// \addtogroup BE
/// @{

/**
 * \brief The ChatbotTempFile class creates a temporal chatbot rules file without sensible
 *        data to be upload.
 *
 * "Without sensible data" means that clears the roster and keeps only those
 * contacts that have scored.
 */
class ChatbotTempFile
{
public:

    /**
     * Returns the name of a temporal rules file without sensible data to be upload.
     * If an error occurs, returns an empty string.
     */
    QString getTempFileForUpload(const QString &origFilename);

private:
    void setNewRoster(BE::ChatbotRulesFile &rules);
};

/// @}

} // namespace BE

/// @}

} // namespace Lvk


#endif // LVK_BE_CHATBOTTEMPFILE_H

