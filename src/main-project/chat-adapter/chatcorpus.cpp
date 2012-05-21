/*
 * Copyright (C) 2012 Andres Pagliano, Gabriel Miretti, Gonzalo Buteler,
 * Nestor Bustamante, Pablo Perez de Angelis
 *
 * This file is part of LVK Botmaster.
 *
 * LVK Botmaster is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * LVK Botmaster is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LVK Botmaster.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "chatcorpus.h"

#include <QFile>
#include <QStringList>
#include <QRegExp>

#define CORPUS_FILE     "./corpus.dat"

#define SENTENCE_SPLIT_TOKEN    "\n"
#define USER_SENT_SPLIT_TOKEN   ":\t"


//--------------------------------------------------------------------------------------------------
// Helpers
//--------------------------------------------------------------------------------------------------

namespace
{

QString sanitize(const QString &str)
{
    return QString(str).replace(QRegExp("\\s+"), " ");
}

} // namespace


//--------------------------------------------------------------------------------------------------
// ChatCorpus
//--------------------------------------------------------------------------------------------------

bool Lvk::CA::ChatCorpus::m_init = false;

QFile Lvk::CA::ChatCorpus::m_corpusFile(CORPUS_FILE);

QList<Lvk::CA::ChatCorpus::CorpusEntry> Lvk::CA::ChatCorpus::m_corpus;

//--------------------------------------------------------------------------------------------------

Lvk::CA::ChatCorpus::ChatCorpus()
{
    if (!m_init) {
        load();

        if (!m_corpusFile.open(QFile::WriteOnly)) {
            qWarning(QObject::tr("Warning: cannot open corpus file for writing"));
        }

        m_init = true;
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
    m_corpus.append(entry);

    if (m_corpusFile.isOpen()) {
        m_corpusFile.write(sanitize(entry.first).toUtf8());
        m_corpusFile.write(USER_SENT_SPLIT_TOKEN);
        m_corpusFile.write(sanitize(entry.second).toUtf8());
        m_corpusFile.write(SENTENCE_SPLIT_TOKEN);
        m_corpusFile.flush();
    }
}

//--------------------------------------------------------------------------------------------------

const QList<Lvk::CA::ChatCorpus::CorpusEntry> & Lvk::CA::ChatCorpus::corpus()
{
    return m_corpus;
}

//--------------------------------------------------------------------------------------------------

void Lvk::CA::ChatCorpus::load()
{
    if (!m_corpusFile.exists()) {
        return;
    }
    if (!m_corpusFile.open(QFile::ReadOnly)) {
        qWarning(QObject::tr("Warning: cannot open corpus file for reading"));

        return;
    }

    char buf[10*1024];
    qint64 len = 0;
    QString line;
    QStringList tokens;

    do {
        len = m_corpusFile.readLine(buf, sizeof(buf));
        if (len > 0) {
            line = QString::fromUtf8(buf);
            tokens = line.split(USER_SENT_SPLIT_TOKEN);
            if (tokens.size() == 2) {
                add(tokens[0], tokens[1]);
            }
        }
    } while (len != -1);

    m_corpusFile.close();
}
