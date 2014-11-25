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

#include "da-clue/batchanalyzer.h"
#include "back-end/appfacade.h"
#include "common/globalstrings.h"
#include "common/version.h"

#include <QtGlobal>
#include <QtAlgorithms>
#include <QString>
#include <QStringList>
#include <QFileInfo>
#include <QDir>

#include <iostream>

#ifdef WIN32
// Windows does not support console output if the application has a window.
// So we print the output in a file:
#define BATCH_OUTPUT_FILENAME   "batch-mode-result.txt"
#endif

//--------------------------------------------------------------------------------------------------
// Helpers
//--------------------------------------------------------------------------------------------------

namespace
{

const int PRINT_FILENAME_MAX_LEN = 30;

//--------------------------------------------------------------------------------------------------

inline QString covFormat(float f)
{
    return QString("%1%").arg(f, 6, 'f', 2, QChar(' '));
}

//--------------------------------------------------------------------------------------------------

inline float globalCoverage(const Lvk::Clue::AnalyzedList &ascripts)
{
    if (ascripts.isEmpty()) {
        return 0.0;
    }

    float gcov = 0.0;

    foreach (const Lvk::Clue::AnalyzedScript &s, ascripts) {
        gcov += s.coverage;
    }

    return gcov / ascripts.size();
}

//--------------------------------------------------------------------------------------------------

inline QString truncatedFilename(const QString &filename)
{
    QString truncFilename = filename.right(PRINT_FILENAME_MAX_LEN);

    if (truncFilename.size() < filename.size()) {
        truncFilename.replace(0, 3, "...");
    }

    return truncFilename;
}

}

//--------------------------------------------------------------------------------------------------
// BatchAnalyzer
//--------------------------------------------------------------------------------------------------

Lvk::Clue::BatchAnalyzer::BatchAnalyzer(QObject *parent /*= 0*/)
    : QObject(parent), m_appFacade(0)
    #ifdef WIN32
      , m_outputFile(BATCH_OUTPUT_FILENAME)
    #endif
{
}

//--------------------------------------------------------------------------------------------------

Lvk::Clue::BatchAnalyzer::~BatchAnalyzer()
{
    delete m_appFacade;
}

//--------------------------------------------------------------------------------------------------

int Lvk::Clue::BatchAnalyzer::exec(const QString &target)
{
    printInfo(tr("Starting %1 in batch processing mode...").arg(APP_NAME));

    QFileInfo info(target);

    if (!info.exists()) {
        printErr(tr("File or directory '%1' does not exist").arg(target));

        return 1;
    }

    #ifdef WIN32
    if (!m_outputFile.isOpen()) {
        if (!m_outputFile.open(QFile::WriteOnly | QFile::Text)) {
            printErr(tr("Cannot create file %1").arg(m_outputFile.fileName()));

            return 2;
        }
    }
    #endif

    if (!m_appFacade) {
        m_appFacade = new BE::AppFacade();
    }

    m_data.clear();
    analyze(target, 0);
    qSort(m_data);
    printDataCollection();

    return dataCollectionHasError() ? 3 : 0;
}

//--------------------------------------------------------------------------------------------------

void Lvk::Clue::BatchAnalyzer::printDataCollection()
{
    if (m_data.size() == 1 && m_data.first().depth == 0) {
        printDetails(m_data.first());
    } else {
        foreach (const DataEntry &p, m_data) {
            printBrief(p);
        }
    }
}

//--------------------------------------------------------------------------------------------------

bool Lvk::Clue::BatchAnalyzer::dataCollectionHasError()
{
    if (m_data.isEmpty()) {
        return true;
    }

    foreach (const DataEntry &p, m_data) {
        if (p.hasError()) {
            return true;
        }
    }

    return false;
}

//--------------------------------------------------------------------------------------------------

