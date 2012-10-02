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

#ifndef LVK_DAS_UPDATEINFO_H
#define LVK_DAS_UPDATEINFO_H

#include <QString>
#include <QDate>
#include <QStringList>

#include "da-server/updateversion.h"

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace DAS
{

/// \ingroup Lvk
/// \addtogroup DAS
/// @{

/**
 * \brief The UpdateInfo class provides information about an application update.
 *
 * The UpdateInfo class provides information such as update version, update severity, what's new,
 * download URL, date, etc..
 */
class UpdateInfo
{
public:

    /**
     * Update severity
     */
    enum Severity
    {
        Low,
        Critical
    };

    /**
     *
     */
    void setVersion(const UpdateVersion &version) { m_version = version; }

    /**
     *
     */
    const UpdateVersion & version() const { return m_version; }

    /**
     *
     */
    void setSeverity(Severity severity) { m_severity = severity;}

    /**
     *
     */
    Severity severity() const { return m_severity; }

    /**
     *
     */
    void setWhatsNew(const QStringList &whatsNew) { m_whatsNew = whatsNew; }

    /**
     *
     */
    const QStringList& whatsNew() const { return m_whatsNew; }

    /**
     *
     */
    void setUrl(const QString &url) { m_url = url; }

    /**
     *
     */
    const QString & url() const { return m_url; }


    /**
     *
     */
    void setDate(const QDate &date) { m_date = date; }

    /**
     *
     */
    const QDate & date() const { return m_date; }


private:
    UpdateVersion m_version;
    Severity m_severity;
    QStringList m_whatsNew;
    QString m_url;
    QDate m_date;
};

/// @}

} // namespace DAS

/// @}

} // namespace Lvk


#endif // LVK_DAS_UPDATEINFO_H

