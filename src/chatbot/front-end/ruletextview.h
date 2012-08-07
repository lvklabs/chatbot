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

#ifndef LVK_FE_RULETEXTVIEW_H
#define LVK_FE_RULETEXTVIEW_H

#include <QTextEdit>

namespace Lvk
{

namespace BE
{

class Rule;

}

/// \addtogroup Lvk
/// @{

namespace FE
{

/// \ingroup Lvk
/// \addtogroup FE
/// @{

/**
 * \brief The RuleTextView class provides a read-only text edit widget to display a rule.
 */

class RuleTextView : public QTextEdit
{
    Q_OBJECT

public:

    /**
     * Constructs a RuleTextView widget which is a child of \a parent.
     */
    explicit RuleTextView(QWidget *parent = 0);

    /**
     * Displays \a rule in the widget.
     */
    void setRule(const BE::Rule *rule);

    /**
     * Displays \a rule in the widget and highlights \a n-th input (starting from zero)
     */
    void setRule(const BE::Rule *rule, int n);

    /**
     * Returns the ID of the rule being displayed. Returns 0 if there is no rule.
     */
    quint64 ruleId();

private:
    quint64 m_ruleId;
};

/// @}

} // namespace FE

/// @}

} // namespace Lvk


#endif // LVK_FE_RULETEXTVIEW_H

