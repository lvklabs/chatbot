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

#ifndef LVK_FE_EXPORTDIALOG_H
#define LVK_FE_EXPORTDIALOG_H

#include "portdialog.h"

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
 * \brief The ExportDialog class provides a dialog that allow users to select rules to export to
 */

class ExportDialog : public PortDialog
{
    Q_OBJECT

public:

    /**
     * Constructs an ExportDialog widget which is a child of the given parent.
     */
    explicit ExportDialog(QWidget *parent = 0);

    /**
     * Constructs an ExportDialog widget with the given title, model and parent widget.
     */
    ExportDialog(const QString &title, RuleTreeModel *model, QWidget *parent = 0);
};

/// @}

} // namespace FE

/// @}

} // namespace Lvk

#endif // LVK_FE_EXPORTDIALOG_H
