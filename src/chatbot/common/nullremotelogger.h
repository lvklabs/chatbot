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

#ifndef LVK_CMN_NULLREMOTELOGGER_H
#define LVK_CMN_NULLREMOTELOGGER_H

#include "common/remotelogger.h"

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace Cmn
{

/// \ingroup Lvk
/// \addtogroup Cmn
/// @{

/**
 * \brief The NullRemoteLogger class provides a remote logger that does nothing
 */

class NullRemoteLogger : public RemoteLogger
{
public:

    /**
     * Creates a NullRemoteLogger object.
     */
    NullRemoteLogger() { }

    /**
     * Creates a NullRemoteLogger object.
     */
    NullRemoteLogger(LogFomat /*format*/, bool /*encrypt*/) { }

    /**
     * \copydoc RemoteLogger::log(const QString&)
     */
    virtual int log(const QString &/*msg*/) { return 0; }

    /**
     * \copydoc RemoteLogger::log(const QString &, const FieldList&)
     */
    virtual int log(const QString &/*msg*/, const FieldList &/*fields*/) { return 0; }
};

/// @}

} // namespace Cmn

/// @}

} // namespace Lvk


#endif // LVK_CMN_NULLREMOTELOGGER_H

