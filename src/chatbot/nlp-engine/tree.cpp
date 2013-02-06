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

#include "tree.h"

//--------------------------------------------------------------------------------------------------
// Tree
//--------------------------------------------------------------------------------------------------

Lvk::Nlp::Tree::Tree()
{
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::Tree::add(const Rule &/*rule*/)
{
    // TODO
}

//--------------------------------------------------------------------------------------------------

QStringList Lvk::Nlp::Tree::getResponses(const QString &/*input*/, Engine::MatchList &matches)
{
    QStringList responses;

    // TODO
    responses.append("*** NOT IMPLEMENTED ***");
    matches.append(QPair<RuleId, int>(0,0));

    return responses;
}

