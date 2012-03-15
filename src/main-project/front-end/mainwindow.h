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

private:
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

private slots:
    void addCategoryWithInputDialog();
    void addRuleWithInputDialog();
    void removeSelectedItem();
    void handleRuleInputEdited(const QString &ruleInput);
    void handleRuleInputEditingFinished();
    void handleRuleOutputEditingFinished();
    void handleRuleSelectionChanged(const QItemSelection &selected,
                                    const QItemSelection &deselected);
    void testInputTextEntered();
    void highlightMatchedRules(const BE::CoreApp::MatchList &matches);

    void toggleChatConnection();
    void handleConnectionOk();
    void handleConnectionError(int err);
    void handleDisconnection();

};

} // namespace Lvk

} // namespace FE


#endif // LVK_FE_MAINWINDOW_H
