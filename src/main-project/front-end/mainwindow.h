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
    class CoreApp;
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

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void clear();

    BE::Rule *addCategory(const QString &name);
    BE::Rule *addRule(const QString &name, BE::Rule *category);

    enum UiMode { DefaultUiMode, EditCategoryUiMode, EditRuleUiMode };

    void setUiMode(UiMode mode);

private:
    Ui::MainWindow *ui;
    BE::CoreApp *m_coreApp;
    RuleTreeModel *m_ruleTreeModel;
    QItemSelectionModel *m_ruleTreeSelectionModel;

private slots:
    void addCategoryWithInputDialog();
    void addRuleWithInputDialog();
    void removeSelectedItem();
    void handleRuleInputEdited(const QString &ruleInput);
    void handleRuleSelectionChanged(const QItemSelection &selected,
                                    const QItemSelection &deselected);
    void testInputTextEntered();
    void appendTestConversation(const QString &input, const QString &response, bool match);
    void highlightMatchedRules(const QList<BE::Rule *> &matched);

};

} // namespace Lvk

} // namespace FE


#endif // LVK_FE_MAINWINDOW_H
