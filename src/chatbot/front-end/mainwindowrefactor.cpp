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

            addTab(ui->welcomeTab, QObject::tr("Init"));
            removeTab(ui->teachTab);
            removeTab(ui->testTab);
            removeTab(ui->connectTab);
            removeTab(ui->conversationsTab);
            removeTab(ui->scoreTab);
            removeTab(ui->verificationTab);
            removeTab(ui->clueTab);
            break;

        case VerifyAccountUiMode:
            ui->actionSave->setEnabled(false);
            ui->actionSaveAs->setEnabled(false);
            ui->actionImport->setEnabled(false);
            ui->actionExport->setEnabled(false);
            ui->actionOptions->setEnabled(false);

            removeTab(ui->welcomeTab);
            removeTab(ui->testTab);
            removeTab(ui->connectTab);
            removeTab(ui->teachTab);
            removeTab(ui->conversationsTab);
            removeTab(ui->scoreTab);
            removeTab(ui->clueTab);
            addTab(ui->verificationTab, QObject::tr("Verify account"));
            break;

        default:
            ui->actionSave->setEnabled(true);
            ui->actionSaveAs->setEnabled(true);
            ui->actionImport->setEnabled(true);
            ui->actionExport->setEnabled(true);
            ui->actionOptions->setEnabled(true);

            removeTab(ui->welcomeTab);
            addTab(ui->teachTab,         QObject::tr("T&each"));
            addTab(ui->testTab,          QObject::tr("&Test your chatbot"));
            addTab(ui->connectTab,       QObject::tr("&Connection"));
            addTab(ui->conversationsTab, QObject::tr("C&onversations"));
        #ifdef DA_CONTEST
            //addTab(ui->scoreTab,         QObject::tr("&Score"));
        #endif
            addTab(ui->clueTab,          QObject::tr("C&lue"));
            removeTab(ui->verificationTab);
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

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindowRefactor::removeTab(QWidget *w)
{
    ui->mainTabWidget->removeTab(ui->mainTabWidget->indexOf(w));
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindowRefactor::addTab(QWidget *w, const QString &label)
{
    ui->mainTabWidget->addTab(w, label);
}

