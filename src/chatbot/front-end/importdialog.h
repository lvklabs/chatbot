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

#ifndef LVK_FE_IMPORTDIALOG_H
#define LVK_FE_IMPORTDIALOG_H

#include "front-end/portdialog.h"

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace FE
{

/// \ingroup Lvk
/// \addtogroup FE
/// @{

/**
 * \brief The ImportDialog class provides a dialog that allow users to select rules to import to.
 */
class ImportDialog : public PortDialog
{
    Q_OBJECT

public:

    /**
     * Constructs an ImportDialog widget which is a child of the given parent.
     */
    explicit ImportDialog(QWidget *parent = 0);

    /**
     * Constructs an ImportDialog widget with the given title, model and parent widget.
     */
    ImportDialog(const QString &title, RuleTreeModel *model, QWidget *parent = 0);
};

/// @}

} // namespace FE

/// @}

} // namespace Lvk

#endif // LVK_FE_IMPORTDIALOG_H
