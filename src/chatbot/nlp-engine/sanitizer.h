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

#ifndef LVK_NLP_SANITIZER_H
#define LVK_NLP_SANITIZER_H

#include <QString>
#include <QStringList>

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace Nlp
{

/// \ingroup Lvk
/// \addtogroup Nlp
/// @{

/**
 * \brief The Sanitizer class provides an abstract interface for all sanitizers.
 *
 * A Sanitizer removes harmful and/or not useless characters from strings. For instance,
 * a sanitzer may remove extra whitespaces, null or non-ASCII characters.
 */
class Sanitizer
{
public:

    /**
     * Destroys the object.
     */
    virtual ~Sanitizer() {}

    /**
     * Sanitizes the string \a str
     */
    virtual QString sanitize(const QString &str) const = 0;

    /**
     * Sanitizes each string of the list \a list and returns a new one.
     */
    virtual QStringList sanitize(const QStringList &list) const
    {
        QStringList sanitizedList;
        for (int i = 0; i < list.size(); ++i) {
            sanitizedList.append(sanitize(list[i]));
        }
        return sanitizedList;
    }
};

/// @}

} // namespace Nlp

/// @}

} // namespace Lvk


#endif // LVK_NLP_SANITIZER_H
