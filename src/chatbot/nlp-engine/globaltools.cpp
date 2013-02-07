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

#include "globaltools.h"
#include "nlp-engine/nullsanitizer.h"
#include "nlp-engine/nulllemmatizer.h"

#include <QMutex>
#include <QMutexLocker>

//--------------------------------------------------------------------------------------------------
// GlobalTools
//--------------------------------------------------------------------------------------------------

Lvk::Nlp::GlobalTools * Lvk::Nlp::GlobalTools::m_instance = 0;
QMutex * Lvk::Nlp::GlobalTools::m_mutex = new QMutex();

//--------------------------------------------------------------------------------------------------

Lvk::Nlp::GlobalTools::GlobalTools()
    : m_preSanitizer(new NullSanitizer()),
      m_lemmatizer(new NullLemmatizer()),
      m_postSanitizer(new NullSanitizer())
{
}

//--------------------------------------------------------------------------------------------------

Lvk::Nlp::GlobalTools * Lvk::Nlp::GlobalTools::instance()
{
    if (!m_instance) {
        QMutexLocker locker(m_mutex);
        if (!m_instance) {
            m_instance = new GlobalTools();
        }
    }

    return m_instance;
}

//--------------------------------------------------------------------------------------------------

Lvk::Nlp::Sanitizer * Lvk::Nlp::GlobalTools::preSanitizer()
{
    return m_preSanitizer.get();
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::GlobalTools::setPreSanitizer(Lvk::Nlp::Sanitizer *sanitizer)
{
    m_preSanitizer.reset(sanitizer ? sanitizer : new NullSanitizer());
}

//--------------------------------------------------------------------------------------------------

Lvk::Nlp::Lemmatizer * Lvk::Nlp::GlobalTools::lemmatizer()
{
    return m_lemmatizer.get();
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::GlobalTools::setLemmatizer(Lvk::Nlp::Lemmatizer *lemmatizer)
{
    m_lemmatizer.reset(lemmatizer ? lemmatizer : new NullLemmatizer());
}

//--------------------------------------------------------------------------------------------------

Lvk::Nlp::Sanitizer * Lvk::Nlp::GlobalTools::postSanitizer()
{
    return m_postSanitizer.get();
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::GlobalTools::setPostSanitizer(Lvk::Nlp::Sanitizer *sanitizer)
{
    m_postSanitizer.reset(sanitizer ? sanitizer : new NullSanitizer());
}


