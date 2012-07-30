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

#ifndef LVK_BE_CONVERSATIONWRITER_H
#define LVK_BE_CONVERSATIONWRITER_H

#include "back-end/conversation.h"

class QIODevice;
class QString;
class QByteArray;

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace BE
{

/// \ingroup Lvk
/// \addtogroup BE
/// @{

/**
 * \brief The ConversationWriter class provides a format independent interface for writing chat
 *        conversations to files or other devices.
 *
 * The ConversationWriter class provides a format independent interface for writing chat
 * conversations to files or other devices. By default the device or file is opened in append mode.
 * To not use default append mode you must provide a device already opened with the desired flags.
 *
 * To read a conversation see the ConversationReader class.
 */

class ConversationWriter
{
public:

    /**
     * Constructs an empty ConversationWriter object.
     */
    ConversationWriter();

    /**
     * Constructs a ConversationWriter object using the given device.
     * After construction class owns the pointer.
     */
    ConversationWriter(QIODevice * device);

    /**
     * Constructs a ConversationWriter object that will write to a file with the given name.
     */
    ConversationWriter(const QString & filename);

    /**
     * Destructs the ConversationWriter object and frees the device if was given any.
     */
    ~ConversationWriter();

    /**
     * Writes the given conversation to the assigned device or file name.
     * Returns true on success; otherwise returns false.
     */
    bool write(const Conversation& conv);

    /**
     * Writes the given conversation entry to the assigned device or file name.
     * Returns true on success; otherwise returns false.
     */
    bool write(const Conversation::Entry &entry);

    /**
     * Returns true if the current write position is at the end of the device; otherwise returns
     * false.
     */
    bool atEnd();

private:
    ConversationWriter(const ConversationWriter&);
    ConversationWriter& operator=(const ConversationWriter&);

    QIODevice *m_device;

    bool writeln(const QByteArray &data);
    bool flush();
};

/// @}

} // namespace BE

/// @}

} // namespace Lvk


#endif // LVK_BE_CONVERSATIONWRITER_H

