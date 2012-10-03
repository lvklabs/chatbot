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

//--------------------------------------------------------------------------------------------------
// SendScoreDialog
//--------------------------------------------------------------------------------------------------

Lvk::FE::SendScoreDialog::SendScoreDialog(const Stats::Score &s, const BE::Rule *root,
                                          QWidget *parent)
    : DetailsDialog(QObject::tr("Send score? This will also send your rule definitions."),
                    QObject::tr("See what I'm sending."),
                    getSendScoreDetails(s, root), parent)
{
}

//--------------------------------------------------------------------------------------------------

QString Lvk::FE::SendScoreDialog::getSendScoreDetails(const Stats::Score &s, const BE::Rule *root)
{
    QString details;

    // Append score
    details += QString("%1,%2,%3,%4\n").arg(QString::number(s.contacts),
                                            QString::number(s.conversations),
                                            QString::number(s.rules),
                                            QString::number(s.total));

    // Append rule definitions with custom format (it's not important)
    BE::Rule::const_iterator it;
    for (it = root->begin(); it != root->end(); ++it) {
        const BE::Rule *rule = *it;
        if (rule == root) {
            continue;
        }
        switch (rule->type()) {
        case BE::Rule::OrdinaryRule:
            details += "   " + rule->input().join(",") + " ==> " + rule->output().join(",") + "\n";
            break;
        case BE::Rule::ContainerRule:
            details += "[[" + rule->name() + "]]\n";
            break;
        case BE::Rule::EvasiveRule:
            details += "[[_]]\n" + rule->output().join(",") + "\n";
            break;
        }
    }

    return details;
}
