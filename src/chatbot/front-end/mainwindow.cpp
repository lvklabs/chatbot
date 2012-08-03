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

#include "front-end/mainwindow.h"
#include "back-end/appfacade.h"
#include "front-end/ruletreemodel.h"
#include "back-end/rule.h"
#include "ui_mainwindow.h"
#include "front-end/exportdialog.h"
#include "front-end/importdialog.h"
#include "back-end/roster.h"
#include "common/version.h"
#include "common/settings.h"
#include "common/settingskeys.h"
#include "back-end/conversationwriter.h"

#include <QStandardItemModel>
#include <QItemDelegate>
#include <QInputDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QIcon>
#include <QFile>
#include <QDir>
#include <QFileDialog>
#include <QApplication>
#include <QDesktopWidget>
#include <QtDebug>

#define TEST_CONVERSATION_LOG_FILE  "local_tests.log"
#define APP_ICON_FILE               ":/icons/app_icon"

#define FILE_EXTENSION              QString(QObject::tr("crf"))
#define FILE_EXPORT_EXTENSION       QString(QObject::tr("cef"))

#define FB_ICON_FILE                ":/icons/facebook_24x24.png"
#define GMAIL_ICON_FILE             ":/icons/gmail_24x24.png"


//--------------------------------------------------------------------------------------------------
// Non-member helpers
//--------------------------------------------------------------------------------------------------

namespace
{

//--------------------------------------------------------------------------------------------------
// Cannonic representation for chat accounts used to persist some settings

inline QString canonicAccount(const QString &username, Lvk::BE::AppFacade::ChatType type)
{
    return username.trimmed().split("@").at(0) + "@" + QString::number(type);
}

//--------------------------------------------------------------------------------------------------
// Localized file filters for open/save dialogs

inline QString getFileFilters()
{
    return QObject::tr("Chatbot Rule Files") + QString(" (*.") + FILE_EXTENSION + QString(");;")
            + QObject::tr("All files") + QString(" (*.*)");
}

//--------------------------------------------------------------------------------------------------
// Localized export file filters for import/export dialogs

inline QString getFileExportFilters()
{
    return QObject::tr("Chatbot Export Files") + QString(" (*.") + FILE_EXPORT_EXTENSION
            + QString(");;") + QObject::tr("All files") + QString(" (*.*)");
}

//--------------------------------------------------------------------------------------------------
// Save roster to filename

inline bool saveRoster(const Lvk::BE::Roster &roster, const QString &filename)
{
    QFile file(filename);
    if (file.open(QFile::WriteOnly)) {
        QDataStream out(&file);
        out << roster;
        return out.status() == QDataStream::Ok;
    } else {
        qCritical() << "Cannot not save roster in " << filename;
        return false;
    }
}

//--------------------------------------------------------------------------------------------------
// load roster from filename

inline bool loadRoster(Lvk::BE::Roster &roster, const QString &filename)
{
    QFile file(filename);
    if (file.open(QFile::ReadOnly)) {
        QDataStream in(&file);
        in >> roster;
        return in.status() == QDataStream::Ok;
    } else {
        qCritical() << "Cannot not load roster from " << filename;
        return false;
    }
}

//--------------------------------------------------------------------------------------------------
// get persisted roster filename for the give  cannonic account

inline QString rosterFilename(QString account)
{
    Lvk::Cmn::Settings settings;
    QString dataPath = settings.value(SETTING_DATA_PATH).toString();

    return dataPath + QDir::separator() + "roster_" + account + ".dat";
}

//--------------------------------------------------------------------------------------------------
// get persisted black list roster filename for the given cannonic account

inline QString blackRosterFilename(QString account)
{
    Lvk::Cmn::Settings settings;
    QString dataPath = settings.value(SETTING_DATA_PATH).toString();

    return dataPath + QDir::separator() + "black_roster_" + account + ".dat";
}

} // namespace


//--------------------------------------------------------------------------------------------------
// Constructors, destructor and init methods
//--------------------------------------------------------------------------------------------------

Lvk::FE::MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_appFacade(new BE::AppFacade(this)),
    m_ruleTreeModel(0),
    m_ruleEdited(false),
    m_testConversationLog(0),
    m_tabsLayout(NullLayout),
    m_connectionStatus(DisconnectedFromChat)
{
    qDebug() << "Setting up main window...";

    ui->setupUi(this);
    ui->teachTabsplitter->setBackgroundColor(QColor(0,0,0,0));

    Lvk::Cmn::Settings settings;
    m_lastFilename = settings.value(SETTING_LAST_FILE, QString()).toString();

    clear();

    connectSignals();

    ui->testInputText->installEventFilter(this);
    ui->ruleInputWidget->installEventFilter(this);
    ui->ruleOutputWidget->installEventFilter(this);

    setWindowIcon(QIcon(APP_ICON_FILE));

    QString logsPath = settings.value(SETTING_LOGS_PATH).toString();
    QString testConvLogFilename = logsPath + QDir::separator() + TEST_CONVERSATION_LOG_FILE;
    m_testConversationLog = new Lvk::BE::ConversationWriter(testConvLogFilename);

    loadAllSettings();

    qDebug() << "Main window created!";
}

//--------------------------------------------------------------------------------------------------

Lvk::FE::MainWindow::~MainWindow()
{
    delete m_testConversationLog;
    delete m_appFacade;
    delete ui;

    qDebug() << "Main window destroyed cleanly!";
}

//--------------------------------------------------------------------------------------------------


