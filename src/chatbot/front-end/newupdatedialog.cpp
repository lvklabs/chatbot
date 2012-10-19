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
#include <QPushButton>
#include <QLabel>
#include <QLayout>
#include <QTimer>

//--------------------------------------------------------------------------------------------------
// NewUpdateDialog
//--------------------------------------------------------------------------------------------------

Lvk::FE::NewUpdateDialog::NewUpdateDialog(const DAS::UpdateInfo &info, QWidget *parent)
    : QDialog(parent), m_url(info.url()), m_label(new QLabel(this)), m_downloadAccepted(false)
{
    setModal(true);
    setMinimumWidth(600);
    setMaximumWidth(600);
    setWindowTitle(tr("New Update"));
    setUpdateInfo(info);

    m_label->setWordWrap(true);

    QGridLayout *layout = new QGridLayout(this);
    setLayout(layout);

    m_later = new QPushButton(tr("Later"), this);
    m_download = new QPushButton(tr("Download Now!"), this);
    m_download->setDefault(true);

    QSpacerItem *vspacer = new QSpacerItem(0, 0, QSizePolicy::Fixed, QSizePolicy::Expanding);
    QSpacerItem *hspacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Fixed);

    layout->addWidget(m_label,    0, 0, 1, 3);
    layout->addItem(vspacer,      1, 0, 1, 3);
    layout->addItem(hspacer,      2, 0);
    layout->addWidget(m_later,    2, 1);
    layout->addWidget(m_download, 2, 2);

    connect(m_download, SIGNAL(clicked()), SLOT(onAccepted()));
    connect(m_later,    SIGNAL(clicked()), SLOT(onRejected()));
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::NewUpdateDialog::setUpdateInfo(const DAS::UpdateInfo &info)
{
    QString msg = QString(tr("<p><b>New update: Chatbot %1</b></p>"))
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

    m_label->setText(msg);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::NewUpdateDialog::onAccepted()
{
    if (!m_downloadAccepted) {
        QDesktopServices::openUrl(QUrl(m_url));
        m_downloadAccepted = true;
        m_download->setText(tr("Downloading..."));
        m_download->setEnabled(false);
        QTimer::singleShot(3000, this, SLOT(onTimeout()));
        m_later->hide();
    } else {
        setResult(QDialog::Accepted);
        close();
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::NewUpdateDialog::onRejected()
{
    setResult(QDialog::Rejected);
    close();
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::NewUpdateDialog::onTimeout()
{
    m_download->setText(tr("Continue"));
    m_download->setEnabled(true);
}

