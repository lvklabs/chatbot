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

#ifndef LVK_CMN_SETTINGS_H
#define LVK_CMN_SETTINGS_H

#include <QSettings>

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace Cmn
{

/// \ingroup Lvk
/// \addtogroup Cmn
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
     * Dummy struct used to construct a Settings object that stores in a location specific to
     * the current user (e.g., in the user's home directory).
     */
    struct UserScope {};

    /**
     * Constructs a Settings object with the given parent and scope local to the current
     * instance of the chatbot. Settings are stored in the working directory.
     */
    explicit Settings(QObject *parent = 0);

    /**
     * Constructs a Settings object with the given parent and scope user. \see UserScope
     */
    explicit Settings(UserScope, QObject *parent = 0);

    /**
     * Returns the value for the given key. If the key doesn't exist, returns defaultValue.
     */
    virtual QVariant value(const QString &key, const QVariant &defaultValue = QVariant()) const;
};

/// @}

} // namespace Cmn

/// @}

} // namespace Lvk


#endif // LVK_CMN_SETTINGS_H
