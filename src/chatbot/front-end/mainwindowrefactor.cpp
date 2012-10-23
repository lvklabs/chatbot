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

#include "mainwindowrefactor.h"
#include "ui_mainwindow.h"
#include "back-end/appfacade.h"
#include "common/settings.h"
#include "common/globalstrings.h"
#include "common/settingskeys.h"


//--------------------------------------------------------------------------------------------------
// MainWindowRefactor
//--------------------------------------------------------------------------------------------------

Lvk::FE::MainWindowRefactor::MainWindowRefactor(Ui::MainWindow *ui, BE::AppFacade *appFacade)
    : ui(ui), m_appFacade(appFacade), m_mode(NullMode), m_tabsLayout(NullLayout)
{
}

//--------------------------------------------------------------------------------------------------

Lvk::FE::UiMode Lvk::FE::MainWindowRefactor::uiMode()
{
    return m_mode;
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindowRefactor::setUiMode(FE::UiMode mode)
{
    updateTabsLayout(mode);

    // Set up tabs ///////////////////////////////////////

    switch (mode) {

    case NullMode:
        // nothing to do
        break;

     case WelcomeTabUiMode:
        ui->mainTabWidget->setCurrentWidget(ui->welcomeTab);
        ui->openLastChatbotButton->setVisible(hasLastFile());
        break;

    case EditRuleUiMode:
        ui->mainTabWidget->setCurrentWidget(ui->teachTab);
        break;

    case ConnectionUiMode:
    case ChangeAccountUiMode:
        ui->mainTabWidget->setCurrentWidget(ui->connectTab);
        break;

    case VerifyAccountUiMode:
        ui->mainTabWidget->setCurrentWidget(ui->welcomeTab);
        break;
    }

    m_mode = mode;
}

//--------------------------------------------------------------------------------------------------

Lvk::FE::UiTabsLayout Lvk::FE::MainWindowRefactor::uiTabsLayout()
{
    return m_tabsLayout;
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindowRefactor::updateTabsLayout(UiMode mode)
{
    FE::UiTabsLayout visibleTabs = NullLayout;

    switch (mode) {
    case WelcomeTabUiMode:
        visibleTabs = WelcomeTabsLayout;
        break;
    case VerifyAccountUiMode:
        visibleTabs = VerifyAccountTabsLayout;
        break;
    default:
        visibleTabs = TeachTabsLayout;
        break;
    }

    if (m_tabsLayout != visibleTabs) {
        m_tabsLayout = visibleTabs;

        switch (mode) {

        case WelcomeTabUiMode:
            ui->actionSave->setEnabled(false);
            ui->actionSaveAs->setEnabled(false);
            ui->actionImport->setEnabled(false);
            ui->actionExport->setEnabled(false);
            ui->actionOptions->setEnabled(false);

            ui->mainTabWidget->addTab(ui->welcomeTab, QObject::tr("Init"));
            ui->mainTabWidget->removePage(ui->teachTab);
            ui->mainTabWidget->removePage(ui->testTab);
            ui->mainTabWidget->removePage(ui->connectTab);
            ui->mainTabWidget->removePage(ui->conversationsTab);
            ui->mainTabWidget->removePage(ui->scoreTab);
            ui->mainTabWidget->removePage(ui->verificationTab);
            break;

        case VerifyAccountUiMode:
            ui->actionSave->setEnabled(false);
            ui->actionSaveAs->setEnabled(false);
            ui->actionImport->setEnabled(false);
            ui->actionExport->setEnabled(false);
            ui->actionOptions->setEnabled(false);

            ui->mainTabWidget->removePage(ui->welcomeTab);
            ui->mainTabWidget->removePage(ui->testTab);
            ui->mainTabWidget->removePage(ui->connectTab);
            ui->mainTabWidget->removePage(ui->teachTab);
            ui->mainTabWidget->removePage(ui->conversationsTab);
            ui->mainTabWidget->removePage(ui->scoreTab);
            ui->mainTabWidget->addTab(ui->verificationTab, QObject::tr("Verify account"));
            break;

        default:
            ui->actionSave->setEnabled(true);
            ui->actionSaveAs->setEnabled(true);
            ui->actionImport->setEnabled(true);
            ui->actionExport->setEnabled(true);
            ui->actionOptions->setEnabled(true);

            ui->mainTabWidget->removePage(ui->welcomeTab);
            ui->mainTabWidget->addTab(ui->teachTab,         QObject::tr("Teach"));
            ui->mainTabWidget->addTab(ui->testTab,          QObject::tr("Test your chatbot"));
            ui->mainTabWidget->addTab(ui->connectTab,       QObject::tr("Connection"));
            ui->mainTabWidget->addTab(ui->conversationsTab, QObject::tr("Conversations"));
        #ifdef DA_CONTEST
            ui->mainTabWidget->addTab(ui->scoreTab,         QObject::tr("Score"));
        #endif
            ui->mainTabWidget->removePage(ui->verificationTab);
            break;
        }

        // Tabs icons must be reseted
        setConnectionTabIcon(m_appFacade->isConnected());
    }
}


//--------------------------------------------------------------------------------------------------

inline bool Lvk::FE::MainWindowRefactor::hasLastFile()
{
    return Cmn::Settings(Cmn::Settings::UserScope()).contains(SETTING_LAST_FILE);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindowRefactor::setConnectionTabIcon(bool connected)
{
    int connectTabIdx = ui->mainTabWidget->indexOf(ui->connectTab);
    QIcon icon = connected ? QIcon(STATUS_CONNECTED_ICON_FILE) : QIcon(STATUS_DISCONNEC_ICON_FILE);

    ui->mainTabWidget->setTabIcon(connectTabIdx, icon);
}


