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

#ifndef LVK_NLP_GLOBALTOOLS_H
#define LVK_NLP_GLOBALTOOLS_H

#include "nlp-engine/sanitizer.h"
#include "nlp-engine/lemmatizer.h"
#include <memory>

class QMutex;

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace Nlp
{

/// \ingroup Lvk
/// \addtogroup Nlp
/// @{

// TODO refactor and remove this class!

/**
 * \brief The GlobalTools class provides collection of common NLP tools
 */
class GlobalTools
{
public:
    static GlobalTools* instance();


    Sanitizer * preSanitizer();

    void setPreSanitizer(Sanitizer *sanitizer);

    Lemmatizer * lemmatizer();

    void setLemmatizer(Lemmatizer *lemmatizer);

    Sanitizer * postSanitizer();

    void setPostSanitizer(Sanitizer *sanitizer);

private:
    GlobalTools();
    GlobalTools(GlobalTools&);
    GlobalTools& operator=(GlobalTools&);

    static GlobalTools *m_instance;
    static QMutex *m_mutex;

    // TODO use share pointers
    std::auto_ptr<Sanitizer>  m_preSanitizer;
    std::auto_ptr<Lemmatizer> m_lemmatizer;
    std::auto_ptr<Sanitizer>  m_postSanitizer;
};

/// @}

} // namespace Nlp

/// @}

} // namespace Lvk


#endif // LVK_NLP_GLOBALTOOLS_H

