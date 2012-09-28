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

#ifndef LVK_CMN_JSON_H
#define LVK_CMN_JSON_H

#include <QVariantList>
#include <QString>
#include <QMetaType>

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
 * \brief The Json class provides a simple JSON parser.
 *
 * Warning! Partially implemented. Nested objects and lists are not supported.
 */

class Json
{
public:

    /**
     * The KeyValue class provides a JSON key-value object
     */
    typedef QPair<QString, QVariant> KeyValue;

    /**
     * The Object class provides a JSON object. A JSON object is a sequence of zero or more
     * KeyValue objects.
     */
    typedef QList<KeyValue> Object;

    /**
     * Parses the given \a json string into \a obj. Returns true on success. Otherwise, returns
     * false.
     */
    bool parse(const QString &json, Json::Object &obj);

private:
    bool parseObject(const QString &json, Json::Object &obj);
    bool parseKeyValues(const QString &keyval, Json::Object &obj);
    bool removeBraces(const QString &json, QString &parsed);
};

/// @}

} // namespace Cmn

/// @}

} // namespace Lvk

Q_DECLARE_METATYPE(Lvk::Cmn::Json::KeyValue)
Q_DECLARE_METATYPE(Lvk::Cmn::Json::Object)

#endif // LVK_CMN_JSON_H

