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

#include "da-server/gelf.h"
#include "da-server/zlibhelper.h"
#include "common/version.h"

#include <QString>
#include <QHostInfo>

#define GELF_MSG_TEMPLATE   "{"\
                            "\"version\": \"1.0\","\
                            "\"facility\": \"%1\","\
                            "\"host\": \"%2\","\
                            "\"short_message\": \"%3\","\
                            "\"full_message\": \"%4\","\
                            "\"level\": %5,"\
                            "%6"\
                            "}"

//--------------------------------------------------------------------------------------------------
// Gelf
//--------------------------------------------------------------------------------------------------

Lvk::DAS::Gelf::Gelf()
{
    buildGelf(Critical, "", FieldList());
}

//--------------------------------------------------------------------------------------------------

Lvk::DAS::Gelf::Gelf(Level level, const QString &msg)
{
    buildGelf(level, msg, FieldList());
}

//--------------------------------------------------------------------------------------------------

Lvk::DAS::Gelf::Gelf(Level level, const QString &msg, const FieldList &fields)
{
    buildGelf(level, msg, fields);
}

//--------------------------------------------------------------------------------------------------

bool Lvk::DAS::Gelf::isValid()
{
    return !m_data.isEmpty();
}

//--------------------------------------------------------------------------------------------------

void Lvk::DAS::Gelf::buildGelf(Level level, const QString &msg, const FieldList &fields)
{
    QString gelfMsg = QString(GELF_MSG_TEMPLATE).arg(QString(APP_NAME "_" APP_VERSION_STR),
                                                     QHostInfo::localHostName(),
                                                     msg, msg,
                                                     QString::number(level),
                                                     fields.toString());

    if (ZLibHelper::deflate(gelfMsg.toUtf8(), m_data) != Z_OK) {
        m_data.clear();
    }
}

//--------------------------------------------------------------------------------------------------
// Gelf::Field
//--------------------------------------------------------------------------------------------------

Lvk::DAS::Gelf::Field::Field()
{
}

//--------------------------------------------------------------------------------------------------

Lvk::DAS::Gelf::Field::Field(const QPair<QString, QString> &p)
    : QPair<QString, QString>(p)
{
}

//--------------------------------------------------------------------------------------------------

QString Lvk::DAS::Gelf::Field::toString() const
{
    if (first.startsWith("_")) {
        return QString("\"%1\" : \"%2\"").arg(first, second);
    } else {
        return QString("\"_%1\" : \"%2\"").arg(first, second);
    }
}

//--------------------------------------------------------------------------------------------------
// Gelf::FieldList
//--------------------------------------------------------------------------------------------------

QString Lvk::DAS::Gelf::FieldList::toString() const
{
    QString str;
    for (int i = 0; i < size(); ++i) {
        str += operator[](i).toString() + ",";
    }
    return str;
}