void Lvk::FE::MainWindow::clear()
{
    clear(true);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::clear(bool resetModel)
{
    if (resetModel) {
        initCoreAndModelsWithFile("");
    }

    m_ruleEdited = false;

    // reset active tabs
    ui->mainTabWidget->setCurrentIndex(0);
    ui->rightSideTabWidget->setCurrentIndex(0);

    // train tab widgets
    // TODO clear ui->categoriesTree
    ui->ruleInputWidget->clear();
    ui->ruleOutputWidget->clear();

    // chat tab widgets
    //ui->fbChatRadio->setChecked(true);
    ui->rosterWidget->clear();
    m_connectionStatus = DisconnectedFromChat;
    m_appFacade->disconnectFromChat();

    setUiMode(ChatDisconnectedUiMode);
    //ui->usernameText->clear();
    ui->passwordText->clear();

    // conversation tab widgets
    ui->chatHistory->clear();

    // test tab widgets
    ui->testConversationText->clear();
    ui->testInputText->clear();
    ui->clearTestConversationButton->setEnabled(false);

    // advanced options tab widgets
    ui->rmDupCheckBox->setChecked(true);
    ui->lemmatizerCheckBox->setChecked(true);

    setUiMode(WelcomeTabUiMode);
}

//--------------------------------------------------------------------------------------------------

bool Lvk::FE::MainWindow::initCoreAndModelsWithFile(const QString &filename)
{
    bool success = true;

    setFilename(filename);

    success = m_appFacade->load(filename);

    delete m_ruleTreeModel;

    m_ruleTreeModel = new FE::RuleTreeModel(m_appFacade->rootRule(), this);

    ui->categoriesTree->setModel(m_ruleTreeModel);

    m_ruleTreeSelectionModel = ui->categoriesTree->selectionModel();

    connect(m_ruleTreeSelectionModel,
            SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            SLOT(onRuleSelectionChanged(QItemSelection,QItemSelection)));

    return success;
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::connectSignals()
{
    // Menus

    connect(ui->actionNew,         SIGNAL(triggered()), SLOT(onNewMenuTriggered()));
    connect(ui->actionOpen,        SIGNAL(triggered()), SLOT(onOpenMenuTriggered()));
    connect(ui->actionSave,        SIGNAL(triggered()), SLOT(onSaveMenuTriggered()));
    connect(ui->actionSaveAs,      SIGNAL(triggered()), SLOT(onSaveAsMenuTriggered()));
    connect(ui->actionAbout,       SIGNAL(triggered()), SLOT(onAboutMenuTriggered()));
    connect(ui->actionExit,        SIGNAL(triggered()), SLOT(onExitMenuTriggered()));
    connect(ui->actionImport,      SIGNAL(triggered()), SLOT(onImportMenuTriggered()));
    connect(ui->actionExport,      SIGNAL(triggered()), SLOT(onExportMenuTriggered()));

    // init tab

    connect(ui->openChatbotButton,     SIGNAL(clicked()), SLOT(onOpenMenuTriggered()));
    connect(ui->openLastChatbotButton, SIGNAL(clicked()), SLOT(onOpenLastFileMenuTriggered()));
    connect(ui->createChatbotButton,   SIGNAL(clicked()), SLOT(onNewMenuTriggered()));
    connect(ui->verifyAccountButton,   SIGNAL(clicked()), SLOT(onVerifyAccountButtonPressed()));

    connect(ui->passwordText_v, SIGNAL(returnPressed()), SLOT(onVerifyAccountButtonPressed()));

    connect(m_appFacade, SIGNAL(accountOk(BE::Roster)), SLOT(onVerifyAccountOk(BE::Roster)));
    connect(m_appFacade, SIGNAL(accountError(int)),     SLOT(onVerifyAccountError(int)));

    // Edit rules tabs

    connect(ui->addCategoryButton, SIGNAL(clicked()), SLOT(onAddCategoryButtonClicked()));
    connect(ui->addRuleButton,     SIGNAL(clicked()), SLOT(onAddRuleButtonClicked()));
    connect(ui->rmItemButton,      SIGNAL(clicked()), SLOT(onRemoveButtonClicked()));
    connect(ui->teachRuleButton,   SIGNAL(clicked()), SLOT(onTeachButtonPressed()));
    connect(ui->undoRuleButton,    SIGNAL(clicked()), SLOT(onUndoButtonPressed()));

    connect(ui->ruleInputWidget,   SIGNAL(inputVariantsEdited()),    SLOT(onRuleEdited()));
    connect(ui->ruleOutputWidget,  SIGNAL(outputTextEdited()),       SLOT(onRuleEdited()));

    connect(ui->categoryNameTextEdit, SIGNAL(textEdited(QString)),
            SLOT(onRuleInputEdited(QString)));

    connect(ui->ruleInputWidget, SIGNAL(inputTextEdited(QString)),
            SLOT(onRuleInputEdited(QString)));

    connect(ui->ruleInputWidget, SIGNAL(targetTextEdited(QString)),
            SLOT(onRuleTargetEdited(QString)));

    connect(ui->centralSplitter,  SIGNAL(splitterMoved(int,int)), SLOT(onSplitterMoved(int,int)));
    connect(ui->teachTabsplitter, SIGNAL(splitterMoved(int,int)), SLOT(onSplitterMoved(int,int)));

    // Test tab

    connect(ui->testInputText, SIGNAL(returnPressed()), SLOT(onTestInputTextEntered()));

    connect(ui->clearTestConversationButton, SIGNAL(clicked()),
            SLOT(onClearTestConversationButtonPressed()));

    // Chat connetion tab

    connect(ui->connectButton,    SIGNAL(clicked()), SLOT(onConnectButtonPressed()));
    connect(ui->disconnectButton, SIGNAL(clicked()), SLOT(onDisconnectButtonPressed()));

    connect(m_appFacade, SIGNAL(connected()),          SLOT(onConnectionOk()));
    connect(m_appFacade, SIGNAL(disconnected()),       SLOT(onDisconnection()));
    connect(m_appFacade, SIGNAL(connectionError(int)), SLOT(onConnectionError(int)));

    connect(ui->passwordText, SIGNAL(returnPressed()), ui->connectButton, SLOT(click()));

    connect(ui->rosterWidget, SIGNAL(selectionChanged()), SLOT(onRosterSelectionChanged()));

    connect(ui->changeAccountButton,       SIGNAL(clicked()),
            SLOT(onChangeAccountButtonPressed()));
    connect(ui->cancelChangeAccountButton, SIGNAL(clicked()),
            SLOT(onCancelChangeAccountButtonPressed()));

    // Conversation history tab

    connect(m_appFacade, SIGNAL(newConversationEntry(BE::Conversation::Entry)),
            SLOT(onNewChatConversation(BE::Conversation::Entry)));

    connect(ui->chatHistory, SIGNAL(teachRule(QString)), SLOT(onTeachFromHistoryWidget(QString)));
    connect(ui->chatHistory, SIGNAL(showRule(quint64)),  SLOT(onShowRule(quint64)));
    connect(ui->chatHistory, SIGNAL(removed(QDate,QString)), SLOT(onRemovedHistory(QDate,QString)));
    connect(ui->chatHistory, SIGNAL(removedAll()),       SLOT(onRemovedAllHistory()));

    // Options tab

    connect(ui->rmDupCheckBox, SIGNAL(stateChanged(int)), SLOT(onRmDupCheckBoxChanged(int)));
    connect(ui->lemmatizerCheckBox, SIGNAL(stateChanged(int)),
            SLOT(onLemmatizerCheckBoxChanged(int)));
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::selectFirstRule()
{
    if (rootRule()) {
        if (rootRule()->childCount() > 0) {
            BE::Rule *child = rootRule()->child(0);
            if (child->childCount() > 0) {
                selectRule(child->child(0));
            } else {
                selectRule(child);
            }
        }
    }
}

//--------------------------------------------------------------------------------------------------
// Event handling
//--------------------------------------------------------------------------------------------------

bool Lvk::FE::MainWindow::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::FocusOut) {
        if (object == ui->testInputText) {
            ui->ruleInputWidget->clearHighlight();
            ui->ruleOutputWidget->clearHighlight();
//        } else if (object == ui->ruleInputWidget) {
//            onRuleInputEditingFinished();
//        } else if (object == ui->ruleOutputWidget) {
//            onRuleOutputEditingFinished();
        }
    }

    return QMainWindow::eventFilter(object, event);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::closeEvent(QCloseEvent *event)
{
    if (hasUnsavedChanges()) {
        int code = showSaveChangesDialog();

        if (code == QMessageBox::Yes) {
            if (saveChanges()) {
                event->accept();
            } else {
                event->ignore();
            }
        } else if (code == QMessageBox::Cancel) {
            event->ignore();
        } else {
            event->accept();
        }
    }

    if (event->isAccepted()) {
        saveAllSettings();
    }
}

//--------------------------------------------------------------------------------------------------
// Load / Save settings
//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::loadAllSettings()
{
    qDebug() << "Loading main window settings...";
    loadMainWindowSettings();
    loadChatSettings();
    loadSplittersSettings();
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::saveAllSettings()
{
    saveMainWindowSettings();
    saveChatSettings();
    saveSplittersSettings();

    if (!m_filename.isEmpty()) {
        Lvk::Cmn::Settings settings;
        settings.setValue(SETTING_LAST_FILE, m_filename);
        m_lastFilename = m_filename;
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::loadMainWindowSettings()
{
    Cmn::Settings settings;

    if (settings.contains(SETTING_MAIN_WINDOW_SIZE) && settings.contains(SETTING_MAIN_WINDOW_POS)) {
        resize(settings.value(SETTING_MAIN_WINDOW_SIZE).toSize());
        move(settings.value(SETTING_MAIN_WINDOW_POS).toPoint());
    } else {
        QRect scrGeo = qApp->desktop()->availableGeometry(0);

        resize(std::min<int>(width(), scrGeo.width()*0.8),
               std::min<int>(height(), scrGeo.height()*0.8));

        move((scrGeo.width() - width())/2, (scrGeo.height() - height())/2);
    }
    if (settings.value(SETTING_MAIN_WINDOW_MAXIMIZED).toBool()) {
        showMaximized();
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::loadSplittersSettings()
{
    Cmn::Settings settings;

    QList<int> centralSplSizes;
    centralSplSizes << settings.value(SETTING_MAIN_WINDOW_MAIN_TAB_W, width()*0.7).toInt();
    centralSplSizes << settings.value(SETTING_MAIN_WINDOW_TEST_TAB_W, width()*0.3).toInt();
    ui->centralSplitter->setSizes(centralSplSizes);

    QList<int> teachSplSizes;
    teachSplSizes << settings.value(SETTING_MAIN_WINDOW_RULE_TREE_W, width()*0.7*0.45).toInt();
    teachSplSizes << settings.value(SETTING_MAIN_WINDOW_RULE_EDIT_W, width()*0.7*0.55).toInt();
    ui->teachTabsplitter->setSizes(teachSplSizes);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::loadChatSettings()
{
    // Nothing to load
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::saveMainWindowSettings()
{
    Cmn::Settings settings;

    settings.setValue(SETTING_MAIN_WINDOW_SIZE, size());
    settings.setValue(SETTING_MAIN_WINDOW_POS, pos());
    settings.setValue(SETTING_MAIN_WINDOW_MAXIMIZED, isMaximized());
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::saveSplittersSettings()
{
    if (m_tabsLayout == TeachTabsLayout) {
        Cmn::Settings settings;

        settings.setValue(SETTING_MAIN_WINDOW_MAIN_TAB_W, ui->centralSplitter->sizes().at(0));
        settings.setValue(SETTING_MAIN_WINDOW_TEST_TAB_W, ui->centralSplitter->sizes().at(1));
        settings.setValue(SETTING_MAIN_WINDOW_RULE_TREE_W, ui->teachTabsplitter->sizes().at(0));
        settings.setValue(SETTING_MAIN_WINDOW_RULE_EDIT_W, ui->teachTabsplitter->sizes().at(1));
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::saveChatSettings()
{
    // Nothing to save
}

//--------------------------------------------------------------------------------------------------
// UI Modes
//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::setUiMode(UiMode mode)
{
    updateTabsLayout(mode);

    // Set up tabs ///////////////////////////////////////

    switch (mode) {

    // init tab //

    case WelcomeTabUiMode:
        ui->mainTabWidget->setCurrentWidget(ui->welcomeTab);
        ui->initStackWidget->setCurrentIndex(0);
        ui->openLastChatbotButton->setVisible(m_lastFilename.size() > 0);
        break;

    // Edit rules tab //

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
        ui->teachRuleButton->setText(tr("Save changes"));
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
        ui->chatbotRepliesLabel->setText(tr("Chatbot replies:"));
        ui->teachRuleButton->setText(tr("Teach rule to the chatbot"));
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
        ui->chatbotRepliesLabel->setText(tr("If chatbot does not understand, it replies:"));
        ui->teachRuleButton->setText(tr("Teach rule to the chatbot"));
        break;

    // Chat connection tab //

    case ChatDisconnectedUiMode:
        ui->mainTabWidget->setCurrentWidget(ui->connectTab);
        ui->curUsernameLabel->setText(m_appFacade->username());
        ui->chatTypeIcon->setPixmap(m_appFacade->chatType() == BE::AppFacade::FbChat ?
                                        QPixmap(FB_ICON_FILE) : QPixmap(GMAIL_ICON_FILE));
        ui->connectToChatStackWidget->setCurrentIndex(0);
        ui->passwordText->setEnabled(true);
        ui->changeAccountButton->setEnabled(true);
        ui->connectButton->setText(tr("Connect"));
        ui->connectionProgressBar->setVisible(false);
        ui->connectionStatusLabel->setText(tr("Disconnected"));
        ui->connectionStatusLabel->setStyleSheet("");
        ui->rosterWidget->clear();
        break;

    case ChatConnectingUiMode:
        ui->mainTabWidget->setCurrentWidget(ui->connectTab);
        ui->connectToChatStackWidget->setCurrentIndex(0);
        ui->passwordText->setEnabled(false);
        ui->changeAccountButton->setEnabled(false);
        ui->connectButton->setText(tr("Disconnect"));
        ui->connectionProgressBar->setVisible(true);
        ui->connectionStatusLabel->setText(tr("Connecting..."));
        ui->connectionStatusLabel->setStyleSheet("");
        break;

    case ChatConnectionFailedUiMode:
        ui->mainTabWidget->setCurrentWidget(ui->connectTab);
        ui->connectToChatStackWidget->setCurrentIndex(0);
        ui->passwordText->setEnabled(true);
        ui->changeAccountButton->setEnabled(true);
        ui->connectButton->setText(tr("Connect"));
        ui->connectionProgressBar->setVisible(false);
        ui->connectionStatusLabel->setText(tr("Connection error. "
                                              "Please verify your username and password."));
        ui->connectionStatusLabel->setStyleSheet("color:red");
        break;

    case ChatConnectionSSLFailedUiMode:
        ui->mainTabWidget->setCurrentWidget(ui->connectTab);
        ui->connectToChatStackWidget->setCurrentIndex(0);
        ui->passwordText->setEnabled(true);
        ui->changeAccountButton->setEnabled(true);
        ui->connectButton->setText(tr("Connect"));
        ui->connectionProgressBar->setVisible(false);
        ui->connectionStatusLabel->setText(tr("Connection error. "
                                              "You system does not support secure connections."));
        ui->connectionStatusLabel->setStyleSheet("color:red");
        break;

    case ChatConnectionOkUiMode:
        ui->mainTabWidget->setCurrentWidget(ui->connectTab);
        ui->connectToChatStackWidget->setCurrentIndex(1);
        ui->disconnectButton->setText(tr("Disconnect ") + m_appFacade->username());
        ui->disconnectButton->setIcon(m_appFacade->chatType() == BE::AppFacade::FbChat ?
                                          QIcon(FB_ICON_FILE) : QIcon(GMAIL_ICON_FILE));
        // Not visible anymore:
        ui->passwordText->setEnabled(false);
        ui->connectButton->setText(tr("Disconnect"));
        ui->connectionProgressBar->setVisible(false);
        ui->connectionStatusLabel->setText(tr("Connection sucessful!"));
        ui->connectionStatusLabel->setStyleSheet("color:green");
        break;

    case ChangeAccountUiMode:
        ui->mainTabWidget->setCurrentWidget(ui->connectTab);
        ui->connectToChatStackWidget->setCurrentIndex(2);
        ui->verifyExplanationLabel->setText(tr("Please insert your username and password and press "
                                               "\"Verify account\" button."));
        ui->verifyAccountButton->setEnabled(true);
        ui->usernameText_v->setEnabled(true);
        ui->passwordText_v->setEnabled(true);
        ui->fbChatRadio_v->setEnabled(true);
        ui->gtalkChatRadio_v->setEnabled(true);
        ui->connectionProgressBar_v->setVisible(false);
        ui->connectionStatusLabel_v->setVisible(false);
        ui->passwordText->setText("");
        break;

    case VerifyAccountUiMode:
        ui->mainTabWidget->setCurrentWidget(ui->welcomeTab);
        ui->connectToChatStackWidget->setCurrentIndex(2);
        ui->verifyExplanationLabel->setText(tr("To create a chatbot you need a Facebook or Gmail "
                                               "account.\nPlease insert your username and password "
                                               "and press \"Verify account\" button."));
        ui->verifyAccountButton->setEnabled(true);
        ui->usernameText_v->setEnabled(true);
        ui->passwordText_v->setEnabled(true);
        ui->fbChatRadio_v->setEnabled(true);
        ui->gtalkChatRadio_v->setEnabled(true);
        ui->connectionProgressBar_v->setVisible(false);
        ui->connectionStatusLabel_v->setVisible(false);
        ui->passwordText->setText("");
        break;

    case ChangeAccountConnectingUiMode:
    case VerifyAccountConnectingUiMode:
        ui->mainTabWidget->setCurrentWidget(mode == VerifyAccountConnectingUiMode ?
                                                ui->welcomeTab : ui->connectTab);
        ui->connectToChatStackWidget->setCurrentIndex(2);
        ui->verifyAccountButton->setEnabled(false);
        ui->usernameText_v->setEnabled(false);
        ui->passwordText_v->setEnabled(false);
        ui->fbChatRadio_v->setEnabled(false);
        ui->gtalkChatRadio_v->setEnabled(false);
        ui->connectionProgressBar_v->setVisible(true);
        ui->connectionStatusLabel_v->setVisible(true);
        break;

    case ChangeAccountFailedUiMode:
    case VerifyAccountFailedUiMode:
        ui->mainTabWidget->setCurrentWidget(mode == VerifyAccountFailedUiMode ?
                                                ui->welcomeTab : ui->connectTab);
        ui->connectToChatStackWidget->setCurrentIndex(2);
        ui->verifyAccountButton->setEnabled(true);
        ui->usernameText_v->setEnabled(true);
        ui->passwordText_v->setEnabled(true);
        ui->fbChatRadio_v->setEnabled(true);
        ui->gtalkChatRadio_v->setEnabled(true);
        ui->connectionProgressBar_v->setVisible(false);
        ui->connectionStatusLabel_v->setVisible(false);
        break;
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::updateTabsLayout(UiMode mode)
{
    UiTabsLayout visibleTabs = NullLayout;

    switch (mode) {
    case WelcomeTabUiMode:
        visibleTabs = WelcomeTabsLayout;
        break;
    case VerifyAccountUiMode:
    case VerifyAccountConnectingUiMode:
    case VerifyAccountFailedUiMode:
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

            ui->welcomeTab->setVisible(true);
            ui->teachTab->setVisible(false);
            ui->connectTab->setVisible(false);
            ui->conversationsTab->setVisible(false);
            ui->rightSideTabWidget->setVisible(false);

            ui->mainTabWidget->addTab(ui->welcomeTab, tr("Init"));
            ui->mainTabWidget->removePage(ui->teachTab);
            ui->mainTabWidget->removePage(ui->connectTab);
            ui->mainTabWidget->removePage(ui->conversationsTab);
            ui->mainTabWidget->removePage(ui->optionsTab);
            break;

        case VerifyAccountUiMode:
        case VerifyAccountConnectingUiMode:
        case VerifyAccountFailedUiMode:
            ui->actionSave->setEnabled(false);
            ui->actionSaveAs->setEnabled(false);
            ui->actionImport->setEnabled(false);
            ui->actionExport->setEnabled(false);

            ui->welcomeTab->setVisible(false);
            ui->teachTab->setVisible(false);
            ui->connectTab->setVisible(true);
            ui->conversationsTab->setVisible(false);
            ui->rightSideTabWidget->setVisible(false);

            ui->mainTabWidget->addTab(ui->connectTab, tr("Verify account"));
            ui->mainTabWidget->removePage(ui->welcomeTab);
            ui->mainTabWidget->removePage(ui->teachTab);
            ui->mainTabWidget->removePage(ui->conversationsTab);
            ui->mainTabWidget->removePage(ui->optionsTab);
            break;

        default:
            ui->actionSave->setEnabled(true);
            ui->actionSaveAs->setEnabled(true);
            ui->actionImport->setEnabled(true);
            ui->actionExport->setEnabled(true);

            ui->welcomeTab->setVisible(false);
            ui->teachTab->setVisible(true);
            ui->connectTab->setVisible(true);
            ui->conversationsTab->setVisible(true);
            ui->rightSideTabWidget->setVisible(true);

            ui->mainTabWidget->removePage(ui->welcomeTab);
            ui->mainTabWidget->addTab(ui->teachTab, tr("Teach"));
            ui->mainTabWidget->addTab(ui->connectTab, tr("Connect"));
            ui->mainTabWidget->addTab(ui->conversationsTab, tr("Conversations"));
            ui->mainTabWidget->addTab(ui->optionsTab, tr("Advanced options"));
            break;
        }
    }
}

//--------------------------------------------------------------------------------------------------
// Shortcuts to special rules
//--------------------------------------------------------------------------------------------------

inline Lvk::BE::Rule * Lvk::FE::MainWindow::rootRule()
{
    return m_appFacade->rootRule();
}

//--------------------------------------------------------------------------------------------------

inline const Lvk::BE::Rule * Lvk::FE::MainWindow::rootRule() const
{
    return m_appFacade->rootRule();
}

//--------------------------------------------------------------------------------------------------

inline Lvk::BE::Rule * Lvk::FE::MainWindow::evasivesRule()
{
    return m_appFacade->evasivesRule();
}

//--------------------------------------------------------------------------------------------------

inline Lvk::BE::Rule * Lvk::FE::MainWindow::selectedRule()
{
    if (!m_ruleTreeSelectionModel->selectedIndexes().isEmpty()) {
        QModelIndex selectedIndex = m_ruleTreeSelectionModel->selectedIndexes().first();
        if (selectedIndex.isValid()) {
            return m_ruleTreeModel->itemFromIndex(selectedIndex);
        }
    }
    return 0;
}

//--------------------------------------------------------------------------------------------------
// Misc Rules
//--------------------------------------------------------------------------------------------------

QString Lvk::FE::MainWindow::getRuleDisplayName(const BE::Rule *rule) const
{
    if (!rule) {
        return "";
    } else  {
        return getRuleDisplayName(m_ruleTreeModel->indexFromItem(rule));
    }
}

//--------------------------------------------------------------------------------------------------

QString Lvk::FE::MainWindow::getRuleDisplayName(const QModelIndex &index) const
{
    return index.data(Qt::DisplayRole).toString();
}

//--------------------------------------------------------------------------------------------------
// New, save, save as, load, open menus
//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onNewMenuTriggered()
{
    bool canceled = false;

    if (hasUnsavedChanges()) {
        int code = showSaveChangesDialog();

        if (code == QMessageBox::Yes) {
            if (!saveChanges()) {
                canceled = true;
            }
        } else if (code == QMessageBox::No) {
            // Nothing to do
        } else if (code == QMessageBox::Cancel) {
            canceled = true;
        }
    }

    if (!canceled) {
        clear();
        setUiMode(VerifyAccountUiMode);
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onOpenMenuTriggered()
{
    bool canceled = false;

    if (hasUnsavedChanges()) {
        int code = showSaveChangesDialog();

        if (code == QMessageBox::Yes) {
            if (!saveChanges()) {
                canceled = true;
            }
        } else if (code == QMessageBox::No) {
            // Nothing to do
        } else if (code == QMessageBox::Cancel) {
            canceled = true;
        }
    }

    if (!canceled) {
        QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), "",getFileFilters());

        if (!filename.isEmpty()) {
            if (load(filename)) {
                startEditMode();
            } else {
                clear();
            }
        }
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onOpenLastFileMenuTriggered()
{
    if (!m_lastFilename.isEmpty()) {
        if (load(m_lastFilename)) {
            startEditMode();
        } else {
            clear();
        }
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onSaveMenuTriggered()
{
    saveChanges();
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onSaveAsMenuTriggered()
{
    saveAsChanges();
}

//--------------------------------------------------------------------------------------------------

bool Lvk::FE::MainWindow::saveChanges()
{
    bool saved = false;

    if (m_filename.isEmpty()) {
        saved = saveAsChanges();
    } else {
        if (m_ruleEdited) {
            teachRule(selectedRule());
        }

        saved = m_appFacade->save();

        if (!saved) {
            QMessageBox::critical(this, tr("Save File"), tr("Could not save file. "
                                  "Please verify that you have write permissions."));
        }
    }

    return saved;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::FE::MainWindow::saveAsChanges()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Save File"), "", getFileFilters());

    bool saved = false;

    if (!filename.isEmpty()) {
        if (!filename.endsWith("." + FILE_EXTENSION)) {
            filename.append("." + FILE_EXTENSION);
        }

        QString filenameBak = m_filename;

        setFilename(filename);

        if (m_ruleEdited) {
            teachRule(selectedRule());
        }

        saved = m_appFacade->saveAs(m_filename);

        if (!saved) {
            QMessageBox::critical(this, tr("Save File"), tr("Could not save file. "
                                  "Please verify that you have write permissions."));

            setFilename(filenameBak);
        }
    }

    return saved;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::FE::MainWindow::load(const QString &filename)
{
    clear(false);

    bool success = initCoreAndModelsWithFile(filename);

    if (success) {
        // load conversation history
        ui->chatHistory->setConversation(m_appFacade->chatHistory());

        // load persisted roster
        BE::Roster roster;
        loadRoster(roster, rosterFilename());
        ui->ruleInputWidget->setRoster(roster);

        // Advanced options
        unsigned options = m_appFacade->nlpEngineOptions();
        ui->rmDupCheckBox->setCheckState(options & BE::AppFacade::RemoveDupChars ?
                                             Qt::Checked : Qt::Unchecked);
        ui->lemmatizerCheckBox->setCheckState(options & BE::AppFacade::LemmatizeSentence ?
                                                  Qt::Checked : Qt::Unchecked);
    } else {
        QMessageBox::critical(this, tr("Open File"), tr("Cannot open ") + m_filename);
    }

    return success;
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::setFilename(const QString &filename)
{
    m_filename = filename;

    if (!filename.isEmpty()) {
        QFileInfo fileInfo(filename);
        setWindowTitle(fileInfo.fileName() + " - " + APP_NAME);
    } else {
        setWindowTitle(APP_NAME);
    }
}

//--------------------------------------------------------------------------------------------------

bool Lvk::FE::MainWindow::hasUnsavedChanges()
{
    return m_ruleEdited || m_appFacade->hasUnsavedChanges();
}

//--------------------------------------------------------------------------------------------------

int Lvk::FE::MainWindow::showSaveChangesDialog()
{
    QString text = !m_filename.isEmpty() ?
                tr("Do you want to save the changes in ") + m_filename + "?" :
                tr("Do you want to save the changes?");

    QMessageBox msg(QMessageBox::Question, tr("Save changes"), text,
                    QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, this);

    return msg.exec();
}

//--------------------------------------------------------------------------------------------------
// Import and export menus
//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onImportMenuTriggered()
{
    const QString IMPORT_TITLE = tr("Import Rules");

    QString filename = QFileDialog::getOpenFileName(this, IMPORT_TITLE, "", getFileExportFilters());

    if (!filename.isEmpty()) {

        BE::Rule container;
        if (m_appFacade->importRules(&container, filename)) {
            FE::RuleTreeModel *model = new FE::RuleTreeModel(&container, this);

            FE::ImportDialog importDialog(IMPORT_TITLE, model, this);

            BE::Rule selectedRulesContainer;
            if (importDialog.exec(&selectedRulesContainer) == QDialog::Accepted) {

                // FIXME do not use notifyDataAboutToChange
                m_ruleTreeModel->notifyDataAboutToChange();
                if (!m_appFacade->mergeRules(&selectedRulesContainer)) {
                    QMessageBox::critical(this, IMPORT_TITLE, tr("Cannot import file ") + filename);
                }
                // FIXME do not use notifyDataChanged
                m_ruleTreeModel->notifyDataChanged();

                refreshRuleOnWidget();
            }
        } else {
            QMessageBox::critical(this, IMPORT_TITLE, tr("Cannot import file ") + filename);
        }
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onExportMenuTriggered()
{
    const QString EXPORT_TITLE = tr("Export Rules");

    FE::RuleTreeModel *model = new FE::RuleTreeModel(m_appFacade->rootRule(), this);

    FE::ExportDialog exportDialog(EXPORT_TITLE, model, this);

    BE::Rule container;
    if (exportDialog.exec(&container) == QDialog::Accepted) {

        QString filename = QFileDialog::getSaveFileName(this, EXPORT_TITLE , "",
                                                        getFileExportFilters());

        if (!filename.isEmpty()) {
            if (!filename.endsWith("." + FILE_EXPORT_EXTENSION)) {
                filename.append("." + FILE_EXPORT_EXTENSION);
            }

            if (!m_appFacade->exportRules(&container, filename)) {
                QMessageBox::critical(this, EXPORT_TITLE, tr("Cannot export file ") + filename);
            }
        }
    }
}

//--------------------------------------------------------------------------------------------------
// Other menus
//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onAboutMenuTriggered()
{
    QString title = QString(tr("About %1..."))
            .arg(APP_NAME);
    QString text = QString(tr("<b>%1 %2</b><br/>"
                              "Rev: %3<br/><br/>"
                              "Developed by LVK<br/>%4<br/>%5"))
            .arg(APP_NAME)
            .arg(APP_VERSION_STR)
            .arg(QString(APP_VERSION_REV).mid(0, 30))
            .arg(APP_URL_CONTACT)
            .arg(APP_MAIL_CONTACT);

    QMessageBox msg(QMessageBox::NoIcon, title, text);
    msg.setIconPixmap(QPixmap(APP_ICON_FILE));
    msg.exec();
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onExitMenuTriggered()
{
    this->close();
}

//--------------------------------------------------------------------------------------------------
// Add categories and rules
//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onAddCategoryButtonClicked()
{
    addCategoryWithDialog();
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onAddRuleButtonClicked()
{
    addRuleWithDialog();
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onRemoveButtonClicked()
{
    removeSelectedRuleWithDialog();
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::Rule * Lvk::FE::MainWindow::addRuleWithDialog()
{
    BE::Rule *rule = 0;

    if (m_ruleTreeSelectionModel->selectedRows().isEmpty()) {
        QString title = tr("Add rule");
        QString text = tr("Select the category where the rule will belong to");
        QMessageBox::information(this, title, text);

        return rule;
    }

    QModelIndex selectedIndex = m_ruleTreeSelectionModel->selectedRows().first();
    BE::Rule *selectedItem = m_ruleTreeModel->itemFromIndex(selectedIndex);
    BE::Rule *parentCategory = 0;

    if (selectedItem->type() == BE::Rule::ContainerRule) {
        parentCategory = selectedItem;
        ui->categoriesTree->setExpanded(selectedIndex, true);
    } else if (selectedItem->type() == BE::Rule::OrdinaryRule) {
        parentCategory =  m_ruleTreeModel->itemFromIndex(selectedIndex.parent());
        ui->categoriesTree->setExpanded(selectedIndex.parent(), true);
    } else if (selectedItem->type() == BE::Rule::EvasiveRule) {
        QString title = tr("Add rule");
        QString text = tr("'%0' is an special category that cannot contain rules");
        QMessageBox::critical(this, title, text.arg(getRuleDisplayName(selectedIndex)));
    }

    if (parentCategory) {
        rule = addRule("", parentCategory);

        if (rule) {
            selectRule(rule);
            ui->ruleInputWidget->setFocusOnInput();
        } else {
            QString title = tr("Internal error");
            QString text = tr("The rule could not be added because of an internal error");
            QMessageBox::critical(this, title, text);
        }
    }

    return rule;
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::Rule* Lvk::FE::MainWindow::addCategoryWithDialog()
{
    BE::Rule *category = 0;

    bool ok;
    QString name = QInputDialog::getText(this, tr("Add category"), tr("Category name:"),
                                         QLineEdit::Normal, "", &ok);

    if (ok) {
        if (!name.isEmpty()) {
            category = addCategory(name);

            if (category) {
                selectRule(category);
            } else {
                QString title = tr("Internal error");
                QString text = tr("The category could not be added because of an internal error");
                QMessageBox::critical(this, title, text);
            }
        } else {
            QString title = tr("Add category");
            QString text = tr("The category name cannot be empty");
            QMessageBox::critical(this, title, text);
        }
    }

    return category;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::FE::MainWindow::removeSelectedRuleWithDialog()
{
    bool removed = false;

    if (m_ruleTreeSelectionModel->selectedRows().isEmpty()) {
        QString text = tr("Remove rule or category");
        QString title = tr("Select the rule or category you want to remove");
        QMessageBox::critical(this, text, title);

        return removed;
    }

    QModelIndex selectedIndex = m_ruleTreeSelectionModel->selectedRows().first();
    BE::Rule *selectedItem = m_ruleTreeModel->itemFromIndex(selectedIndex);
    BE::Rule::Type ruleType = selectedItem->type();

    QString dialogText;
    QString dialogTitle;

    if (ruleType == BE::Rule::ContainerRule || ruleType == BE::Rule::OrdinaryRule) {
        if (selectedItem->type() == BE::Rule::ContainerRule) {
            dialogTitle = tr("Remove category");
            dialogText  = tr("Are you sure you want to remove the category '%0'?\n"
                             "All rules belonging to that category will be also removed");

        } else {
            dialogTitle = tr("Remove rule");
            dialogText  = tr("Are you sure you want to remove the rule '%0'?");
        }

        QMessageBox msg(QMessageBox::Question, dialogTitle,
                        dialogText.arg(getRuleDisplayName(selectedIndex)),
                        QMessageBox::Yes | QMessageBox::No, this);

       if (msg.exec() == QMessageBox::Yes) {
            removed = m_ruleTreeModel->removeRow(selectedIndex.row(), selectedIndex.parent());

            if (removed) {
                m_appFacade->refreshNlpEngine();
            } else {
                dialogTitle = tr("Internal error");
                dialogText = tr("The rule/category could not be removed because of an internal"
                                " error");
                QMessageBox::critical(this, dialogTitle, dialogText);
            }
        }
    } else if (ruleType == BE::Rule::EvasiveRule) {
        dialogTitle = tr("Cannot remove");
        dialogText = tr("The selected category cannot be removed because is mandatory");
        QMessageBox::information(this, dialogTitle, dialogText);
    }

    return removed;
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::Rule *Lvk::FE::MainWindow::addCategory(const QString &name)
{
    //BE::Rule *category = new BE::Rule(name, BE::Rule::ContainerRule);
    //bool appended = m_ruleTreeModel->appendItem(category);

    int lastButOneRow = m_ruleTreeModel->rowCount() - 1;
    if (lastButOneRow < 0) {
        lastButOneRow = 0;
    }

    bool added = m_ruleTreeModel->insertRows(lastButOneRow, 1, QModelIndex());
    BE::Rule *category = m_ruleTreeModel->rootItem()->child(lastButOneRow);
    category->setName(name);
    category->setType(BE::Rule::ContainerRule);

    if (added) {
        return category;
    } else {
        delete category;

        return 0;
    }
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::Rule *Lvk::FE::MainWindow::addRule(const QString &name, BE::Rule *category)
{
    BE::Rule *rule = new BE::Rule(name, category);

    bool appended = m_ruleTreeModel->appendItem(rule);

    if (appended) {
        return rule;
    } else {
        delete rule;

        return 0;
    }
}

//--------------------------------------------------------------------------------------------------

const Lvk::BE::Rule *Lvk::FE::MainWindow::findRule(quint64 ruleId) const
{
    const BE::Rule *root = rootRule();

    if (!root || !ruleId) {
        return 0;
    }

    for (BE::Rule::const_iterator it = root->begin(); it != root->end(); ++it) {
        const BE::Rule* rule = *it;
        if (rule->id() == ruleId /*&& rule->type() == BE::Rule::OrdinaryRule*/) {
            return rule;
        }
    }

    return 0;
}

//--------------------------------------------------------------------------------------------------
// History actions
//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onRemovedAllHistory()
{
    m_appFacade->clearChatHistory();
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onRemovedHistory(const QDate &date, const QString &username)
{
    m_appFacade->clearChatHistory(date, username);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onShowRule(quint64 ruleId)
{
    const BE::Rule *rule = findRule(ruleId);

    if (rule && rule->type() == BE::Rule::OrdinaryRule) {
        m_ruleTreeSelectionModel->clearSelection();
        selectRule(rule);
    } else {
        QString title = tr("Rule not found");
        QString msg = tr("Rule not found. The rule was removed.");
        QMessageBox::information(this, title, msg, QMessageBox::Ok);
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onTeachFromHistoryWidget(const QString &msg)
{
    setUiMode(EditRuleUiMode);

    BE::Rule *category = getCategoryFromDialog();

    if (category) {
        BE::Rule *rule = new BE::Rule("", QStringList() << msg, QStringList(), category);

        bool appended = m_ruleTreeModel->appendItem(rule);

        if (appended) {
            selectRule(rule);
            ui->ruleOutputWidget->setFocus();
        } else {
            delete rule;
        }
    }
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::Rule * Lvk::FE::MainWindow::getCategoryFromDialog()
{
    const QString SPLIT_TOKEN = ". ";

    QStringList categories;

    for (int i = 0; rootRule() && i < rootRule()->childCount(); ++i) {
        const Lvk::BE::Rule * rule = rootRule()->child(i);
        if (rule->type() == BE::Rule::ContainerRule) {
            categories << (QString::number(i+1) + SPLIT_TOKEN + rule->name());
        }
    }

    categories << tr("New category...");

    bool ok;
    QString title = tr("Select category");
    QString msg = tr("Please select a category to add the new rule:");
    QString item = QInputDialog::getItem(title, msg, categories, 0, false, &ok);

    BE::Rule *category = 0;

    if (ok) {
        int i = item.split(SPLIT_TOKEN).at(0).toInt() - 1;
        if (i >= 0 && i < rootRule()->childCount()) {
            category = rootRule()->child(i);
        } else {
            category = addCategoryWithDialog();
        }
    }

    return category;
}

//--------------------------------------------------------------------------------------------------
// Rule selection
//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onRuleSelectionChanged(const QItemSelection &selected,
                                            const QItemSelection &deselected)
{
    if (!deselected.indexes().isEmpty()) {
        if (m_ruleEdited) {
            BE::Rule *rule = m_ruleTreeModel->itemFromIndex(deselected.indexes().first());
            handleRuleEdited(rule);
        }
    }

    m_ruleEdited = false;

    if (!selected.indexes().isEmpty()) {
        const BE::Rule *rule =
                static_cast<const BE::Rule *>(selected.indexes().first().internalPointer());

        showRuleOnWidget(rule);
    } else {
        showRuleOnWidget(0);
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::showRuleOnWidget(const BE::Rule *rule)
{
    // TODO Create a RuleWidget class that contains nameTextEdit, inputWidget and outputWidget.
    //      Define a method setRule(Rule*) that does this job.

    if (!rule) {
        setUiMode(RuleSelectionEmptyUiMode);
        ui->categoryNameLabel->clear();
        ui->ruleInputWidget->clear();
        ui->ruleOutputWidget->clear();
    } else if (rule->type() == BE::Rule::OrdinaryRule) {
        setUiMode(EditRuleUiMode);
        ui->categoryNameTextEdit->clear();
        ui->ruleInputWidget->setTargets(rule->target());
        ui->ruleInputWidget->setInput(rule->input());
        ui->ruleOutputWidget->setOutput(rule->output());
    } else if (rule->type() == BE::Rule::EvasiveRule) {
        setUiMode(EditEvasivesUiMode);
        ui->categoryNameTextEdit->clear();
        ui->ruleInputWidget->clear();
        ui->ruleOutputWidget->setOutput(rule->output());
    } else if (rule->type() == BE::Rule::ContainerRule) {
        setUiMode(EditCategoryUiMode);
        ui->categoryNameTextEdit->setText(rule->name());
        ui->ruleInputWidget->clear();
        ui->ruleOutputWidget->clear();
    } else {
        setUiMode(RuleSelectionEmptyUiMode);
        ui->categoryNameLabel->clear();
        ui->ruleInputWidget->clear();
        ui->ruleOutputWidget->clear();
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::refreshRuleOnWidget()
{
    showRuleOnWidget(selectedRule());
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::selectRule(const BE::Rule *rule)
{
    if (!rule) {
        return;
    }

    QModelIndex ruleIndex = m_ruleTreeModel->indexFromItem(rule);

    if (ruleIndex.isValid()) {
        m_ruleTreeSelectionModel->setCurrentIndex(ruleIndex, QItemSelectionModel::ClearAndSelect);

        ui->categoriesTree->setExpanded(ruleIndex.parent(), true);
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::startEditMode()
{
    setUiMode(ChatDisconnectedUiMode);
    setUiMode(EditRuleUiMode);
    selectFirstRule();
}

//--------------------------------------------------------------------------------------------------
// Rule edition & undo
//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onRuleInputEdited(const QString &ruleInput)
{
    onRuleEdited();

    // Refresh rule tree while user is typing
    if (!m_ruleTreeSelectionModel->selectedIndexes().isEmpty()) {
        QModelIndex selectedIndex = m_ruleTreeSelectionModel->selectedIndexes().first();
        m_ruleTreeModel->setData(selectedIndex, ruleInput, Qt::EditRole);
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onRuleTargetEdited(const QString &/*ruleInput*/)
{
    onRuleEdited();
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onRuleEdited()
{
    const BE::Rule *selRule = selectedRule();

    if (selRule && !m_ruleEdited) {
        m_ruleEdited = true;

        m_ruleBackup.setStatus(selRule->status());
        m_ruleBackup.setName(selRule->name());
        m_ruleBackup.setTarget(selRule->target());
        m_ruleBackup.setInput(selRule->input());
        m_ruleBackup.setOutput(selRule->output());

        ui->teachRuleButton->setEnabled(true);
        ui->undoRuleButton->setEnabled(true);
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onTeachButtonPressed()
{
    teachRule(selectedRule());
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onUndoButtonPressed()
{
    undoRule(selectedRule());
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::handleRuleEdited(BE::Rule *rule)
{
    if (!rule) {
        return;
    }

    QString dialogText;
    QString dialogTitle;

    if (rule->type() == BE::Rule::ContainerRule) {
        dialogTitle = tr("Category changed");
        dialogText  = tr("The category '%0' has changed.\n"
                         "Do you want to save the changes made to the category?");
    } else {
        dialogTitle = tr("Rule changed");
        dialogText  = tr("The rule '%0' has changed.\n"
                         "Do you want to teach the changes made to the rule?");
    }

    QMessageBox msg(QMessageBox::Question,
                    dialogTitle, dialogText.arg(getRuleDisplayName(rule)),
                    QMessageBox::Yes | QMessageBox::No, this);

    int code = msg.exec();

    if (code == QMessageBox::Yes) {
        teachRule(rule);
    } else {
        undoRule(rule);
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::teachRule(BE::Rule *rule)
{
    if (!rule) {
        return;
    }
    if (!m_ruleEdited) {
        return;
    }

    if (rule->type() == BE::Rule::OrdinaryRule) {
        rule->setTarget(ui->ruleInputWidget->targets());
        rule->setInput(ui->ruleInputWidget->input());
        rule->setOutput(ui->ruleOutputWidget->output());
    } else if (rule->type() == BE::Rule::EvasiveRule) {
        rule->setOutput(ui->ruleOutputWidget->output());
    } else if (rule->type() == BE::Rule::ContainerRule) {
        rule->setName(ui->categoryNameTextEdit->text());
    }

    m_ruleEdited = false;

    ui->teachRuleButton->setEnabled(false);
    ui->undoRuleButton->setEnabled(false);

    m_appFacade->refreshNlpEngine();
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::undoRule(BE::Rule *rule)
{
    if (!rule) {
        return;
    }
    if (!m_ruleEdited) {
        return;
    }

    // Refresh rule IO widgets
    if (rule->type() == BE::Rule::OrdinaryRule) {
        ui->ruleInputWidget->setTargets(m_ruleBackup.target());
        ui->ruleInputWidget->setInput(m_ruleBackup.input());
        ui->ruleOutputWidget->setOutput(m_ruleBackup.output());
    } else if (rule->type() == BE::Rule::EvasiveRule) {
        ui->ruleOutputWidget->setOutput(m_ruleBackup.output());
    } else if (rule->type() == BE::Rule::ContainerRule) {
        ui->categoryNameTextEdit->setText(m_ruleBackup.name());
    }

    // Refresh rule tree
    if (rule->type() == BE::Rule::OrdinaryRule) {
        QString ruleName = m_ruleBackup.input().isEmpty() ? "" : m_ruleBackup.input().first();
        m_ruleTreeModel->setData(m_ruleTreeModel->indexFromItem(rule),
                                 QVariant(ruleName), Qt::EditRole);
    } else if (rule->type() == BE::Rule::EvasiveRule) {
        // Nothing to do
    } else if (rule->type() == BE::Rule::ContainerRule) {
        m_ruleTreeModel->setData(m_ruleTreeModel->indexFromItem(rule),
                                 QVariant(m_ruleBackup.name()), Qt::EditRole);
    }

    m_ruleEdited = false;

    ui->teachRuleButton->setEnabled(false);
    ui->undoRuleButton->setEnabled(false);
}

//--------------------------------------------------------------------------------------------------
// Local test
//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onTestInputTextEntered()
{
    if (m_ruleEdited) {
        handleRuleEdited(selectedRule());
    }

    QString input = ui->testInputText->text();
    BE::AppFacade::MatchList matches;

    QString response = m_appFacade->getResponse(input, matches);

    ui->testConversationText->appendConversation(input, response, !matches.isEmpty());
    ui->testInputText->setText("");
    ui->clearTestConversationButton->setEnabled(true);

    highlightMatchedRules(matches);

    // Log conversation
    BE::Conversation::Entry entry(QDateTime::currentDateTime(),"test", "test", input, response,
                                  !matches.isEmpty());

    // Disabled conversation history
    //onNewChatConversation(entry);

    m_testConversationLog->write(entry);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onClearTestConversationButtonPressed()
{
    ui->clearTestConversationButton->setEnabled(false);

    ui->testConversationText->clear();
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::highlightMatchedRules(const BE::AppFacade::MatchList &matches)
{
    ui->ruleInputWidget->clearHighlight();
    ui->ruleOutputWidget->clearHighlight();

    if (!matches.empty()) {
        // Assuming only one match

        quint64 ruleId = matches.first().first;
        int inputNumber = matches.first().second;

        const BE::Rule *rule = findRule(ruleId);

        if (rule) {
            selectRule(rule);
            ui->ruleInputWidget->highlightInput(inputNumber);
        }
    } else {
        selectRule(evasivesRule());
        ui->ruleOutputWidget->highlightOuput(0);
    }
}

//--------------------------------------------------------------------------------------------------
// Verify account
//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onVerifyAccountButtonPressed()
{
    if (!ui->usernameText_v->text().isEmpty()) {
        setUiMode(VerifyAccountConnectingUiMode);

        m_appFacade->verifyAccount(uiChatSelected(),
                                   ui->usernameText_v->text(),
                                   ui->passwordText_v->text());
    } else {
        QMessageBox::information(this, tr("Invalid username"), tr("Please provide a username"));

        ui->usernameText_v->setFocus();
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onVerifyAccountOk(const BE::Roster &roster)
{
    ui->ruleInputWidget->setRoster(roster);

    BE::AppFacade::ChatType chatType = uiChatSelected();
    QString username = ui->usernameText_v->text();

    m_appFacade->setChatType(chatType);
    m_appFacade->setUsername(username);

    // persist roster
    saveRoster(roster, ::rosterFilename(::canonicAccount(username, chatType)));

    if (m_tabsLayout == VerifyAccountTabsLayout) {
        startEditMode();
    } else {
        setUiMode(ChatDisconnectedUiMode);
    }

    QMessageBox::information(this, tr("Account verified"), tr("Account verified!"));
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onVerifyAccountError(int err)
{
    setUiMode(VerifyAccountFailedUiMode);

    QString title;
    QString msg;

    if (err != BE::AppFacade::SSLNotSupportedError) {
        title = tr("Account error");
        msg   = tr("The account could not be verified. "
                   "Please check your username and password and internet connection.");
    } else {
        title = tr("SSL error");
        msg   = tr("The account could not be verified. "
                   "Your system does not support secure connections.");
    }

    QMessageBox::critical(this, title, msg);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onChangeAccountButtonPressed()
{
    QString title = tr("Change Account");
    QString msg   = tr("If you change your account some rules might not work anymore.\n"
                       "Are you sure you want to change your account?");

    QMessageBox::StandardButtons buttons = QMessageBox::Yes | QMessageBox::No;

    if (QMessageBox::question(this, title, msg, buttons) == QMessageBox::Yes) {
        setUiMode(ChangeAccountUiMode);
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onCancelChangeAccountButtonPressed()
{
    m_appFacade->cancelVerifyAccount();

    if (m_tabsLayout == VerifyAccountTabsLayout) {
        setUiMode(WelcomeTabUiMode);
    } else {
        setUiMode(ChatDisconnectedUiMode);
    }
}

//--------------------------------------------------------------------------------------------------
// Chat connection
//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onConnectButtonPressed()
{
    if (m_connectionStatus == DisconnectedFromChat || m_connectionStatus == ConnectionError) {
        m_connectionStatus =  ConnectingToChat;
        setUiMode(ChatConnectingUiMode);

        m_appFacade->connectToChat(ui->passwordText->text());
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onDisconnectButtonPressed()
{
    if (m_connectionStatus == ConnectedToChat || m_connectionStatus == ConnectingToChat) {
        m_connectionStatus = DisconnectedFromChat;
        setUiMode(ChatDisconnectedUiMode);

        m_appFacade->disconnectFromChat();
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onConnectionOk()
{
    m_connectionStatus = ConnectedToChat;
    setUiMode(ChatConnectionOkUiMode);

    BE::Roster roster = m_appFacade->roster();

    saveRoster(roster, rosterFilename());

    ui->ruleInputWidget->setRoster(roster);

    BE::Roster blackListRoster;

    if (!loadRoster(blackListRoster, blackRosterFilename())) {
        blackListRoster = roster; // By default all contacts are in the black list
    }

    ui->rosterWidget->setRoster(roster, blackListRoster);
    m_appFacade->setBlackListRoster(blackListRoster);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onConnectionError(int err)
{
    m_connectionStatus = ConnectionError;

    if (err != BE::AppFacade::SSLNotSupportedError) {
        setUiMode(ChatConnectionFailedUiMode);
    } else {
        setUiMode(ChatConnectionSSLFailedUiMode);
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onDisconnection()
{
    m_connectionStatus = DisconnectedFromChat;
    setUiMode(ChatDisconnectedUiMode);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onNewChatConversation(const BE::Conversation::Entry &entry)
{
    ui->chatHistory->addConversationEntry(entry);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onRosterSelectionChanged()
{
    updateBlackList();
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::updateBlackList()
{
    BE::Roster blackList = ui->rosterWidget->uncheckedRoster();

    m_appFacade->setBlackListRoster(blackList);

    saveRoster(blackList, blackRosterFilename());
}

//--------------------------------------------------------------------------------------------------

inline Lvk::BE::AppFacade::ChatType Lvk::FE::MainWindow::uiChatSelected()
{
    return ui->gtalkChatRadio_v->isChecked() ? BE::AppFacade::GTalkChat : BE::AppFacade::FbChat;
}

//--------------------------------------------------------------------------------------------------

inline QString Lvk::FE::MainWindow::rosterFilename()
{
    return ::rosterFilename(canonicAccount());
}

//--------------------------------------------------------------------------------------------------

inline QString Lvk::FE::MainWindow::blackRosterFilename()
{
    return ::blackRosterFilename(canonicAccount());
}

//--------------------------------------------------------------------------------------------------

inline QString Lvk::FE::MainWindow::canonicAccount()
{
    return ::canonicAccount(m_appFacade->username(), m_appFacade->chatType());
}

//--------------------------------------------------------------------------------------------------
// Misc events
//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onSplitterMoved(int, int)
{
    saveSplittersSettings();
}

//--------------------------------------------------------------------------------------------------
// Advanced Options
//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onRmDupCheckBoxChanged(int state)
{
    setNlpEngineOption(BE::AppFacade::RemoveDupChars, state == Qt::Checked);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onLemmatizerCheckBoxChanged(int state)
{
    setNlpEngineOption(BE::AppFacade::LemmatizeSentence, state == Qt::Checked);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::setNlpEngineOption(BE::AppFacade::NlpEngineOption opt, bool enabled)
{
    unsigned options = m_appFacade->nlpEngineOptions();

    options = enabled ? options | opt : options & ~opt;

    m_appFacade->setNlpEngineOptions(options);
}


