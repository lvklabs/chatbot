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

#ifndef LVK_FE_FILEDIALOG_H
#define LVK_FE_FILEDIALOG_H

#include <QString>
#include <QObject>
#include <QFileDialog>

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
 * \brief The FileDialog class provides localized strings for file extensions
 *        and common dialogs to open and save chatbots
 */
// TODO rename to FileDialog
class FileDialog
{
public:

    /**
     * Localized chatbot rules file extension
     */
    static QString chatbotExtension()
    {
        return QString(QObject::tr("crf"));
    }

    /**
     * Localized chatbot export file extension
     */
    static QString exportExtension()
    {
        return QString(QObject::tr("cef"));
    }

    /**
     * Shows the "New chatbot file" dialog. Returns the selected filename or
     * an empty string if the dialog was canceled
     */
    static QString newChatbot(QWidget *parent = 0)
    {
        QString filename = QFileDialog::getSaveFileName(parent,
                                                        QObject::tr("New file"), "",
                                                        chatbotFilter());

        appendExtension(filename, chatbotExtension());

        return filename;
    }

    /**
     * Shows the "Save chatbot file" dialog. Returns the selected filename or
     * an empty string if the dialog was canceled
     */
    static QString saveChatbot(QWidget *parent = 0)
    {
        QString filename = QFileDialog::getSaveFileName(parent,
                                                        QObject::tr("Save File"), "",
                                                        chatbotFilter());

        appendExtension(filename, chatbotExtension());

        return filename;
    }

    /**
     * Shows the "Open chatbot file" dialog. Returns the selected filename or
     * an empty string if the dialog was canceled
     */
    static QString openChatbot(QWidget *parent = 0)
    {
        return QFileDialog::getOpenFileName(parent,
                                            QObject::tr("Open File"), "",
                                            chatbotFilter());
    }


    /**
     * Shows the "Save export file" dialog. Returns the selected filename or
     * an empty string if the dialog was canceled
     */
    static QString saveExport(QWidget *parent = 0)
    {
        QString filename = QFileDialog::getSaveFileName(parent,
                                                        QObject::tr("Export Rules"), "",
                                                        exportFilter());

        appendExtension(filename, exportExtension());

        return filename;
    }

    /**
     * Shows the "Open export file" dialog. Returns the selected filename or
     * an empty string if the dialog was canceled
     */
    static QString openExport(QWidget *parent = 0)
    {
        return QFileDialog::getOpenFileName(parent,
                                            QObject::tr("Import Rules"), "",
                                            exportFilter());
    }

private:
    FileDialog();

    static void appendExtension(QString &filename, const QString &ext)
    {
        if (!filename.isEmpty() && !filename.endsWith("." + ext)) {
            filename.append("." + ext);
        }
    }

    static QString chatbotFilter()
    {
        return QObject::tr("Chatbot Rule Files") + QString(" (*.") + chatbotExtension()
                + QString(");;") + QObject::tr("All files") + QString(" (*.*)");
    }

    static QString exportFilter()
    {
        return QObject::tr("Chatbot Export Files") + QString(" (*.") + exportExtension()
                + QString(");;") + QObject::tr("All files") + QString(" (*.*)");
    }
};

/// @}

} // namespace FE

/// @}

} // namespace Lvk


#endif // LVK_FE_FILEDIALOG_H

