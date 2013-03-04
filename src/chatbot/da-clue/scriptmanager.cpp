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
#include "da-clue/scriptparser.h"
#include "common/settings.h"
#include "common/settingskeys.h"

#include <QStringList>
#include <QDir>
#include <QFile>
#include <QtDebug>
#include <QObject>

//--------------------------------------------------------------------------------------------------
// ScriptManager
//--------------------------------------------------------------------------------------------------

Lvk::Clue::ScriptManager::ScriptManager()
    : m_error(Clue::NoError)
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

    if (f.open(QFile::ReadOnly | QFile::Text)) {
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

bool Lvk::Clue::ScriptManager::loadScriptsForCharacter(const Clue::Character &c)
{
    return loadScriptsForCharacter(c.name);
}

//--------------------------------------------------------------------------------------------------

bool Lvk::Clue::ScriptManager::loadScriptsForCharacter(const QString &name)
{
    qDebug() << "ScriptManager: Loading scripts for character" << name;

    QDir dir(m_clueBasePath);
    QStringList nameFilters;
    nameFilters.append(QString("*%1*").arg(name));
    QStringList files = dir.entryList(nameFilters, QDir::Files, QDir::Name);

    Clue::ScriptParser parser;
    Clue::Script script;

    resetError();

    foreach (const QString &file, files) {
        if (parser.parse(m_clueBasePath + file, script)) {
            m_scripts.append(script);
        } else {
            setError(parser.error(), file);
            break;
        }
    }

    return m_error == Clue::NoError;
}

//--------------------------------------------------------------------------------------------------

const Lvk::Clue::ScriptList & Lvk::Clue::ScriptManager::scripts()
{
    return m_scripts;
}

//--------------------------------------------------------------------------------------------------

void Lvk::Clue::ScriptManager::clear()
{
    m_scripts.clear();
    m_errMsg.clear();
    m_error = Clue::NoError;
}

//--------------------------------------------------------------------------------------------------

Lvk::Clue::ScriptError Lvk::Clue::ScriptManager::error(QString *errMsg)
{
    if (errMsg) {
        *errMsg = m_errMsg;
    }

    return m_error;
}

//--------------------------------------------------------------------------------------------------

void Lvk::Clue::ScriptManager::setError(Clue::ScriptError err, const QString &filename)
{
    m_error = err;

    switch (err) {
    case Clue::NoError:
        m_errMsg = "";
        break;

    case Clue::FileNotFoundError:
        m_errMsg = QObject::tr("File not found '%1'").arg(filename);
        break;

    case Clue::ReadError:
        m_errMsg = QObject::tr("Cannot read file '%1'").arg(filename);
        break;

    case Clue::InvalidFormatError:
        m_errMsg = QObject::tr("Invalid format in file '%1'").arg(filename);
        break;

    case Clue::UnknownError:
    default:
        m_errMsg = QObject::tr("Unknown error while parsing file '%1'").arg(filename);
        break;
    }

    if (!m_errMsg.isEmpty()) {
        qDebug() << "ScriptManager: " << m_errMsg;
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::Clue::ScriptManager::resetError()
{
    m_error = Clue::NoError;
    m_errMsg.clear();
}