void Lvk::Clue::BatchAnalyzer::analyze(const QString &target, int depth)
{
    QFileInfo info(target);

    if (info.isDir()) {
        analyzeDir(target, depth);
    } else {
        analyzeFile(target, depth);
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::Clue::BatchAnalyzer::analyzeDir(const QString &dirname, int depth)
{
    if (depth > 0 && dirname.endsWith(EXTRAS_DIR_SUFFIX)) {
        return;
    }

    printInfo(tr("Analyzing directory '%1'...").arg(dirname));

    QFileInfoList infoList = QDir(dirname).entryInfoList(
                QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot, QDir::DirsLast);

    if (infoList.size() > 0) {
        foreach (QFileInfo info, infoList) {
            analyze(info.filePath(), depth + 1);
        }
    } else {
        if (depth == 0) {
            printErr(tr("No files found in directory '%1'").arg(dirname));
        } else {
            printWarn(tr("No files found in directory '%1'").arg(dirname));
        }
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::Clue::BatchAnalyzer::analyzeFile(const QString &filename, int depth)
{
    DataEntry entry(filename, depth);

    if (m_appFacade->load(filename)) {
        QString username = m_appFacade->username();
        entry.username = username.isEmpty() ? tr("(No username)") : username;

        QString character = m_appFacade->currentCharacter();
        if (!character.isEmpty()) {
            entry.character = character;
            Lvk::Clue::AnalyzedList ascripts = m_appFacade->analyzedScripts();
            if (ascripts.size() > 0) {
                entry.ascripts = ascripts;
                entry.globalCoverage = globalCoverage(ascripts);
            } else {
                entry.error = tr("No scripts found for character '%1'").arg(character);
            }
        } else {
            entry.error = tr("No script character set");
        }
    } else {
        entry.error = tr("Cannot load file");
    }

    m_data.append(entry);
}

//--------------------------------------------------------------------------------------------------

void Lvk::Clue::BatchAnalyzer::printBrief(const DataEntry &entry)
{
    if (entry.hasError()) {
        printInfo(tr("%1 ERROR: %2")
                  .arg(truncatedFilename(entry.filename), -PRINT_FILENAME_MAX_LEN)
                  .arg(entry.error));
    } else {
        QString strPartialCov;
        foreach (Clue::AnalyzedScript s, entry.ascripts) {
            if (strPartialCov.size() > 0) {
                strPartialCov += " ";
            }
            strPartialCov += covFormat(s.coverage);
        }

        printInfo(tr("%1 %2 %3 Global: %4   Partials: %5")
                  .arg(truncatedFilename(entry.filename), -PRINT_FILENAME_MAX_LEN)
                  .arg(entry.username, -30, ' ')
                  .arg(entry.character, -12, ' ')
                  .arg(covFormat(entry.globalCoverage))
                  .arg(strPartialCov));
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::Clue::BatchAnalyzer::printDetails(const DataEntry &entry)
{
    if (entry.hasError()) {
        printInfo(tr("Chabot File: %1\nERROR: %2")
                  .arg(entry.filename)
                  .arg(entry.error));
    } else {
        QString strPartialCov;
        foreach (Clue::AnalyzedScript s, entry.ascripts) {
            strPartialCov += QString("  %1   %2\n").arg(covFormat(s.coverage), s.filename);
        }

        printInfo(tr("Chabot File: %1\nUsername: %2\nCharacter: %3\n"
                     "Global Coverage: %4\nPartial Coverages:\n%5")
                  .arg(entry.filename)
                  .arg(entry.username)
                  .arg(entry.character)
                  .arg(covFormat(entry.globalCoverage))
                  .arg(strPartialCov));
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::Clue::BatchAnalyzer::printErr(const QString &msg)
{
    print(tr("ERROR: ") + msg, true);
}

//--------------------------------------------------------------------------------------------------

void Lvk::Clue::BatchAnalyzer::printWarn(const QString &msg)
{
    print(tr("Warning: ") + msg);
}

//--------------------------------------------------------------------------------------------------

void Lvk::Clue::BatchAnalyzer::printInfo(const QString &msg)
{
    print(msg);
}

//--------------------------------------------------------------------------------------------------

void Lvk::Clue::BatchAnalyzer::print(const QString &msg, bool err)
{
    (err ? std::cerr : std::cout) << msg.toUtf8().data() << std::endl;

#ifdef WIN32
    m_outputFile.write(msg.toUtf8());
    m_outputFile.write("\n");
#endif
}
