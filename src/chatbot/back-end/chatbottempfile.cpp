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

#include "back-end/chatbottempfile.h"
#include "back-end/chatbotrulesfile.h"
#include "back-end/filemetadata.h"
#include "stats/statsmanager.h"

#include <QtDebug>
#include <QVariant>
#include <QDir>
#include <QFile>


//--------------------------------------------------------------------------------------------------
// ChatbotTempFile
//--------------------------------------------------------------------------------------------------

QString Lvk::BE::ChatbotTempFile::getTempFileForUpload(const QString &origFilename)
{
    QString tmpFile;

    try {
        if (origFilename.isEmpty()) {
            throw QString("Empty chatbot rules filename");
        }

        int retry = 0;
        int MAX_RETRIES = 1000;

        do {
            tmpFile = QDir::tempPath() + QDir::separator() + "chatbot" + QString::number(retry);

            if (QFile::exists(tmpFile) && !QFile::remove(tmpFile)) {
                qWarning() << "ChatbotTempFile: Cannot remove temp file" << tmpFile;
            }
        } while (!QFile::copy(origFilename, tmpFile) && ++retry < MAX_RETRIES);

        if (retry >= MAX_RETRIES) {
            throw QString("Retries exceded");
        }

        BE::ChatbotRulesFile rules;

        if (!rules.load(tmpFile)) {
            throw QString("Cannot load temp rules file");
        }

        setNewRoster(rules);

        if (!rules.save()) {
            throw QString("Cannot save changes in temp rules file");
        }

    } catch (const QString &err) {
        qCritical() << "ChatbotTempFile: Cannot create temp file:" << err;

        tmpFile.clear();
    }

    return tmpFile;
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::ChatbotTempFile::setNewRoster(BE::ChatbotRulesFile &rules)
{
    // Replace the full roster with only those contacts that have scored

    BE::Roster scoreRoster;
    QSet<QString> scoreContacts = Stats::StatsManager::manager()->scoreContacts();

    foreach(const QString& c, scoreContacts) {
        scoreRoster.append(BE::RosterItem(c, QString()));
    }

    rules.setMetadata(FILE_METADATA_ROSTER,       QVariant::fromValue(scoreRoster));
    rules.setMetadata(FILE_METADATA_BLACK_ROSTER, QVariant::fromValue(BE::Roster()));
}
