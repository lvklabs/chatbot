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

#ifndef LVK_FE_FILEFILTERS_H
#define LVK_FE_FILEFILTERS_H

#include <QString>
#include <QObject>

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace FE
{

/// \ingroup Lvk
/// \addtogroup FE
/// @{

/**
 * \brief The FileFilters class provides localized strings for common file filters and extensions
 */
class FileFilters
{
public:

    /**
     * Localized file filters for open/save dialogs
     */
    static QString chatbotFilter()
    {
        return QObject::tr("Chatbot Rule Files") + QString(" (*.") + chatbotExtension()
                + QString(");;") + QObject::tr("All files") + QString(" (*.*)");
    }

    /**
     * Localized export file filters for import/export dialogs
     */
    static QString exportFilter()
    {
        return QObject::tr("Chatbot Export Files") + QString(" (*.") + exportExtension()
                + QString(");;") + QObject::tr("All files") + QString(" (*.*)");
    }

    /**
     * Localized chatbot rules file extension
     */
    static QString chatbotExtension()
    {
        return QString(QObject::tr("crf"));
    }

    /**
     * Localized chatbot export file extension
     */
    static QString exportExtension()
    {
        return QString(QObject::tr("cef"));
    }

private:
    FileFilters();

};

/// @}

} // namespace FE

/// @}

} // namespace Lvk


#endif // LVK_FE_FILEFILTERS_H

