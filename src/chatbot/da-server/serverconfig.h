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
#ifndef LVK_DAS_SERVERCONFIG_H
#define LVK_DAS_SERVERCONFIG_H

#ifdef DA_CONTEST
#  ifdef DA_SERVER_CONFIG
#    include "daserverconfig.h"
#  else
#    error "daserverconfig.h is required to build a DA release"
#    error "Contact andres.pagliano@lvklabs.com for support"
#  endif
#else
#  include "nullserverconfig.h"
#endif

#endif // LVK_DAS_SERVERCONFIG_H
