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
#include "rule.h"
#include "ruletreemodel.h"

#include <QString>

namespace Lvk
{

namespace FE
{

/**
 * The ExportDialog class provides a dialog that allow users to select rules and the filename
 * of the file being exported
 */

class ExportDialog : public PortDialog
{
    Q_OBJECT

public:
    explicit ExportDialog(QWidget *parent = 0);

    ExportDialog(const QString &title, const QString &msg, RuleTreeModel *model,
                 QWidget *parent = 0);

    int exec(BE::Rule *container);

private:
    RuleTreeModel *m_model;

    void copyCheckedRules(BE::Rule *to, BE::Rule *from);
};

} // namespace FE

} // namespace Lvk

#endif // LVK_FE_EXPORTDIALOG_H
