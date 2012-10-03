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

#ifndef LVK_DAS_UPDATEVERSION_H
#define LVK_DAS_UPDATEVERSION_H

#include <QString>
#include <QStringList>

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
 * The UpdateVersion provides a container for update versions and operators to work with.
 */
struct UpdateVersion
{
    /**
     * Creates an empty version \b 0.0
     */
    UpdateVersion() : _major(0), _minor(0) { }

    /**
     * Creates a version \b major.minor"
     */
    UpdateVersion(int major, int minor) : _major(major), _minor(minor) { }

    /**
     * Creates a version from string. The String must have format "major.minor"
     */
    UpdateVersion(const QString &strVer) { *this = fromString(strVer); }

    int _major; ///< Major part of the version
    int _minor; ///< Minor part of the version

    /**
     * Returns the string representation of the current object.
     */
    QString toString() const
    {
        return QString("%1.%2").arg(_major).arg(_minor);
    }

    /**
     * Creates a version from string. The String must have format "major.minor".
     * Returns a valid UpdateVersion if the string format is valid. Otherwise;
     * return an invalid UpdateVersion. \see isValid()
     */
    static UpdateVersion fromString(const QString &strVer)
    {
        UpdateVersion v(-1, -1);
        QStringList tokens = strVer.split(".", QString::SkipEmptyParts);
        if (tokens.size() == 2) {
            v._major = tokens[0].toInt();
            v._minor = tokens[1].toInt();
        }
        return v;
    }

    /**
     * Returns true if the version is valid. Otherwise; returns false.
     */
    bool isValid() const
    {
        return _major >= 0 && _minor >= 0;
    }

    /**
     * Returns true if the version is empty, i.e "0.0". Otherwise; returns false.
     */
    bool isEmpty()  const
    {
        return _major == 0 && _minor == 0;
    }

    /**
     * Clears the object setting all members to zero.
     */
    void clear()
    {
        _major = 0;
        _minor = 0;
    }
};

/// @}

} // namespace DAS

/// @}

} // namespace Lvk

/**
 * Returns true if \a v1 is equal than \a v2. Otherwise; returns false.
 */
inline bool operator==(const Lvk::DAS::UpdateVersion &v1, const Lvk::DAS::UpdateVersion &v2)
{
    return v1._major == v2._major && v1._minor == v2._minor;
}

/**
 * Returns true if \a v1 is less than \a v2. Otherwise; returns false.
 */
inline bool operator<(const Lvk::DAS::UpdateVersion &v1, const Lvk::DAS::UpdateVersion &v2)
{
    return v1._major < v2._major || (v1._major == v2._major && v1._minor < v2._minor);
}


#endif // LVK_DAS_UPDATEVERSION_H

