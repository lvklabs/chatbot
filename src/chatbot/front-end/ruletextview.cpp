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

#define SEP_1       "&nbsp;"
#define SEP_2       "<br/>"
#define HL_START    "<span style=\"color:#0000ff;\">"
#define HL_END      "</span>"

//--------------------------------------------------------------------------------------------------
// Helpers
//--------------------------------------------------------------------------------------------------
//
// Helpers to get a string representation of rules

namespace
{

// Join l and highlight n-th element
inline QString hljoin(const QStringList &l, int n)
{
    QString s;

    for (int i = 0; i < l.size(); ++i) {
        if (i == n) {
            s += HL_START;
        }

        s += SEP_1 + l[i] + SEP_2;

        if (i == n) {
            s += HL_END;
        }
    }

    return s;
}

//--------------------------------------------------------------------------------------------------

inline QString childrenToHtml(const Lvk::BE::Rule *rule)
{
    QString s;

    foreach (Lvk::BE::Rule *r, rule->children()) {
        if (!r->input().isEmpty()) {
            if (!s.isEmpty()) {
                s += SEP_2;
            }
            s += SEP_1 + r->input().first();
        }
    }

    return s;
}

//--------------------------------------------------------------------------------------------------

// HTML string for the given rule with n-th input highlighted
inline QString ruleToHtml(const Lvk::BE::Rule *rule, int n)
{
    if (!rule) {
        return "";
    } else if (rule->type() == Lvk::BE::Rule::OrdinaryRule) {
        return QString(QObject::tr("<b>If user writes:</b><br/>%1<br/><br/>"
                                   "<b>Chatbot replies:</b><br/>%2"))
                .arg(hljoin(rule->input(), n))
                .arg(hljoin(rule->output(), -1)); // TODO highlight output
    } else if (rule->type() == Lvk::BE::Rule::ContainerRule) {
        return QString(QObject::tr("<b>Category:</b><br/>%1<br/><br/>"
                                   "<b>Rules:</b><br/>%2"))
                .arg(SEP_1 + rule->name())
                .arg(childrenToHtml(rule));
    } else if (rule->type() == Lvk::BE::Rule::EvasiveRule) {
        return QString(QObject::tr("<b>If Chatbot does not understand:</b><br/>%1"))
                .arg(hljoin(rule->output(), -1));  // TODO highlight output
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
    setRule(rule, -1);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::RuleTextView::setRule(const BE::Rule *rule, int n)
{
    if (rule) {
        setText(ruleToHtml(rule, n));
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

