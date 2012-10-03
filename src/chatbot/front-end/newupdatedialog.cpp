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

#include "front-end/newupdatedialog.h"
#include "da-server/updateinfo.h"

#include <QDesktopServices>
#include <QUrl>

//--------------------------------------------------------------------------------------------------
// NewUpdateDialog
//--------------------------------------------------------------------------------------------------

Lvk::FE::NewUpdateDialog::NewUpdateDialog(const DAS::UpdateInfo &info, QWidget *parent)
    : QMessageBox(parent), m_url(info.url())
{
    setWindowTitle(tr("New Update"));
    setUpdateInfo(info);
    addButton(tr("Later"), QMessageBox::RejectRole);
    addButton(tr("Download Now!"), QMessageBox::AcceptRole);

    connect(this, SIGNAL(accepted()), SLOT(onAccepted()));
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::NewUpdateDialog::setUpdateInfo(const DAS::UpdateInfo &info)
{
    QString msg = QString(tr("<p><b>New update - Chatbot %1</b></p>"))
            .arg(info.version().toString());

    if (info.severity() == DAS::UpdateInfo::Critical) {
        msg.append(tr("<p><span style=\"color:#ff0000\"><b>"
                      "This is a critical update! You cannot connect your chatbot or send your "
                      "score until you update the program.</b></span></p>"));
    } else {
        msg.append(tr("<p>This new version fixes several bugs. We recommend you to update the "
                      "application.</p>"));
    }

    if (info.whatsNew().size() > 0) {
        msg.append(tr("<p>What's new:<br/> - ") + info.whatsNew().join("<br/> - ") + "</p>");
    }

    setText(msg);

}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::NewUpdateDialog::onAccepted()
{
    QDesktopServices::openUrl(QUrl(m_url));
}

