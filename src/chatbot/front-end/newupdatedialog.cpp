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
#include <QApplication>
#include <QClipboard>
#include <QPushButton>
#include <QToolButton>
#include <QLabel>
#include <QLayout>
#include <QTimer>

//--------------------------------------------------------------------------------------------------
// NewUpdateDialog
//--------------------------------------------------------------------------------------------------

Lvk::FE::NewUpdateDialog::NewUpdateDialog(const DAS::UpdateInfo &info, QWidget *parent)
    : QDialog(parent), m_url(info.url()), m_downloadAccepted(false)
{
    setModal(true);
    setMinimumWidth(600);
    setMaximumWidth(600);
    setWindowTitle(tr("New Update"));

    m_label = new QLabel(this);
    m_label->setWordWrap(true);
    m_label->setOpenExternalLinks(true);

    m_hash = new QLabel(this);
    m_hash->setTextInteractionFlags(m_label->textInteractionFlags() | Qt::TextSelectableByMouse);
    m_hash->setCursor(QCursor(Qt::IBeamCursor));

    m_copy = new QToolButton(this);
    m_copy->setAutoRaise(true);
    m_copy->setIcon(QIcon(":/icons/copy.png"));
    m_copy->setToolTip(tr("Copy signature"));

    m_later = new QPushButton(tr("Later"), this);

    m_download = new QPushButton(tr("Download Now!"), this);
    m_download->setDefault(true);
    m_download->setFocus();

    QGridLayout *mainLayout = new QGridLayout(this);
    setLayout(mainLayout);

    QHBoxLayout *hashLayout = new QHBoxLayout();

    QSpacerItem *vspacer = new QSpacerItem(0, 0, QSizePolicy::Fixed, QSizePolicy::Expanding);
    QSpacerItem *hspacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Fixed);

    mainLayout->addWidget (m_label,    0, 0, 1, 3);
    mainLayout->addLayout (hashLayout, 1, 0, 1, 3);
    mainLayout->addItem   (vspacer,    2, 0, 1, 3);
    mainLayout->addItem   (hspacer,    3, 0);
    mainLayout->addWidget (m_later,    3, 1);
    mainLayout->addWidget (m_download, 3, 2);

    hashLayout->setMargin(0);
    hashLayout->addWidget(m_hash);
    hashLayout->addWidget(m_copy);
    hashLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Fixed));

    connect(m_download, SIGNAL(clicked()), SLOT(onAccepted()));
    connect(m_later,    SIGNAL(clicked()), SLOT(onRejected()));
    connect(m_copy,     SIGNAL(clicked()), SLOT(onCopyHash()));

    setUpdateInfo(info);
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
        msg.append(tr("<p>SHA-1 signature for this udpate is:</p>"));
    }

    m_label->setText(msg);

    if (!info.hash().isEmpty()) {
        m_hash->setText(info.hash());
    } else {
        m_hash->setVisible(false);
        m_copy->setVisible(false);
    }

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

//--------------------------------------------------------------------------------------------------

void Lvk::FE::NewUpdateDialog::onCopyHash()
{
    QApplication::clipboard()->setText(m_hash->text());
}

