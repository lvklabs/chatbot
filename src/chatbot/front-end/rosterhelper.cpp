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

#include "rosterhelper.h"
#include "common/settings.h"
#include "common/settingskeys.h"

#include <QFile>
#include <QDir>
#include <QDataStream>
#include <QtDebug>

//--------------------------------------------------------------------------------------------------
// RosterHelper
//--------------------------------------------------------------------------------------------------

Lvk::FE::RosterHelper::RosterHelper(RosterType type, const QString &account)
    : m_type(type), m_account(account)
{
    QString dataPath = Lvk::Cmn::Settings().value(SETTING_DATA_PATH).toString();
    QString prefix = m_type == FullRoster ? "roster_" : "black_roster_";

    m_filename = dataPath + QDir::separator() + prefix + account + ".dat";
}

//--------------------------------------------------------------------------------------------------

bool Lvk::FE::RosterHelper::save(const Lvk::BE::Roster &roster)
{
    return saveFile(roster, m_filename);
}

//--------------------------------------------------------------------------------------------------

bool Lvk::FE::RosterHelper::load(Lvk::BE::Roster &roster)
{
    return loadFile(roster, m_filename);
}

//--------------------------------------------------------------------------------------------------

bool Lvk::FE::RosterHelper::exists()
{
    return QFile::exists(m_filename);
}

//--------------------------------------------------------------------------------------------------

bool Lvk::FE::RosterHelper::remove()
{
    return QFile::remove(m_filename);
}

//--------------------------------------------------------------------------------------------------

bool Lvk::FE::RosterHelper::saveFile(const Lvk::BE::Roster &roster, const QString &filename)
{
    QFile file(filename);
    if (file.open(QFile::WriteOnly)) {
        QDataStream out(&file);
        out << roster;
        return out.status() == QDataStream::Ok;
    } else {
        qCritical() << "RosterHelper: Cannot not save roster in " << filename;
        return false;
    }
}

//--------------------------------------------------------------------------------------------------

bool Lvk::FE::RosterHelper::loadFile(Lvk::BE::Roster &roster, const QString &filename)
{
    QFile file(filename);
    if (file.open(QFile::ReadOnly)) {
        QDataStream in(&file);
        in >> roster;
        return in.status() == QDataStream::Ok;
    } else {
        qCritical() << "RosterHelper: Cannot not load roster from " << filename;
        return false;
    }
}

