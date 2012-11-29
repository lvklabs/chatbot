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

#ifndef LVK_FE_UPLOADERPROGRESSDIALOG_H
#define LVK_FE_UPLOADERPROGRESSDIALOG_H

#include <QProgressDialog>

#include "da-server/contestdata.h"
#include "da-server/contestdatauploader.h"

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
 * \brief The UploaderProgressDialog class provides a progress dialog that invokes a
 * SftpContestDataUploader.
 *
 * Calling exec() starts the upload and the progress dialog is displayed while the
 * upload is in progress. Once finished the dialog is closed and exec() returns zero if the
 * upload was success.
 */

class UploaderProgressDialog : public QProgressDialog
{
    Q_OBJECT

public:
    /**
     * Constructs an UploaderProgressDialog widget which is a child of \a parent.
     */
    explicit UploaderProgressDialog(const DAS::ContestData &data, QWidget *parent = 0);

    /**
     * Destroys the object.
     */
    ~UploaderProgressDialog();

    /**
     * Shows the dialog as a modal dialog, blocking until the user closes it or the upload finishes.
     * The function returns zero if the upload was success. Otherwise; returns non-zero value.
     */
    int exec();

private slots:
    void startUpload();
    void onUploadFinished(DAS::ContestDataUploader::Status status);

private:
    DAS::ContestDataUploader *m_uploader;
    DAS::ContestData m_data;
};

/// @}

} // namespace FE

/// @}

} // namespace Lvk


#endif // LVK_FE_UPLOADERPROGRESSDIALOG_H

