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

#include "chatcorpus.h"
#include "csvdocument.h"
#include "globalstrings.h"

#include <QFile>
#include <QStringList>
#include <QRegExp>
#include <QMutex>
#include <QMutexLocker>
#include <QDateTime>

#define CORPUS_FILE     "./corpus.dat"

//--------------------------------------------------------------------------------------------------
// Helpers
//--------------------------------------------------------------------------------------------------

namespace
{

QString sanitize(const QString &str)
{
    return str.simplified();
}

} // namespace


//--------------------------------------------------------------------------------------------------
// ChatCorpus
//--------------------------------------------------------------------------------------------------

bool Lvk::CA::ChatCorpus::m_init = false;

QFile Lvk::CA::ChatCorpus::m_corpusFile(CORPUS_FILE);

QList<Lvk::CA::ChatCorpus::CorpusEntry> Lvk::CA::ChatCorpus::m_corpus;

QMutex *Lvk::CA::ChatCorpus::m_mutex = new QMutex();

//--------------------------------------------------------------------------------------------------

Lvk::CA::ChatCorpus::ChatCorpus()
{
    init();
}

//--------------------------------------------------------------------------------------------------

void Lvk::CA::ChatCorpus::init()
{
    if (!m_init) {
        QMutexLocker locker(m_mutex);
        if (!m_init) {
            load();

            if (!m_corpusFile.open(QFile::Append)) {
                qWarning(QObject::tr("Warning: cannot open corpus file for writing"));
            }

            m_init = true;
        }
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::CA::ChatCorpus::add(const QString &user, const QString &sentence)
{
    add(qMakePair(user, sentence));
}

//--------------------------------------------------------------------------------------------------

void Lvk::CA::ChatCorpus::add(const CorpusEntry &entry)
{
    QMutexLocker locker(m_mutex);

    m_corpus.append(entry);

    if (m_corpusFile.isOpen()) {
        Cmn::CsvRow row;
        row.append(QDateTime::currentDateTime().toString(STR_CHAT_CORPUS_DATE_TIME_FORMAT));
        row.append(sanitize(entry.first));
        row.append(sanitize(entry.second));

        m_corpusFile.write(row.toString().toUtf8());
        m_corpusFile.write("\n");
        m_corpusFile.flush();
    }
}

//--------------------------------------------------------------------------------------------------

QList<Lvk::CA::ChatCorpus::CorpusEntry> Lvk::CA::ChatCorpus::corpus()
{
    QMutexLocker locker(m_mutex);
    return m_corpus;
}

//--------------------------------------------------------------------------------------------------

void Lvk::CA::ChatCorpus::load()
{
    if (!m_corpusFile.exists()) {
        // Nothing to do
        return;
    }
    if (!m_corpusFile.open(QFile::ReadOnly)) {
        qWarning(QObject::tr("Warning: cannot open corpus file for reading"));
        return;
    }

    char buf[10*1024];
    qint64 len = 0;
    QString line;
    Cmn::CsvRow row;

    do {
        len = m_corpusFile.readLine(buf, sizeof(buf));
        if (len > 0) {
            line = QString::fromUtf8(buf);
            row = Cmn::CsvRow(line);

            if (row.size() == 3) {
                m_corpus.append(qMakePair(row[1], row[2]));
            }
        }
    } while (len != -1);

    m_corpusFile.close();
}
