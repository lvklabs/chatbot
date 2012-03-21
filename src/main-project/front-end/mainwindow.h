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

#ifndef LVK_FE_MAINWINDOW_H
#define LVK_FE_MAINWINDOW_H

#include <QMainWindow>

#include "coreapp.h"
#include "conversation.h"

class TestMainWindow;

namespace Ui
{
    class MainWindow;
}

class QItemSelectionModel;
class QItemSelection;

namespace Lvk
{

namespace BE
{
    class Rule;
}

namespace FE
{

class RuleTreeModel;


/**
 * \brief This class is the application main window
 */

class MainWindow : public QMainWindow
{
    Q_OBJECT

    friend class ::TestMainWindow;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void clear();

    BE::Rule *addCategory(const QString &name);
    BE::Rule *addRule(const QString &name, BE::Rule *category);

    enum UiMode {
        RuleSelectionEmptyUiMode,
        EditCategoryUiMode,
        EditRuleUiMode,
        EditEvasivesUiMode,
        ChatDisconnectedUiMode,
        ChatConnectingUiMode,
        ChatConnectionFailedUiMode,
        ChatConnectionOkUiMode
    };

    void setUiMode(UiMode mode);

    virtual bool eventFilter(QObject *object, QEvent *event);

protected:
    virtual void closeEvent(QCloseEvent *event);

private:
    MainWindow(MainWindow&);
    MainWindow& operator=(MainWindow&);

    Ui::MainWindow *ui;
    BE::CoreApp *m_coreApp;
    RuleTreeModel *m_ruleTreeModel;
    QItemSelectionModel *m_ruleTreeSelectionModel;

    enum {
        DisconnectedFromChat,
        ConnectingToChat,
        ConnectedToChat,
        ConnectionError
    } m_connectionStatus;

    void initCoreAndModels();
    void connectSignals();

    void selectFirstRule();

    void selectRule(BE::Rule *rule);
    BE::Rule *selectedRule();
    BE::Rule *evasiveRule();
    BE::Rule *rootRule();

    void highlightMatchedRules(const BE::CoreApp::MatchList &matches);

private slots:
    void onAddCategoryButtonClicked();
    void onAddRuleButtonClicked();
    void onRemoveButtonClicked();

    void onRuleInputEdited(const QString &ruleInput);
    void onRuleInputEditingFinished();
    void onRuleOutputEditingFinished();
    void onRuleSelectionChanged(const QItemSelection &selected,
                                    const QItemSelection &deselected);
    void onTestInputTextEntered();

    void onConnectButtonPressed();
    void onConnectionOk();
    void onConnectionError(int err);
    void onDisconnection();

    void onNewChatConversation(const BE::Conversation::Entry &entry);
};

} // namespace Lvk

} // namespace FE


#endif // LVK_FE_MAINWINDOW_H
