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

#include "front-end/uploaderprogressdialog.h"
#include "da-server/datauploaderfactory.h"

#include <QMessageBox>
#include <QTimer>

//--------------------------------------------------------------------------------------------------
// UploaderProgressDialog
//--------------------------------------------------------------------------------------------------

Lvk::FE::UploaderProgressDialog::UploaderProgressDialog(const DAS::ContestData &data,
                                                        QWidget *parent)
    : QProgressDialog(tr("Uploading..."), tr("Cancel"), 0, 0, parent),
      m_uploader(DAS::DataUploaderFactory().createUploader()),
      m_data(data)
{
    connect(m_uploader,
            SIGNAL(finished(DAS::ContestDataUploader::Status)),
            SLOT(onUploadFinished(DAS::ContestDataUploader::Status)));
}

//--------------------------------------------------------------------------------------------------

Lvk::FE::UploaderProgressDialog::~UploaderProgressDialog()
{
    delete m_uploader;
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::UploaderProgressDialog::onUploadFinished(DAS::ContestDataUploader::Status status)
{
    //hide();

    if (status == DAS::ContestDataUploader::Success) {
        QString title = tr("Upload score");
        QString message = tr("Score uploaded successfully!");
        QMessageBox::information(this, title, message);
    } else {
        QString title = tr(" Error ") + QString::number(status);
        QString message = tr("Could not upload score. Please, check your internet "
                             "connection and try again");
        QMessageBox::critical(this, title, message);
    }

    done(status);
}

//--------------------------------------------------------------------------------------------------

int Lvk::FE::UploaderProgressDialog::exec()
{
    QTimer::singleShot(2000, this, SLOT(startUpload()));

    return QProgressDialog::exec();
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::UploaderProgressDialog::startUpload()
{
    m_uploader->upload(m_data);
}

