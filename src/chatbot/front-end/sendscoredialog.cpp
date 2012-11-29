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

#include "front-end/sendscoredialog.h"
#include "stats/score.h"
#include "back-end/rule.h"

#include <QFile>
#include <QDebug>

//--------------------------------------------------------------------------------------------------
// SendScoreDialog
//--------------------------------------------------------------------------------------------------

Lvk::FE::SendScoreDialog::SendScoreDialog(const Stats::Score &s, const QString &rulesFile,
                                          QWidget *parent)
    : DetailsDialog(QObject::tr("Send score? This will also send your rule definitions."),
                    QObject::tr("See what I'm sending."),
                    getSendScoreDetails(s, rulesFile), parent)
{
}

//--------------------------------------------------------------------------------------------------

QString Lvk::FE::SendScoreDialog::getSendScoreDetails(const Stats::Score &s,
                                                      const QString &rulesFile)
{
    QString details;

    // Append score
    details += QString("%1,%2,%3,%4\n").arg(QString::number(s.contacts),
                                            QString::number(s.conversations),
                                            QString::number(s.rules),
                                            QString::number(s.total));


    // Append file contents.
    QFile file(rulesFile);

    if (file.open(QFile::ReadOnly)) {
        qDebug() << "SendScoreDialog: Reading rules file" << rulesFile;

        const int BUF_SIZE = 1024*10;
        char buf[BUF_SIZE];
        int len = 0;

        while ( (len = file.readLine(buf, BUF_SIZE)) > 0 ) {
            for (int i = 0; i < len; ++i) {
                char c = buf[i];
                if (c > 0) {
                    details += c;
                }
            }
        }
    } else {
        qCritical() << "SendScoreDialog: Could not read rules file" << rulesFile;
        details += tr("[Error: Could not read rules file]");
    }

    return details;
}
