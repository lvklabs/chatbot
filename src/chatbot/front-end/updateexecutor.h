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

#ifndef LVK_FE_UPDATEEXECUTOR_H
#define LVK_FE_UPDATEEXECUTOR_H

#include "da-server/updater.h"

#include <QObject>

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace FE
{

/// \ingroup Lvk
/// \addtogroup FE
/// @{

/**
 * \brief The UpdateExecutor class executes a functor depending on the availability of updates.
 */
class UpdateExecutor : public QObject
{
    Q_OBJECT

public:

    /**
     * Functor abstract interface
     */
    class Functor
    {
    public:
        /**
         * Functor executed on no update
         */
        virtual void operator()() = 0;

        /**
         * Functor executed on update
         */
        virtual void operator()(DAS::UpdateInfo info) = 0;

        /**
         * Destroys the object
         */
        virtual ~Functor() { }
    };

    /**
     * Predicate over DAS::UpdateInfo
     */
    typedef bool (*Pred)(const DAS::UpdateInfo &info);


    /**
     * Constructs a UpdateExecutor object
     */
    UpdateExecutor(QObject *parent = 0)
        : QObject(parent), m_f(0), m_pred(0), m_delete(false)
    {
        connect(&m_updater, SIGNAL(noUpdate()), SLOT(onNoUpdate()));
        connect(&m_updater, SIGNAL(update(DAS::UpdateInfo)), SLOT(onUpdate(DAS::UpdateInfo)));
    }

    /**
     * Destroys the object and the functor given.
     */
    ~UpdateExecutor()
    {
        delete m_f;
    }

    /**
     * Executes functor \a f() if there is an update and predicate \a pred
     * is true. Otherwise executes functor \a f(DAS::UpdateInfo). \a pred can be null.
     */
    void execute(Functor *f, Pred pred = 0)
    {
        m_f = f;
        m_pred = pred;
        m_updater.checkForUpdate();
    }

    /**
     * Deletes \a this object automatically after handling the update() or noUpdate() signal
     */
    void autoDelete()
    {
        m_delete = true;
    }

    /**
     * This is a shorthand for:
     \code
        UpdateExecutor *e = new UpdateExecutor();
        e->autoDelete();
        e->execute(f, pred);
     \endcode
     */
    static void exec(Functor *f, Pred pred = 0)
    {
        UpdateExecutor *e = new UpdateExecutor();
        e->autoDelete();
        e->execute(f, pred);
    }

private slots:

    void onNoUpdate()
    {
        (*m_f)();

        if (m_delete) {
            deleteLater();
        }
    }

    void onUpdate(const DAS::UpdateInfo &info)
    {
        if (!m_pred || m_pred(info)) {
            (*m_f)(info);
        } else {
            (*m_f)();
        }

        if (m_delete) {
            deleteLater();
        }
    }

private:
    Functor *m_f;
    Pred m_pred;
    DAS::Updater m_updater;
    bool m_delete;
};

/**
 * This predicate returns true if if the given update has critical severity.
 * Otherwise; returns false.
 */
inline bool isCritical(const DAS::UpdateInfo &info)
{
    return info.severity() == DAS::UpdateInfo::Critical;
}

/**
 * This predicate returns true if if the given update has low severity.
 * Otherwise; returns false.
 */
inline bool isLow(const DAS::UpdateInfo &info)
{
    return info.severity() == DAS::UpdateInfo::Low;
}


/// @}

} // namespace FE

/// @}

} // namespace Lvk


#endif // LVK_FE_UPDATEEXECUTOR_H

