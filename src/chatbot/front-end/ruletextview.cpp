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

#include "front-end/ruletextview.h"
#include "back-end/rule.h"

#include <QString>

//--------------------------------------------------------------------------------------------------
// Helpers
//--------------------------------------------------------------------------------------------------
//
// Helpers to get a string representation of rules

namespace
{

inline QString rulesOfCategoryToString(const Lvk::BE::Rule *rule)
{
    QString str;
    foreach (Lvk::BE::Rule *r, rule->children()) {
        if (!r->input().isEmpty()) {
            if (!str.isEmpty()) {
                str.append("<br/>");
            }
            str.append(r->input().first());
        }
    }
    return str;
}

inline QString ruleToString(const Lvk::BE::Rule *rule)
{
    if (!rule) {
        return "";
    } else if (rule->type() == Lvk::BE::Rule::OrdinaryRule) {
        return QString(QObject::tr("<b>If user writes:</b><br/>%1<br/><br/>"
                                   "<b>Chatbot replies:</b><br/>%2"))
                .arg(rule->input().join("<br/>"))
                .arg(rule->output().join("<br/>"));
    } else if (rule->type() == Lvk::BE::Rule::ContainerRule) {
        return QString(QObject::tr("<b>Category:</b><br/>%1<br/><br/>"
                                   "<b>Rules:</b><br/>%2"))
                .arg(rule->name())
                .arg(rulesOfCategoryToString(rule));
    } else if (rule->type() == Lvk::BE::Rule::EvasiveRule) {
        return QString(QObject::tr("<b>If Chatbot does not understand:</b><br/>%1"))
                .arg(rule->output().join("<br/>"));
    } else {
        return QObject::tr("<Unknown Rule Type>");
    }
}

} // namespace

//--------------------------------------------------------------------------------------------------
// RuleTextView
//--------------------------------------------------------------------------------------------------

Lvk::FE::RuleTextView::RuleTextView(QWidget *parent)
    : QTextEdit(parent), m_ruleId(0)
{
    setReadOnly(true);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::RuleTextView::setRule(const BE::Rule *rule)
{
    if (rule) {
        setText(ruleToString(rule));
        m_ruleId = rule->id();
    } else {
        setText("");
        m_ruleId = 0;
    }
}

//--------------------------------------------------------------------------------------------------

quint64 Lvk::FE::RuleTextView::ruleId()
{
    return m_ruleId;
}

