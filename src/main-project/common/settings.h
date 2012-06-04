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

#ifndef LVK_COMMON_SETTINGS_H
#define LVK_COMMON_SETTINGS_H

#include <QSettings>

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace Common
{

/// \ingroup Lvk
/// \addtogroup Common
/// @{

/**
 * \brief The Settings class provides persistent platform-independent application settings.
 *
 * Settings is a thin wrapper over the QSettings class.
 */
class Settings : public QSettings
{
    Q_OBJECT

public:
    /**
     * Constructs a Settings object with the given parent
     */
    explicit Settings(QObject *parent = 0);

    /**
     * Returns the value for the given key. If the key doesn't exist, returns defaultValue.
     */
    virtual QVariant value(const QString &key, const QVariant &defaultValue = QVariant()) const;

};

/// @}

} // namespace Common

/// @}

} // namespace Lvk


#endif // LVK_COMMON_SETTINGS_H
