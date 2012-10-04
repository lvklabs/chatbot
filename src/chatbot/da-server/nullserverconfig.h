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
#ifndef LVK_DAS_NULLSERVERCONFIG_H
#define LVK_DAS_NULLSERVERCONFIG_H

// Updater -----------------------------------------------------------------------------------------

#define UPDATER_REST_API_CFU        "http://127.0.0.1/cfu"

// UserAuth ----------------------------------------------------------------------------------------

#define USER_AUTH_REST_FB_TRANSL    "http://127.0.0.1/fbtransl?email=%1"

// Graylog -----------------------------------------------------------------------------------------

#ifdef GELF_STATS_SUPPORT
# define GRAYLOG_HOST               "127.0.0.1"
# define GRAYLOG_UDP_PORT           0
# define GRAYLOG_TCP_PORT           0
#endif

#endif // LVK_DAS_NULLSERVERCONFIG_H
