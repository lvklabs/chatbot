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

#ifndef LVK_FE_NEWUPDATEDIALOG_H
#define LVK_FE_NEWUPDATEDIALOG_H

#include <QObject>
#include <QDialog>

class QLabel;
class QToolButton;
class QPushButton;

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace DAS
{
class UpdateInfo;
}

namespace FE
{

/// \ingroup Lvk
/// \addtogroup FE
/// @{

/**
 * \brief The NewUpdateDialog class provides a dialog that displays information about a new update
 */
class NewUpdateDialog : public QDialog
{
    Q_OBJECT

public:

    /**
     * Constructs a NewUpdateDialog object which is child of \a parent and display the update
     * information \a info
     */
    NewUpdateDialog(const DAS::UpdateInfo &info, QWidget *parent = 0);

private slots:
    void onRejected();
    void onAccepted();
    void onTimeout();
    void onCopyHash();

private:
    QString m_url;
    QLabel *m_label;
    QLabel *m_hash;
    QPushButton *m_later;
    QPushButton *m_download;
    QToolButton *m_copy;
    bool m_downloadAccepted;

    void setUpdateInfo(const DAS::UpdateInfo &info);
};

/// @}

} // namespace FE

/// @}

} // namespace Lvk


#endif // LVK_FE_NEWUPDATEDIALOG_H

