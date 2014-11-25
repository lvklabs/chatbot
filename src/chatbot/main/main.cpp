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
#include <iostream>

#include "main/windowbootstrap.h"
#include "common/version.h"
#include "common/settings.h"
#include "common/settingskeys.h"
#include "common/logger.h"
#include "common/crashhandler.h"

#ifdef DA_CONTEST
# include "da-clue/batchanalyzer.h"
#endif // DA_CONTEST

struct CmdLineOptions
{
    bool valid;
    QtMsgType verboseLevel;
    QString chatbotFilename;
    bool isBatchMode;
    QString batchTarget;
};

void getCmdLineOptions(CmdLineOptions &opt);
void makeDirStructure();
void showSyntax();
void setLanguage();
void makeDir(const QString &name);
void showWindow(int argc, char *argv[]);


//--------------------------------------------------------------------------------------------------
// main
//--------------------------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    QApplication::setOrganizationName(ORGANIZATION_NAME);
    QApplication::setOrganizationDomain(ORGANIZATION_DOMAIN);
    QApplication::setApplicationName(APP_NAME);

    QApplication app(argc, argv);

    CmdLineOptions opt;
    getCmdLineOptions(opt);

    QDir::setCurrent(QApplication::applicationDirPath());

    makeDirStructure();

    Lvk::Cmn::Logger::setVerboseLevel(opt.verboseLevel);
    Lvk::Cmn::Logger::init();

    setLanguage();

    int exitCode = 0;

    if (opt.valid) {
        if (opt.isBatchMode) {
#ifdef DA_CONTEST
            exitCode = Lvk::Clue::BatchAnalyzer().exec(opt.batchTarget);
#endif // DA_CONTEST
        } else {
            Lvk::Cmn::CrashHandler::init();
            WindowBootstrap wb(opt.chatbotFilename);
            exitCode = app.exec();
        }
    } else {
        showSyntax();
        exitCode = 1;
    }

    return exitCode;
}

//--------------------------------------------------------------------------------------------------
// Helpers
//--------------------------------------------------------------------------------------------------

void getCmdLineOptions(CmdLineOptions & opt)
{
    opt.valid = true;
    opt.verboseLevel = QtWarningMsg;
    opt.isBatchMode = false;

    QStringList args = QApplication::arguments();

    for (int i = 1; i < args.size() && opt.valid; ++i) {
        QString arg = args[i];
        if (arg.startsWith("--verbose=")) {
            QStringList tokens = arg.split("=");
            opt.verboseLevel = static_cast<QtMsgType>(QtFatalMsg - tokens[1].toInt(&opt.valid));
#ifdef DA_CONTEST
        } else if (arg == "--batch-mode") {
            ++i;
            if (i < args.size()) {
                opt.isBatchMode = true;
                opt.batchTarget = args[i];
            } else {
                opt.valid = false;
            }
#endif // DA_CONTEST
        } else if (!arg.startsWith("-")) {
            opt.chatbotFilename = arg;
        } else {
            qWarning() << QObject::tr("Warning: Unknown option") << arg;
        }
    }
}

//--------------------------------------------------------------------------------------------------

void showSyntax()
{
    QString appname = QApplication::arguments().first();

    std::cerr << QObject::tr("Error: Invalid command line arguments.").toUtf8().data() << std::endl;
    std::cout << QObject::tr("Syntax: ").toUtf8().data() << std::endl;
    std::cout << QObject::tr("   %1 [chatbot_file]").arg(appname).toUtf8().data() << std::endl;
#ifdef DA_CONTEST
    std::cout << QObject::tr("   %1 --batch-mode <dir> | <chatbot_file>").arg(appname).toUtf8()
                 .data() << std::endl;
#endif // DA_CONTEST
}

//--------------------------------------------------------------------------------------------------

void setLanguage()
{
    qDebug() << "Setting app language...";

    Lvk::Cmn::Settings settings;

    QString lang = settings.value(SETTING_APP_LANGUAGE).toString();
    QString langPath = settings.value(SETTING_LANG_PATH).toString();

    if (!langPath.endsWith(QDir::separator())) {
        langPath.append(QDir::separator());
    }

    qDebug() << "Language:" << lang ;
    qDebug() << "Language path:" << langPath;

    /* Qt common strings */
    QTranslator *qtTranslator = new QTranslator();
    qtTranslator->load(langPath + "qt_" + lang + ".qm");
    QApplication::installTranslator(qtTranslator);

    /* Chatbot specific strings */
    QTranslator *chatbotTranslator = new QTranslator();
    chatbotTranslator->load(langPath + "chatbot2_" + lang + ".qm");
    QApplication::installTranslator(chatbotTranslator);
}

//--------------------------------------------------------------------------------------------------

void makeDirStructure()
{
    Lvk::Cmn::Settings settings;

    makeDir(settings.value(SETTING_LOGS_PATH).toString());
    makeDir(settings.value(SETTING_DATA_PATH).toString());
    makeDir(settings.value(SETTING_LANG_PATH).toString());
#ifdef DA_CONTEST
    makeDir(settings.value(SETTING_CLUE_PATH).toString());
#endif
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

