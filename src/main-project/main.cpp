/*
 * Copyright (C) 2012 Andres Pagliano, Gabriel Miretti, Gonzalo Buteler,
 * Nestor Bustamante, Pablo Perez de Angelis
 *
 * This file is part of LVK Botmaster.
 *
 * LVK Botmaster is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * LVK Botmaster is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LVK Botmaster.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <QApplication>
#include <QTranslator>
#include <QDir>

#include "mainwindow.h"
#include "version.h"
#include "settings.h"
#include "settingskeys.h"

void makeDirStructure();
void setLanguage(QApplication &app);

//--------------------------------------------------------------------------------------------------
// main
//--------------------------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    QApplication::setOrganizationName(ORGANIZATION_NAME);
    QApplication::setOrganizationDomain(ORGANIZATION_DOMAIN);
    QApplication::setApplicationName(APP_NAME);

    QApplication app(argc, argv);

    makeDirStructure();
    setLanguage(app);

    Lvk::FE::MainWindow window;
    window.show();

    return app.exec();
}

//--------------------------------------------------------------------------------------------------
// Helpers
//--------------------------------------------------------------------------------------------------

void setLanguage(QApplication &app)
{
    Lvk::Common::Settings settings;
    QString lang = settings.value(SETTING_APP_LANGUAGE, QString("es")).toString();

    QTranslator *translator = new QTranslator();
    translator->load(":/strings/strings_" + lang);
    app.installTranslator(translator);
}

//--------------------------------------------------------------------------------------------------

void makeDirStructure()
{
    Lvk::Common::Settings settings;
    QString logsPath = settings.value(SETTING_LOGS_PATH).toString();

    QDir qdir;

    if (!qdir.exists(logsPath)) {
        if (!qdir.mkpath(logsPath)) {
            qCritical("Critical: Cannot create logs path");
            exit(1);
        }
    }
}
