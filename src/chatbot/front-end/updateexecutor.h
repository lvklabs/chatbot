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

class MainWindow;

/// \ingroup Lvk
/// \addtogroup FE
/// @{

/**
 * \brief The UpdateExecutor class executes members in a given object depending on the availability
 *        of updates.
 */
class UpdateExecutor : public QObject
{
    Q_OBJECT

public:

    typedef void (MainWindow::*Memb1)(const DAS::UpdateInfo &info); ///< Member executed on update
    typedef void (MainWindow::*Memb2)();                    ///< Member executed on no update
    typedef bool (*Pred)(const DAS::UpdateInfo &info);      ///< Predicate

    /**
     * Constructs a UpdateExecutor object
     */
    UpdateExecutor(QObject *parent = 0)
        : QObject(parent), m_obj(0), m_memb1(0), m_memb2(0), m_pred(0), m_delete(false)
    {
        connect(&m_updater, SIGNAL(noUpdate()), SLOT(onNoUpdate()));
        connect(&m_updater, SIGNAL(update(DAS::UpdateInfo)), SLOT(onUpdate(DAS::UpdateInfo)));
    }

    /**
     * Executes member \a memb1 in object \a obj if there is an update and predicate \a pred
     * is true. Otherwise executes \a memb2. \a memb2 and \a pred can be null.
     */
    void execute(MainWindow *obj, Memb1 memb1, Memb2 memb2 = 0, Pred pred = 0)
    {
        m_obj = obj;
        m_memb1 = memb1;
        m_memb2 = memb2;
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
        e->execute(obj, memb1, memb2);
     \endcode
     */
    static void exec(MainWindow *obj, Memb1 memb1, Memb2 memb2 = 0, Pred pred = 0)
    {
        UpdateExecutor *e = new UpdateExecutor();
        e->autoDelete();
        e->execute(obj, memb1, memb2, pred);
    }

    /**
     * This predicate returns true if if the given update has critical severity.
     * Otherwise; returns false.
     */
    static bool isCritical(const DAS::UpdateInfo &info)
    {
        return info.severity() == DAS::UpdateInfo::Critical;
    }

    /**
     * This predicate returns true if if the given update has low severity.
     * Otherwise; returns false.
     */
    static bool isLow(const DAS::UpdateInfo &info)
    {
        return info.severity() == DAS::UpdateInfo::Low;
    }

private slots:

    void onNoUpdate()
    {
        if (m_memb2) {
            (m_obj->*m_memb2)();
        }

        if (m_delete) {
            deleteLater();
        }
    }

    void onUpdate(const DAS::UpdateInfo &info)
    {
        if (!m_pred || m_pred(info)) {
            (m_obj->*m_memb1)(info);
        } else if (m_memb2) {
            (m_obj->*m_memb2)();
        }

        if (m_delete) {
            deleteLater();
        }
    }

private:

    MainWindow *m_obj;
    Memb1 m_memb1;
    Memb2 m_memb2;
    Pred m_pred;
    DAS::Updater m_updater;
    bool m_delete;
};

/// @}

} // namespace FE

/// @}

} // namespace Lvk


#endif // LVK_FE_UPDATEEXECUTOR_H

