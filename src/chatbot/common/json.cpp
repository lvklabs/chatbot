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

#include "json.h"

#include <QtDebug>

//--------------------------------------------------------------------------------------------------
// Json
//--------------------------------------------------------------------------------------------------

bool Lvk::Cmn::Json::parse(const QString &json, Json::Object &obj)
{
    return parseObject(json, obj);
}

//--------------------------------------------------------------------------------------------------

bool Lvk::Cmn::Json::parseObject(const QString &json, Json::Object &obj)
{
    QString parsed;
    if (removeBraces(json, parsed)) {
        return parseKeyValues(parsed, obj);
    } else {
        return false;
    }
}

//--------------------------------------------------------------------------------------------------

bool Lvk::Cmn::Json::removeBraces(const QString &json, QString &parsed)
{
    QString tmp = json.trimmed();
    if (tmp.size() >= 2 && tmp[0] == '{' && tmp[tmp.size() - 1] == '}') {
        parsed = tmp.mid(1, tmp.size() - 2);
        return true;
    } else {
        return false;
    }
}

//--------------------------------------------------------------------------------------------------

bool Lvk::Cmn::Json::parseKeyValues(const QString &keyvalues, Json::Object &obj)
{
    enum {
        SearchingKey,
        ParsingKey,
        SearchingColon,
        SearchingValue,
        ParsingStringValue,
        ParsingDoubleValue,
        // TODO ParsingNestedObjectValue,
        // TODO ParsingListValue,
        SearchingComma
    } stage = SearchingKey;

    QString key;
    QString value;
    QChar cc; // current char
    QChar pc; // previous char
    bool err = false;

    for (int i = 0; i < keyvalues.size() && !err; ++i) {
        cc = keyvalues[i];
        pc = i > 0 ? keyvalues[i - 1] : '\0';

        switch (stage) {
        case SearchingKey:
            if (cc.isSpace()) {
                // nothing to do
            } else if (cc == '"') {
                stage = ParsingKey;
            } else {
                err = true;
            }
            break;

        case ParsingKey:
            if (cc == '"') {
                if (key.size() > 0) {
                    stage = SearchingColon;
                } else {
                    err = true;
                }
            } else {
                key.append(cc);
            }
            break;

        case SearchingColon:
            if (cc.isSpace()) {
                // nothing to do
            } else if (cc == ':') {
                stage = SearchingValue;
            } else {
                err = true;
            }
            break;

        case SearchingValue:
            if (cc.isSpace()) {
                // nothing to do
            } else if (cc == '"') {
                stage = ParsingStringValue;
            } else if (cc.isDigit() || cc == '.') {
                --i;
                stage = ParsingDoubleValue;
            } else if (cc == '{') {
                qCritical() << "Json: Nested objects are not supported";
                err = true; // TODO implement
            } else if (cc == '[') {
                qCritical() << "Json: Lists are not supported";
                err = true; // TODO implement
            } else {
                err = true;
            }
            break;

        case ParsingStringValue:
            if (cc == '"' && pc != '\\') {
                obj.append(KeyValue(key, value));
                key.clear();
                value.clear();
                stage = SearchingComma;
            } else {
                value.append(cc);
            }
            break;

        case ParsingDoubleValue:
            if (cc.isDigit() || cc == '.') {
                value.append(cc);
            }
            if (cc.isSpace() || i == keyvalues.size() - 1) {
                bool ok = true;
                obj.append(KeyValue(key, value.toDouble(&ok)));
                err = !ok;
                key.clear();
                value.clear();
                stage = SearchingComma;
            }
            break;

        case SearchingComma:
            if (cc.isSpace()) {
                // nothing to do
            } else if (cc == ',') {
                stage = SearchingKey;
            } else {
                err = true;
            }
            break;
        }
    }

    return !err && (stage == SearchingKey || stage == SearchingComma);
}
