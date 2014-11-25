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

#ifndef LVK_CLUE_BATCHANALYZER_H
#define LVK_CLUE_BATCHANALYZER_H

#include <QString>
#include <QObject>
#include <QFile>

#include "da-clue/analyzedscript.h"

namespace Lvk
{

namespace BE
{
    class AppFacade;
}

/// \addtogroup Lvk
/// @{

namespace Clue
{

/// \ingroup Lvk
/// \addtogroup Clue
/// @{

/**
 * \brief The BatchAnalyzer class provides a batch execution mode to show script coverage in the
 *        OS console
 */
class BatchAnalyzer : public QObject
{
    Q_OBJECT

public:

    /**
     * Creates a BatchAnalyzer object
     */
    BatchAnalyzer(QObject *parent = 0);

    /**
     * Destroys the object
     */
    ~BatchAnalyzer();

    /**
     * Executes the application batch mode. Returns 0 if success or not zero if there was an error.
     */
    int exec(const QString &target);

private:

    struct DataEntry
    {
        DataEntry()
            : depth(0), globalCoverage(0)
        { }

        DataEntry(const QString &filename, int depth)
            : filename(filename), depth(depth), globalCoverage(0)
        { }

        QString filename;
        QString username;
        QString character;
        AnalyzedList ascripts;
        QString error;
        int depth;
        float globalCoverage;

        bool hasError() const
        {
            return !error.isEmpty();
        }

        bool operator<(const DataEntry& other) const
        {
            return globalCoverage > other.globalCoverage ||
                    (globalCoverage == other.globalCoverage && !hasError());
        }
    };

    typedef QList<DataEntry> DataCollection;

    BE::AppFacade *m_appFacade;
    DataCollection m_data;

#ifdef WIN32
    QFile m_outputFile;
#endif

    void analyze(const QString &target, int depth);
    void analyzeDir(const QString &dirname, int depth);
    void analyzeFile(const QString &filename, int depth);
    void printDataCollection();
    bool dataCollectionHasError();
    void printBrief(const DataEntry &entry);
    void printDetails(const DataEntry &entry);
    void printErr(const QString &msg);
    void printWarn(const QString &msg);
    void printInfo(const QString &msg);
    void print(const QString &msg, bool err = false);
};

/// @}

} // namespace Clue

/// @}

} // namespace Lvk


#endif // LVK_CLUE_BATCHANALYZER_H

