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

#ifndef LVK_FE_SENDSCOREDIALOG_H
#define LVK_FE_SENDSCOREDIALOG_H

#include "front-end/detailsdialog.h"

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace BE
{
class Rule;
}

namespace Stats
{
class Score;
}

namespace FE
{

/// \ingroup Lvk
/// \addtogroup FE
/// @{

/**
 * \brief The SendScoreDialog class provides a dialog to ask for confirmation before sending
 *        the score and allows the user to see details of what is being sent.
 */
class SendScoreDialog : public DetailsDialog
{
    Q_OBJECT

public:
    /**
     * Constructs a SendScoreDialog with the given score \a s and chatbot rules file \a rulesFile.
     */
    explicit SendScoreDialog(const Stats::Score &s, const QString &rulesFile, QWidget *parent = 0);

private:
    QString getSendScoreDetails(const Stats::Score &s, const QString &rulesFile);
};

/// @}

} // namespace FE

/// @}

} // namespace Lvk


#endif // LVK_FE_SENDSCOREDIALOG_H

