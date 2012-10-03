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

#ifndef LVK_FE_MAINWINDOWREFACTOR_H
#define LVK_FE_MAINWINDOWREFACTOR_H

#include <QPixmap>

namespace Ui
{
    class MainWindow;
}

namespace Lvk
{

namespace BE
{
    class AppFacade;
}

/// \addtogroup Lvk
/// @{

namespace FE
{

/// \ingroup Lvk
/// \addtogroup FE
/// @{

/**
 * Main window UI Modes
 */
enum UiMode
{
    NullMode,
    WelcomeTabUiMode,
    VerifyAccountUiMode,
    VerifyingAccountUiMode,
    VerifyAccountFailedUiMode,
    ChangeAccountUiMode,
    RuleSelectionEmptyUiMode,
    EditCategoryUiMode,
    EditRuleUiMode,
    EditEvasivesUiMode,
    ChatDisconnectedUiMode,
    ChatConnectingUiMode,
    ChatConnectionFailedUiMode,
    ChatConnectionSSLFailedUiMode,
    ChatConnectionOkUiMode
};

/**
 * Main window Tabs layout
 */
enum UiTabsLayout
{
    NullLayout,
    WelcomeTabsLayout,
    VerifyAccountTabsLayout,
    TeachTabsLayout
};

/**
 * \brief The MainWindowRefactor class refacts a MainWindow UI in different modes.
 * The mode must be one of FE::UiMode. The class requires a AppFacade object to know some context
 * information such as username, chatbot type, among others.
 */
class MainWindowRefactor
{
public:
    /**
     * Constructs a MainWindowRefactor object with \a ui and \a appFacade. The object does not
     * own the given pointers, so they are not freed on destruction.
     */
    MainWindowRefactor(Ui::MainWindow *ui, BE::AppFacade  *appFacade);

    /**
     * Sets \a mode as the current UI mode
     */
    void setUiMode(UiMode mode);

    /**
     * Returns the current UI mode
     */
    UiMode uiMode();

    /**
     * Returns the current tabs layout
     */
    UiTabsLayout uiTabsLayout();

private:

    Ui::MainWindow *ui;
    BE::AppFacade  *m_appFacade;
    UiMode          m_mode;
    UiTabsLayout    m_tabsLayout;
    QPixmap         m_fbIcon;
    QPixmap         m_gIcon;

    void updateTabsLayout(UiMode mode);
    void updateTabsIcons(UiMode mode);

    inline bool hasLastFile();
    inline QString username();
    inline QPixmap chatIcon();
};

/// @}

} // namespace FE

/// @}

} // namespace Lvk


#endif // LVK_FE_MAINWINDOWREFACTOR_H

