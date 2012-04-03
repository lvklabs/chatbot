/*
 * Copyright (C) 2012 Andres Pagliano, Gabriel Miretti, Gonzalo Buteler,
 * Nestor Bustamante, Pablo Perez de Angelis
 *
 * This file is part of LVK Botmaster.
 *
 * LVK Botmaster is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * LVK Botmaster is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LVK Botmaster.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef LVK_NLP_SANITIZER_H
#define LVK_NLP_SANITIZER_H

#include <QString>
#include <QStringList>

namespace Lvk
{

namespace Nlp
{

/**
 * \brief Abstract interface for all NLP Sanitizers
 */

class Sanitizer
{
public:

    /**
     * \brief Sanitizes the given string
     */
    virtual QString sanitize(const QString &str) const = 0;

    /**
     * \brief Sanitizes each string of the given list
     */
    virtual QStringList sanitize(const QStringList &list) const
    {
        QStringList sanitizedList;
        for (int i = 0; i < list.size(); ++i) {
            sanitizedList.append(sanitize(list[i]));
        }
        return sanitizedList;
    }

    virtual ~Sanitizer() {}
};

} // namespace Nlp

} // namespace Lvk


#endif // LVK_NLP_SANITIZER_H
