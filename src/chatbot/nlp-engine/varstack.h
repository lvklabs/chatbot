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

#ifndef LVK_NLP_VARSTACK_H
#define LVK_NLP_VARSTACK_H

#include <QString>
#include <QtDebug>

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace Nlp
{

/// \ingroup Lvk
/// \addtogroup Nlp
/// @{

/**
 * \brief
 */
struct VarScope
{
    VarScope(int start = 0, int end = 0)
        : start(start), end(end) { }

    int start;
    int end;

    bool isNull()
    {
        return !start && !end;
    }

    void clear()
    {
        start = 0;
        end = 0;
    }

    bool contains(int i)
    {
        return start <= i && i <= end;
    }
};


inline QDebug& operator<<(QDebug& dbg, const VarScope &s)
{
    dbg.nospace() << s.start << "," << s.end;
    return dbg.maybeSpace();
}


/**
 * \brief
 */
struct Variable
{
    Variable(const QString &name = "", const VarScope &scope = VarScope())
        : name(name), scope(scope) { }

    QString name;
    QString value;
    VarScope scope;

    bool isNull()
    {
        return name.isNull() && scope.isNull() && value.isNull();
    }

    void clear()
    {
        name.clear();
        scope.clear();
        value.clear();
    }
};

inline QDebug& operator<<(QDebug& dbg, const Variable &i)
{
    dbg.nospace() << i.name << "," << i.scope << "," << i.value;
    return dbg.maybeSpace();
}


/**
 * \brief
 */
class VarStack : public QList<Variable>
{
public:

    QString value(const QString &varName) const
    {
        for (int i = size() - 1; i >= 0; --i) {
            if (at(i).name == varName) {
                return at(i).value;
            }
        }
        return QString();
    }
};

/// @}

} // namespace Nlp

/// @}

} // namespace Lvk


#endif // LVK_NLP_VARSTACK_H

