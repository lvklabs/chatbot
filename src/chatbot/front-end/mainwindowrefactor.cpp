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
#include "common/globalstrings.h"
#include "common/settings.h"
#include "common/settingskeys.h"

#include <QIcon>
#include <QPixmap>
#include <QString>

//--------------------------------------------------------------------------------------------------
// MainWindowRefactor
//--------------------------------------------------------------------------------------------------

Lvk::FE::MainWindowRefactor::MainWindowRefactor(Ui::MainWindow *ui, BE::AppFacade *appFacade)
    : ui(ui), m_appFacade(appFacade), m_mode(NullMode), m_tabsLayout(NullLayout),
      m_fbIcon(FB_ICON_FILE), m_gIcon(GMAIL_ICON_FILE)
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
    updateTabsIcons(mode);

    // Set up tabs ///////////////////////////////////////

    switch (mode) {

    case NullMode:
        // nothing to do
        break;

    // init tab //

    case WelcomeTabUiMode:
        ui->mainTabWidget->setCurrentWidget(ui->welcomeTab);
        ui->initStackWidget->setCurrentIndex(0);
        ui->openLastChatbotButton->setVisible(hasLastFile());
        break;

    // Edit rules tab //
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // TODO create widget to edit rules to simplify this mess!
    ////////////////////////////////////////////////////////////////////////////////////////////////

    case RuleSelectionEmptyUiMode:
        ui->mainTabWidget->setCurrentWidget(ui->teachTab);
        ui->categoryNameLabel->setVisible(false);
        ui->categoryNameTextEdit->setVisible(false);
        ui->ruleInputWidget->setVisible(false);
        ui->ruleOutputWidget->setVisible(false);
        ui->chatbotRepliesLabel->setVisible(false);
        ui->teachRuleButton->setVisible(false);
        ui->undoRuleButton->setVisible(false);
        ui->teachRuleButton->setEnabled(false);
        ui->undoRuleButton->setEnabled(false);
        break;

    case EditCategoryUiMode:
        ui->mainTabWidget->setCurrentWidget(ui->teachTab);
        ui->ruleOutputWidget->setMaximumHeight(16777215);
        ui->categoryNameLabel->setVisible(true);
        ui->categoryNameTextEdit->setVisible(true);
        ui->ruleInputWidget->setVisible(false);
        ui->ruleOutputWidget->setVisible(false);
        ui->chatbotRepliesLabel->setVisible(false);
        ui->teachRuleButton->setVisible(true);
        ui->undoRuleButton->setVisible(true);
        ui->teachRuleButton->setEnabled(false);
        ui->undoRuleButton->setEnabled(false);
        ui->teachRuleButton->setText(QObject::tr("Change name"));
        break;

    case EditRuleUiMode:
        ui->mainTabWidget->setCurrentWidget(ui->teachTab);
        ui->ruleOutputWidget->setMaximumHeight(16777215);
        ui->categoryNameLabel->setVisible(false);
        ui->categoryNameTextEdit->setVisible(false);
        ui->ruleInputWidget->setVisible(true);
        ui->ruleOutputWidget->setVisible(true);
        ui->chatbotRepliesLabel->setVisible(true);
        ui->teachRuleButton->setVisible(true);
        ui->undoRuleButton->setVisible(true);
        ui->teachRuleButton->setEnabled(false);
        ui->undoRuleButton->setEnabled(false);
        ui->chatbotRepliesLabel->setText(QObject::tr("Chatbot replies:"));
        ui->teachRuleButton->setText(QObject::tr("Teach rule to the chatbot"));
        break;

    case EditEvasivesUiMode:
        ui->mainTabWidget->setCurrentWidget(ui->teachTab);
        ui->ruleOutputWidget->setMaximumHeight(150);
        ui->categoryNameLabel->setVisible(false);
        ui->categoryNameTextEdit->setVisible(false);
        ui->ruleInputWidget->setVisible(false);
        ui->ruleOutputWidget->setVisible(true);
        ui->chatbotRepliesLabel->setVisible(true);
        ui->teachRuleButton->setVisible(true);
        ui->undoRuleButton->setVisible(true);
        ui->teachRuleButton->setEnabled(false);
        ui->undoRuleButton->setEnabled(false);
        ui->chatbotRepliesLabel->setText(QObject::tr("If chatbot does not understand,"
                                                     " it replies:"));
        ui->teachRuleButton->setText(QObject::tr("Teach rule to the chatbot"));
        break;

    // Chat connection tab //

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // TODO create a connection widget to simplify this mess!
    ////////////////////////////////////////////////////////////////////////////////////////////////

    case ChatDisconnectedUiMode:
        ui->mainTabWidget->setCurrentWidget(ui->connectTab);
        ui->curUsernameLabel->setText(username().isEmpty() ? QObject::tr("(None)") : username());
        ui->chatTypeIcon->setPixmap(chatIcon());
        ui->connectToChatStackWidget->setCurrentIndex(0);
        ui->passwordText->setEnabled(true);
        ui->changeAccountButton->setEnabled(true);
        ui->connectButton->setText(QObject::tr("Connect"));
        ui->connectionProgressBar->setVisible(false);
        ui->connectionStatusLabel->setText(QObject::tr("Disconnected"));
        ui->connectionStatusLabel->setStyleSheet("color:gray");
        ui->rosterWidget->clear();
        // If verification was skipped
        ui->connectButton->setEnabled(!username().isEmpty());
        ui->passwordText->setEnabled(!username().isEmpty());
        //ui->chatTypeIcon->setVisible(!username().isEmpty());
        ui->chatTypeIcon->setVisible(false);
        break;

    case ChatConnectingUiMode:
        ui->mainTabWidget->setCurrentWidget(ui->connectTab);
        ui->connectToChatStackWidget->setCurrentIndex(0);
        ui->passwordText->setEnabled(false);
        ui->changeAccountButton->setEnabled(false);
        ui->connectButton->setText(QObject::tr("Cancel connection"));
        ui->connectionProgressBar->setVisible(true);
        ui->connectionStatusLabel->setText(QObject::tr("Connecting..."));
        ui->connectionStatusLabel->setStyleSheet("");
        break;

    case ChatConnectionFailedUiMode:
        ui->mainTabWidget->setCurrentWidget(ui->connectTab);
        ui->connectToChatStackWidget->setCurrentIndex(0);
        ui->passwordText->setEnabled(true);
        ui->changeAccountButton->setEnabled(true);
        ui->connectButton->setText(QObject::tr("Connect"));
        ui->connectionProgressBar->setVisible(false);
        ui->connectionStatusLabel->setText(QObject::tr("Connection error. Please verify your"
                                                       " username and password."));
        ui->connectionStatusLabel->setStyleSheet("color:red");
        break;

    case ChatConnectionSSLFailedUiMode:
        ui->mainTabWidget->setCurrentWidget(ui->connectTab);
        ui->connectToChatStackWidget->setCurrentIndex(0);
        ui->passwordText->setEnabled(true);
        ui->changeAccountButton->setEnabled(true);
        ui->connectButton->setText(QObject::tr("Connect"));
        ui->connectionProgressBar->setVisible(false);
        ui->connectionStatusLabel->setText(QObject::tr("Connection error. You system does not"
                                                       " support secure connections."));
        ui->connectionStatusLabel->setStyleSheet("color:red");
        break;

    case ChatConnectionOkUiMode:
        ui->mainTabWidget->setCurrentWidget(ui->connectTab);
        ui->connectToChatStackWidget->setCurrentIndex(1);
        ui->disconnectButton->setText(QObject::tr("Disconnect ") + username());
        ui->disconnectButton->setIcon(chatIcon());
        // Not visible anymore:
        ui->passwordText->setEnabled(false);
        ui->connectButton->setText(QObject::tr("Disconnect"));
        ui->connectionProgressBar->setVisible(false);
        ui->connectionStatusLabel->setText(QObject::tr("Connection sucessful!"));
        ui->connectionStatusLabel->setStyleSheet("color:green");
        break;

    case ChangeAccountUiMode:
        ui->mainTabWidget->setCurrentWidget(ui->connectTab);
        ui->connectToChatStackWidget->setCurrentIndex(2);
        ui->verifyExplanationLabel->setText(QObject::tr("Please insert your username and password"
                                                        " and press \"Verify account\" button."));
        ui->verifyAccountButton->setEnabled(true);
        ui->usernameText_v->setEnabled(true);
        ui->passwordText_v->setEnabled(true);
        ui->fbChatRadio_v->setEnabled(true);
        ui->gtalkChatRadio_v->setEnabled(true);
        ui->connectionProgressBar_v->setVisible(false);
        ui->connectionStatusLabel_v->setVisible(false);
        ui->verifyLaterButton->setVisible(false);
        ui->passwordText->setText("");
        break;

    case VerifyAccountUiMode:
        ui->mainTabWidget->setCurrentWidget(ui->welcomeTab);
        ui->connectToChatStackWidget->setCurrentIndex(2);
        ui->verifyExplanationLabel->setText(QObject::tr("To create a chatbot you need a Facebook"
                                                        " or Gmail account.\nPlease insert your"
                                                        " username and password and press "
                                                        "\"Verify account\" button."));
        ui->verifyAccountButton->setEnabled(true);
        ui->usernameText_v->setEnabled(true);
        ui->passwordText_v->setEnabled(true);
        ui->fbChatRadio_v->setEnabled(true);
        ui->gtalkChatRadio_v->setEnabled(true);
        ui->verifyLaterButton->setEnabled(true);
        ui->connectionProgressBar_v->setVisible(false);
        ui->connectionStatusLabel_v->setVisible(false);
        ui->verifyLaterButton->setVisible(true);
        ui->passwordText->setText("");
        break;

    case VerifyingAccountUiMode:
        ui->connectToChatStackWidget->setCurrentIndex(2);
        ui->verifyAccountButton->setEnabled(false);
        ui->usernameText_v->setEnabled(false);
        ui->passwordText_v->setEnabled(false);
        ui->fbChatRadio_v->setEnabled(false);
        ui->gtalkChatRadio_v->setEnabled(false);
        ui->verifyLaterButton->setEnabled(false);
        ui->connectionProgressBar_v->setVisible(true);
        ui->connectionStatusLabel_v->setVisible(true);
        break;

    case VerifyAccountFailedUiMode:
        ui->connectToChatStackWidget->setCurrentIndex(2);
        ui->verifyAccountButton->setEnabled(true);
        ui->usernameText_v->setEnabled(true);
        ui->passwordText_v->setEnabled(true);
        ui->fbChatRadio_v->setEnabled(true);
        ui->gtalkChatRadio_v->setEnabled(true);
        ui->verifyLaterButton->setEnabled(true);
        ui->connectionProgressBar_v->setVisible(false);
        ui->connectionStatusLabel_v->setVisible(false);
        break;
    }

    m_mode = mode;
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindowRefactor::updateTabsIcons(UiMode mode)
{
    int connectTabIdx = ui->mainTabWidget->indexOf(ui->connectTab);

    switch (mode) {
    case ChatDisconnectedUiMode:
    case ChatConnectingUiMode:
    case ChatConnectionFailedUiMode:
    case ChatConnectionSSLFailedUiMode:
    case ChangeAccountUiMode:
        ui->mainTabWidget->setTabIcon(connectTabIdx, QIcon(STATUS_DISCONNEC_ICON_FILE));
        break;
    case ChatConnectionOkUiMode:
        ui->mainTabWidget->setTabIcon(connectTabIdx, QIcon(STATUS_CONNECTED_ICON_FILE));
        break;
    case VerifyAccountUiMode:
    case VerifyingAccountUiMode:
    case VerifyAccountFailedUiMode:
        ui->mainTabWidget->setTabIcon(connectTabIdx, QIcon());
        break;
    default:
        // nothing to do
        break;
    }
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
    case VerifyingAccountUiMode:
    case VerifyAccountFailedUiMode:
        visibleTabs = m_tabsLayout; // Keep layout!
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

            ui->welcomeTab->setVisible(true);
            ui->teachTab->setVisible(false);
            ui->testTab->setVisible(false);
            ui->connectTab->setVisible(false);
            ui->conversationsTab->setVisible(false);
            ui->scoreTab->setVisible(false);

            ui->mainTabWidget->addTab(ui->welcomeTab, QObject::tr("Init"));
            ui->mainTabWidget->removePage(ui->teachTab);
            ui->mainTabWidget->removePage(ui->testTab);
            ui->mainTabWidget->removePage(ui->connectTab);
            ui->mainTabWidget->removePage(ui->conversationsTab);
            ui->mainTabWidget->removePage(ui->scoreTab);
            break;

        case VerifyAccountUiMode:
            ui->actionSave->setEnabled(false);
            ui->actionSaveAs->setEnabled(false);
            ui->actionImport->setEnabled(false);
            ui->actionExport->setEnabled(false);
            ui->actionOptions->setEnabled(false);

            ui->welcomeTab->setVisible(false);
            ui->teachTab->setVisible(false);
            ui->testTab->setVisible(false);
            ui->connectTab->setVisible(true);
            ui->conversationsTab->setVisible(false);
            ui->scoreTab->setVisible(false);

            ui->mainTabWidget->addTab(ui->connectTab, QObject::tr("Verify account"));
            ui->mainTabWidget->removePage(ui->welcomeTab);
            ui->mainTabWidget->removePage(ui->testTab);
            ui->mainTabWidget->removePage(ui->teachTab);
            ui->mainTabWidget->removePage(ui->conversationsTab);
            ui->mainTabWidget->removePage(ui->scoreTab);
            break;

        case VerifyingAccountUiMode:
        case VerifyAccountFailedUiMode:
            // Nothing to do
            break;

        default:
            ui->actionSave->setEnabled(true);
            ui->actionSaveAs->setEnabled(true);
            ui->actionImport->setEnabled(true);
            ui->actionExport->setEnabled(true);
            ui->actionOptions->setEnabled(true);

            ui->welcomeTab->setVisible(false);
            ui->teachTab->setVisible(true);
            ui->testTab->setVisible(true);
            ui->connectTab->setVisible(true);
            ui->conversationsTab->setVisible(true);
        #ifdef DA_CONTEST
            ui->scoreTab->setVisible(true);
        #endif

            ui->mainTabWidget->removePage(ui->welcomeTab);
            ui->mainTabWidget->addTab(ui->teachTab,         QObject::tr("Teach"));
            ui->mainTabWidget->addTab(ui->testTab,          QObject::tr("Test your chatbot"));
            ui->mainTabWidget->addTab(ui->connectTab,       QObject::tr("Connection"));
            ui->mainTabWidget->addTab(ui->conversationsTab, QObject::tr("Conversations"));
        #ifdef DA_CONTEST
            ui->mainTabWidget->addTab(ui->scoreTab,         QObject::tr("Score"));
        #endif
            break;
        }
    }
}

//--------------------------------------------------------------------------------------------------

inline bool Lvk::FE::MainWindowRefactor::hasLastFile()
{
    return Cmn::Settings(Cmn::Settings::UserScope()).contains(SETTING_LAST_FILE);
}

//--------------------------------------------------------------------------------------------------

inline QPixmap Lvk::FE::MainWindowRefactor::chatIcon()
{
    return m_appFacade->chatType() == BE::FbChat ? m_fbIcon : m_gIcon;
}

//--------------------------------------------------------------------------------------------------

inline QString Lvk::FE::MainWindowRefactor::username()
{
    return m_appFacade->username();
}

