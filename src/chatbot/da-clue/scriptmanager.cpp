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

#include "da-clue/scriptmanager.h"
#include "common/settings.h"
#include "common/settingskeys.h"

#include <QStringList>
#include <QFile>
#include <QtDebug>

//--------------------------------------------------------------------------------------------------
// ScriptManager
//--------------------------------------------------------------------------------------------------

Lvk::Clue::ScriptManager::ScriptManager()
{
    initPaths();
}

//--------------------------------------------------------------------------------------------------

void Lvk::Clue::ScriptManager::initPaths()
{
    qDebug() << "ScriptManager: Initializing paths...";

    m_clueBasePath = Cmn::Settings().value(SETTING_CLUE_PATH).toString();
    if (!m_clueBasePath.endsWith("/")) {
        m_clueBasePath.append("/");
    }

    m_charsPath = m_clueBasePath + Cmn::Settings().value(SETTING_CLUE_CHARS_FILE).toString();
}

//--------------------------------------------------------------------------------------------------

QList<Lvk::Clue::Character> Lvk::Clue::ScriptManager::characters()
{
    QList<Clue::Character> chars;

    qDebug() << "ScriptManager: Opening file" << m_charsPath;

    QFile f(m_charsPath);

    if (f.open(QFile::ReadOnly)) {
        QStringList lines = QString::fromUtf8(f.readAll()).split("\n");

        foreach (QString line, lines) {
            line = line.trimmed();

            if (line.isEmpty()) {
                continue;
            }

            qDebug() << "ScriptManager: Parsing character" << line;

            // If detective
            if (line.endsWith("*")) {
                line.truncate(line.size() - 1);
                chars.append(Clue::Character(line, true));
            } else {
                chars.append(Clue::Character(line));
            }
        }
    } else {
        qCritical() << "ScriptManager: Could not open" << m_charsPath;
    }

    return chars;
}

//--------------------------------------------------------------------------------------------------

Lvk::Clue::ScriptError Lvk::Clue::ScriptManager::loadScriptsForCharacter(
        const Clue::Character &/*c*/)
{
    // TODO

    return Clue::UnknownError;
}

//--------------------------------------------------------------------------------------------------

const QList<Lvk::Clue::Script> & Lvk::Clue::ScriptManager::scripts()
{
    return m_scripts;
}

//--------------------------------------------------------------------------------------------------

void Lvk::Clue::ScriptManager::clear()
{
    m_scripts.clear();
}

