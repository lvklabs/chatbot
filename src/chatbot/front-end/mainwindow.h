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

#ifndef LVK_FE_MAINWINDOW_H
#define LVK_FE_MAINWINDOW_H

#include <QMainWindow>

#include "appfacade.h"
#include "conversation.h"
#include "rule.h"

class TestMainWindow;
class QModelIndex;
class QFile;

namespace Ui
{
    class MainWindow;
}

class QItemSelectionModel;
class QItemSelection;

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace BE
{
    class ConversationWriter;
}

namespace FE
{

/// \ingroup Lvk
/// \addtogroup FE
/// @{

class RuleTreeModel;


/**
 * \brief The MainWindow class provides the application main window.
 */

class MainWindow : public QMainWindow
{
    Q_OBJECT

    friend class ::TestMainWindow;

public:

    /**
     * Constructs a MainWindow with \a parent.
     */
    explicit MainWindow(QWidget *parent = 0);

    /**
     * Destroys the window.
     */
    ~MainWindow();

    /**
     * Clears the content of the window including rules, test chats, chat history and disconnects
     * from chat
     */
    void clear();

protected:
    virtual void closeEvent(QCloseEvent *event);

    virtual bool eventFilter(QObject *object, QEvent *event);

private:
    MainWindow(MainWindow&);
    MainWindow& operator=(MainWindow&);

    enum UiTabsLayout {
        NullLayout,
        WelcomeTabsLayout,
        VerifyAccountTabsLayout,
        TeachTabsLayout
    };

    enum UiMode {
        WelcomeTabUiMode,
        VerifyAccountUiMode,
        VerifyAccountConnectingUiMode,
        VerifyAccountFailedUiMode,
        RuleSelectionEmptyUiMode,
        EditCategoryUiMode,
        EditRuleUiMode,
        EditEvasivesUiMode,
        ChatDisconnectedUiMode,
        ChatConnectingUiMode,
        ChatConnectionFailedUiMode,
        ChatConnectionSSLFailedUiMode,
        ChatConnectionOkUiMode,
        ChangeAccountUiMode,
        ChangeAccountConnectingUiMode,
        ChangeAccountFailedUiMode
    };

    enum ConnectionStatus {
        DisconnectedFromChat,
        ConnectingToChat,
        ConnectedToChat,
        ConnectionError
    };

    Ui::MainWindow          *ui;
    BE::AppFacade           *m_appFacade;
    RuleTreeModel           *m_ruleTreeModel;
    QItemSelectionModel     *m_ruleTreeSelectionModel;
    bool                     m_ruleEdited;
    BE::Rule                 m_ruleBackup;
    BE::ConversationWriter  *m_testConversationLog;
    QString                  m_filename;
    QString                  m_fileUsername;
    BE::AppFacade::ChatType  m_fileChatType;
    QString                  m_lastFilename;
    UiTabsLayout             m_tabsLayout;
    ConnectionStatus         m_connectionStatus;

    BE::Rule *addCategory(const QString &name);
    BE::Rule *addRule(const QString &name, BE::Rule *category);

    BE::Rule *addCategoryWithDialog();
    BE::Rule *addRuleWithDialog();
    bool removeSelectedRuleWithDialog();

    void setUiMode(UiMode mode);
    void updateTabsLayout(UiMode mode);

    bool initCoreAndModelsWithFile(const QString &filename);
    void connectSignals();

    void loadAllSettings();
    void loadMainWindowSettings();
    void loadSplittersSettings();
    void loadChatSettings();

    void saveAllSettings();
    void saveMainWindowSettings();
    void saveSplittersSettings();
    void saveChatSettings();

    void startEditMode();

    void selectFirstRule();

    void selectRule(const BE::Rule *rule);
    BE::Rule *selectedRule();
    BE::Rule *evasivesRule();
    BE::Rule *rootRule();

    void teachRule(BE::Rule *rule);
    void undoRule(BE::Rule *rule);
    void handleRuleEdited(BE::Rule *rule);
    void highlightMatchedRules(const BE::AppFacade::MatchList &matches);

    void undoRuleEdited(BE::Rule *rule);

    void showRuleOnWidget(const BE::Rule *rule);
    void refreshRuleOnWidget();

    QString getRuleDisplayName(const QModelIndex &index) const;
    QString getRuleDisplayName(const BE::Rule *rule) const;

    void updateBlackList();
    BE::AppFacade::ChatType uiChatSelected();

    bool hasUnsavedChanges();
    int showSaveChangesDialog();
    bool saveChanges();
    bool saveAsChanges();
    bool load(const QString &filename);
    void setFilename(const QString &filename);

private slots:
    void onAddCategoryButtonClicked();
    void onAddRuleButtonClicked();
    void onRemoveButtonClicked();

    BE::Rule *getCategoryFromDialog();
    void onTeachFromHistoryWidget(const QString &msg);
    void onRemovedAllHistory();
    void onRemovedHistory(const QDate &date, const QString &username);

    void onRuleInputEdited(const QString &ruleInput);
    void onRuleTargetEdited(const QString &ruleInput);
    //void onRuleInputEditingFinished();
    //void onRuleOutputEditingFinished();
    void onRuleEdited();
    void onRuleSelectionChanged(const QItemSelection &selected,
                                    const QItemSelection &deselected);
    void onTeachButtonPressed();
    void onUndoButtonPressed();
    void onTestInputTextEntered();
    void onClearTestConversationButtonPressed();

    void onVerifyAccountButtonPressed();
    void onVerifyAccountOk(const BE::Roster &roster);
    void onVerifyAccountError(int err);
    void onChangeAccountButtonPressed();
    void onCancelChangeAccountButtonPressed();

    void onConnectButtonPressed();
    void onDisconnectButtonPressed();
    void onConnectionOk();
    void onConnectionError(int err);
    void onDisconnection();
    void onRosterSelectionChanged();

    void onNewChatConversation(const BE::Conversation::Entry &entry);

    void onNewMenuTriggered();
    void onOpenMenuTriggered();
    void onOpenLastFileMenuTriggered();
    void onSaveMenuTriggered();
    void onSaveAsMenuTriggered();
    void onImportMenuTriggered();
    void onExportMenuTriggered();
    void onAboutMenuTriggered();
    void onExitMenuTriggered();

    void onSplitterMoved(int, int);
};

/// @}

} // namespace Lvk

/// @}

} // namespace FE


#endif // LVK_FE_MAINWINDOW_H
