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

#ifndef LVK_DAS_CONTESTDATAUPLOADER_H
#define LVK_DAS_CONTESTDATAUPLOADER_H

#include <QObject>

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace DAS
{

struct ContestData;

/// \ingroup Lvk
/// \addtogroup DAS
/// @{

/**
 * \brief The ContestDataUploader class provides the interface for contest data uploaders
 */
class ContestDataUploader : public QObject
{
    Q_OBJECT

public:

    /**
     * Uploads \a data to the 'Data Aceptar' server. Emits finished() when the upload
     * finishes
     */
    virtual void upload(const ContestData &data) = 0;


    /**
     * Upload status
     */
    enum Status
    {
        Success,
        ConnectionError,
        ChannelError,
        SecureLogError,
        UploadInProgressError
    };

signals:

    /**
     * This signal is emitted whenever the upload has finished
     */
    void finished(DAS::ContestDataUploader::Status status);

};

/// @}

} // namespace DAS

/// @}

} // namespace Lvk


#endif // LVK_DAS_CONTESTDATAUPLOADER_H
