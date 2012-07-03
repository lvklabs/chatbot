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

#include <QApplication>
#include <QTranslator>
#include <QDir>
#include <QDebug>

#include "mainwindow.h"
#include "version.h"
#include "settings.h"
#include "settingskeys.h"

void makeDirStructure();
void setLanguage(QApplication &app);
void makeDir(const QString &name);

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
    Lvk::Cmn::Settings settings;
    QString lang = settings.value(SETTING_APP_LANGUAGE, QString("es_AR")).toString();

    QTranslator *translator = new QTranslator();
    translator->load("./lang/lang_" + lang + ".qm");
    app.installTranslator(translator);
}

//--------------------------------------------------------------------------------------------------

void makeDirStructure()
{
    Lvk::Cmn::Settings settings;

    makeDir(settings.value(SETTING_LOGS_PATH).toString());
    makeDir(settings.value(SETTING_DATA_PATH).toString());
}

//--------------------------------------------------------------------------------------------------

void makeDir(const QString &name)
{
    QDir qdir;

    if (!qdir.exists(name)) {
        if (!qdir.mkpath(name)) {
            qCritical() << "Critical: Cannot create path " << name;
            exit(1);
        }
    }
}

