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

#ifndef LVK_BE_AIADAPTER_H
#define LVK_BE_AIADAPTER_H

#include <QString>
#include <QStringList>
#include "chat-adapter/chatbotai.h"

class QReadWriteLock;

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace Nlp
{
    class Engine;
}

namespace Cmn
{
    class ConversationWriter;
}

namespace BE
{

/// \ingroup Lvk
/// \addtogroup BE
/// @{

/**
 * \brief The AIAdapter adapts a Nlp::Engine as a CA::ChatbotAI
 */
class AIAdapter : public CA::ChatbotAI
{

public:

    /**
     * Constructs an AIAdapter with chatbot id \a id, NLP engine \a engine.
     */
    AIAdapter(const QString &id, Nlp::Engine *engine = 0);

    /**
     * Destroys the object. The engine passed is not distroyed.
     */
    ~AIAdapter();

    /**
     * \copydoc CA::ChatbotAI::getEntry()
     */
    virtual Cmn::Conversation::Entry getEntry(const QString &input,
                                              const CA::ContactInfo &contact);
    /**
     * Sets the NLP engine that is used to get responses.
     */
    void setNlpEngine(Nlp::Engine *engine);

    /**
     * Sets the list of evasives.
     * Evasives are senteces used by the virtual user when there is no match in the NLP engine.
     * If there is no match, an evasive is chosen and returned as response.
     */
    void setEvasives(const QStringList &evasives);

private:
    AIAdapter(AIAdapter&);
    AIAdapter& operator=(AIAdapter&);

    QString m_id;
    Nlp::Engine *m_engine;
    QStringList m_evasives;
    QReadWriteLock *m_rwLock;
};

/// @}


} // namespace BE

/// @}

} // namespace Lvk

#endif // LVK_BE_AIADAPTER_H
