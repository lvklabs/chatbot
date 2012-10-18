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
        msg.append(tr("<p><b><span style=\"color:#ff0000\">This is a critical update!</span> "
                      "You cannot connect your chatbot, send your score or verify new accounts "
                      "until you update the program.</b></p>"));
    }

    if (info.whatsNew().size() > 0) {
        msg.append(tr("<p>What's new:<br/> - ") + info.whatsNew().join("<br/> - ") + "</p>");
    }

    msg.append(tr("</p>We recommend you to <a href=\"http://www.daleaceptar.gob.ar/cms/chat/"
                  "descarga-chatbot/verifica-tu-descarga/#sha1\">verify your program</a> once"
                  " it is downloaded. <a href=\"http://www.daleaceptar.gob.ar/cms/chat/"
                  "descarga-chatbot/verifica-tu-descarga/\">What's this?</a></p>"));

    if (!info.hash().isEmpty()) {
        msg.append(QString(tr("<p>SHA-1 signature for this udpate is %1</p>")).arg(info.hash()));
    }

    setText(msg);

}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::NewUpdateDialog::onAccepted()
{
    QDesktopServices::openUrl(QUrl(m_url));
}

