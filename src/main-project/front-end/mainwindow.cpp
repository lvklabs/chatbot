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

#include "mainwindow.h"
#include "coreapp.h"
#include "ruletreemodel.h"
#include "rule.h"
#include "ui_mainwindow.h"
#include "simpleaimlengine.h"
#include "defaultsanitizer.h"
#include "exportdialog.h"
#include "importdialog.h"
#include "roster.h"
#include "version.h"
#include "settings.h"
#include "settingskeys.h"

#include <QStandardItemModel>
#include <QItemDelegate>
#include <QInputDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QIcon>
#include <QFile>
#include <QFileDialog>
#include <QApplication>
#include <QDesktopWidget>

#define TEST_CONVERSATION_LOG_FILE  "test_conversations.log"
#define DEFAULT_RULES_FILE          "rules.crf"
#define APP_ICON_FILE               ":/icons/app_icon"

#define FILE_EXTENSION              QString(QObject::tr("crf"))
#define FILE_EXPORT_EXTENSION       QString(QObject::tr("cef"))

#define FB_ICON_FILE               ":/icons/facebook_24x24.png"
#define GMAIL_ICON_FILE            ":/icons/gmail_24x24.png"


typedef Lvk::Nlp::SimpleAimlEngine DefaultEngine;
typedef Lvk::Nlp::DefaultSanitizer DefaultSanitizer;


//--------------------------------------------------------------------------------------------------
// Non-member helpers
//--------------------------------------------------------------------------------------------------

namespace
{

// These two first helpers are workarounds to save black list settings with QSettings.
// See saveBlackListSettings() and loadBlackListSettings() methods for more details.

QString rosterUsersToString(const Lvk::BE::Roster &roster)
{
    QString s;
    foreach (const Lvk::BE::RosterItem &item, roster) {
        s.append(item.username).append(";");
    }

    return s;
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::Roster rosterUsersfromString(const QString &s)
{
    Lvk::BE::Roster roster;
    foreach (const QString &username, s.split(";")) {
        roster.append(Lvk::BE::RosterItem(username, ""));
    }

    return roster;
}

//--------------------------------------------------------------------------------------------------
// Cannonic representation for chat accounts used to persist some settings

QString canonicAccount(const QString &username, Lvk::BE::CoreApp::ChatType type)
{
    return username.trimmed().split("@").at(0) + "@" + QString::number(type);
}

//--------------------------------------------------------------------------------------------------
// Localized file filters for open/save dialogs

QString getFileFilters()
{
    return QObject::tr("Chatbot Rule Files") + QString(" (*.") + FILE_EXTENSION + QString(");;")
            + QObject::tr("All files") + QString(" (*.*)");
}

//--------------------------------------------------------------------------------------------------
// Localized export file filters for import/export dialogs

QString getFileExportFilters()
{
    return QObject::tr("Chatbot Export Files") + QString(" (*.") + FILE_EXPORT_EXTENSION
            + QString(");;") + QObject::tr("All files") + QString(" (*.*)");
}

} // namespace


//--------------------------------------------------------------------------------------------------
// Constructors, destructor and init methods
//--------------------------------------------------------------------------------------------------

Lvk::FE::MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_coreApp(new BE::CoreApp(new DefaultEngine(new DefaultSanitizer()), parent)),
    m_ruleTreeModel(0),
    m_ruleEdited(false),
    m_testConversationLog(new QFile(TEST_CONVERSATION_LOG_FILE)),
    m_connectionStatus(DisconnectedFromChat)
{
    ui->setupUi(this);
    ui->teachTabsplitter->setSizes(QList<int>() << 10 << 10);

    clear();

    Lvk::Common::Settings settings;
    QString lastFile = settings.value(SETTING_LAST_FILE, QString(DEFAULT_RULES_FILE)).toString();
    initCoreAndModelsWithFile(lastFile);

    connectSignals();

    ui->testInputText->installEventFilter(this);
    ui->ruleInputWidget->installEventFilter(this);
    ui->ruleOutputWidget->installEventFilter(this);

    ui->conversationHistory->setConversation(m_coreApp->conversationHistory());

    selectFirstRule();

    ui->categoriesTree->setFocus();

    setWindowIcon(QIcon(APP_ICON_FILE));

    m_testConversationLog->open(QFile::Append);

    loadAllSettings();
}

//--------------------------------------------------------------------------------------------------

Lvk::FE::MainWindow::~MainWindow()
{
    delete m_testConversationLog;
    delete m_coreApp;
    delete ui;
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::clear()
{
    initCoreAndModelsWithFile("");

    m_ruleEdited = false;

    // reset active tabs
    ui->mainTabWidget->setCurrentIndex(0);
    ui->rightSideTabWidget->setCurrentIndex(0);

    // train tab widgets
    // TODO clear ui->categoriesTree
    setUiMode(RuleSelectionEmptyUiMode);
    ui->ruleInputWidget->clear();
    ui->ruleOutputWidget->clear();

    // chat tab widgets
    //ui->fbChatRadio->setChecked(true);
    ui->rosterWidget->clear();
    m_connectionStatus = DisconnectedFromChat;
    m_coreApp->disconnectFromChat();

    setUiMode(ChatDisconnectedUiMode);
    ui->usernameText->clear();
    ui->passwordText->clear();

    // conversation tab widgets
    ui->conversationHistory->clear();

    // test tab widgets
    ui->testConversationText->clear();
    ui->testInputText->clear();
    ui->clearTestConversationButton->setEnabled(false);
}

//--------------------------------------------------------------------------------------------------

bool Lvk::FE::MainWindow::initCoreAndModelsWithFile(const QString &filename)
{
    bool success = true;

    setFilename(filename);

    if (!filename.isEmpty()) {
        success = m_coreApp->load(filename);
    } else {
        m_coreApp->close();
    }

    delete m_ruleTreeModel;

    m_ruleTreeModel = new FE::RuleTreeModel(m_coreApp->rootRule(), this);

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

    // Test tab

    connect(ui->testInputText, SIGNAL(returnPressed()), SLOT(onTestInputTextEntered()));

    connect(ui->clearTestConversationButton, SIGNAL(clicked()),
            SLOT(onClearTestConversationButtonPressed()));

    // Chat connetion tab

    connect(ui->connectButton,    SIGNAL(clicked()), SLOT(onConnectButtonPressed()));
    connect(ui->disconnectButton, SIGNAL(clicked()), SLOT(onDisconnectButtonPressed()));

    connect(m_coreApp, SIGNAL(connected()),          SLOT(onConnectionOk()));
    connect(m_coreApp, SIGNAL(disconnected()),       SLOT(onDisconnection()));
    connect(m_coreApp, SIGNAL(connectionError(int)), SLOT(onConnectionError(int)));

    connect(ui->passwordText, SIGNAL(returnPressed()), ui->connectButton, SLOT(click()));

    connect(ui->rosterWidget, SIGNAL(selectionChanged()), SLOT(onRosterSelectionChanged()));

    // Conversations tab

    connect(m_coreApp,
            SIGNAL(newConversationEntry(BE::Conversation::Entry)),
            SLOT(onNewChatConversation(BE::Conversation::Entry)));
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
            saveChanges();
            event->accept();
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
    loadMainWindowSettings();
    loadChatSettings();
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::saveAllSettings()
{
    saveMainWindowSettings();
    saveChatSettings();

    if (!m_filename.isEmpty()) {
        Lvk::Common::Settings settings;
        settings.setValue(SETTING_LAST_FILE, m_filename);
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::loadMainWindowSettings()
{
    Common::Settings settings;

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

void Lvk::FE::MainWindow::loadChatSettings()
{
    Common::Settings settings;

    int chatType = settings.value(SETTING_DEFAULT_CHAT_SERVER_TYPE).toInt();
    uiSelectChat(static_cast<BE::CoreApp::ChatType>(chatType));

    ui->usernameText->setText(settings.value(SETTING_DEFAULT_CHAT_USERNAME).toString());
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::saveMainWindowSettings()
{
    Common::Settings settings;

    settings.setValue(SETTING_MAIN_WINDOW_SIZE, size());
    settings.setValue(SETTING_MAIN_WINDOW_POS, pos());
    settings.setValue(SETTING_MAIN_WINDOW_MAXIMIZED, isMaximized());
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::saveChatSettings()
{
    Common::Settings settings;

    settings.setValue(SETTING_DEFAULT_CHAT_SERVER_TYPE, uiChatSelected());
    settings.setValue(SETTING_DEFAULT_CHAT_USERNAME, ui->usernameText->text());
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::Roster Lvk::FE::MainWindow::getBlackListSettings(const QString &chatAccount)
{
    Common::Settings settings;

    QString key = SETTING_BLACK_LIST_ROSTER + QString("/") + chatAccount;

    // Not working:
    //
    // QVariant v = settings.value(key);
    // BE::Roster blackList = v.value<BE::Roster>();
    //
    // Workaround:

    QString s = settings.value(key).toString();
    return rosterUsersfromString(s);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::saveBlackListSettings(const BE::Roster &blackList,
                                                const QString &chatAccount)
{
    Common::Settings settings;

    QString key = SETTING_BLACK_LIST_ROSTER + QString("/") + chatAccount;

    // Not working:
    //
    // QVariant v;
    // v.setValue(ui->rosterWidget->uncheckedRoster());
    // settings.setValue(key, v);
    //
    // Workaround:

    settings.setValue(key, rosterUsersToString(blackList));
}


//--------------------------------------------------------------------------------------------------
// UI Modes
//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::setUiMode(UiMode mode)
{
    switch (mode) {

    // Edit rules tab //////////////////////////////////////////////////////

    case RuleSelectionEmptyUiMode:
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

    // Chat connection tab /////////////////////////////////////////////////

    case ChatDisconnectedUiMode:
        ui->connectToChatStackWidget->setCurrentIndex(0);
        ui->fbChatRadio->setEnabled(true);
        ui->gtalkChatRadio->setEnabled(true);
        ui->usernameText->setEnabled(true);
        ui->passwordText->setEnabled(true);
        ui->connectButton->setText(tr("Connect"));
        ui->connectionProgressBar->setVisible(false);
        ui->connectionStatusLabel->setText(tr("Disconnected"));
        ui->connectionStatusLabel->setStyleSheet("");
        break;

    case ChatConnectingUiMode:
        ui->connectToChatStackWidget->setCurrentIndex(0);
        ui->fbChatRadio->setEnabled(false);
        ui->gtalkChatRadio->setEnabled(false);
        ui->usernameText->setEnabled(false);
        ui->passwordText->setEnabled(false);
        ui->connectButton->setText(tr("Disconnect"));
        ui->connectionProgressBar->setVisible(true);
        ui->connectionStatusLabel->setText(tr("Connecting..."));
        ui->connectionStatusLabel->setStyleSheet("");
        break;

    case ChatConnectionFailedUiMode:
        ui->connectToChatStackWidget->setCurrentIndex(0);
        ui->fbChatRadio->setEnabled(true);
        ui->gtalkChatRadio->setEnabled(true);
        ui->usernameText->setEnabled(true);
        ui->passwordText->setEnabled(true);
        ui->connectButton->setText(tr("Connect"));
        ui->connectionProgressBar->setVisible(false);
        ui->connectionStatusLabel->setText(tr("Connection error"));
        ui->connectionStatusLabel->setStyleSheet("color:red");
        break;

    case ChatConnectionOkUiMode:
        ui->connectToChatStackWidget->setCurrentIndex(1);
        ui->disconnectButton->setText(tr("Disconnect ") + ui->usernameText->text());
        ui->disconnectButton->setIcon(ui->fbChatRadio->isChecked() ?
                                          QIcon(FB_ICON_FILE) : QIcon(GMAIL_ICON_FILE));
        // Not visible anymore:
        ui->fbChatRadio->setEnabled(false);
        ui->gtalkChatRadio->setEnabled(false);
        ui->usernameText->setEnabled(false);
        ui->passwordText->setEnabled(false);
        ui->connectButton->setText(tr("Disconnect"));
        ui->connectionProgressBar->setVisible(false);
        ui->connectionStatusLabel->setText(tr("Connection sucessful!"));
        ui->connectionStatusLabel->setStyleSheet("color:green");
        break;
    }
}

//--------------------------------------------------------------------------------------------------
// Shortcuts to special rules
//--------------------------------------------------------------------------------------------------

Lvk::BE::Rule * Lvk::FE::MainWindow::rootRule()
{
    return m_coreApp->rootRule();
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::Rule * Lvk::FE::MainWindow::evasivesRule()
{
    return m_coreApp->evasivesRule();
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::Rule * Lvk::FE::MainWindow::selectedRule()
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
            load(filename);
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

        m_coreApp->save();

        saved = true;
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

        setFilename(filename);

        if (m_ruleEdited) {
            teachRule(selectedRule());
        }

        m_coreApp->saveAs(m_filename);

        saved = true;
    }

    return saved;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::FE::MainWindow::load(const QString &filename)
{
    clear();

    bool success = initCoreAndModelsWithFile(filename);

    if (!success) {
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
    return m_ruleEdited || m_coreApp->hasUnsavedChanges();
}

//--------------------------------------------------------------------------------------------------

int Lvk::FE::MainWindow::showSaveChangesDialog()
{
    QString text = !m_filename.isEmpty() ?
                tr("Do you want to save the changes in " + m_filename + "?") :
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
        if (m_coreApp->importRules(&container, filename)) {
            FE::RuleTreeModel *model = new FE::RuleTreeModel(&container, this);

            FE::ImportDialog importDialog(IMPORT_TITLE, model, this);

            BE::Rule selectedRulesContainer;
            if (importDialog.exec(&selectedRulesContainer) == QDialog::Accepted) {

                // FIXME do not use notifyDataAboutToChange
                m_ruleTreeModel->notifyDataAboutToChange();
                if (!m_coreApp->mergeRules(&selectedRulesContainer)) {
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

    FE::RuleTreeModel *model = new FE::RuleTreeModel(m_coreApp->rootRule(), this);

    FE::ExportDialog exportDialog(EXPORT_TITLE, model, this);

    BE::Rule container;
    if (exportDialog.exec(&container) == QDialog::Accepted) {

        QString filename = QFileDialog::getSaveFileName(this, EXPORT_TITLE , "",
                                                        getFileExportFilters());

        if (!filename.isEmpty()) {
            if (!filename.endsWith("." + FILE_EXPORT_EXTENSION)) {
                filename.append("." + FILE_EXPORT_EXTENSION);
            }

            if (!m_coreApp->exportRules(&container, filename)) {
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
    bool ok;
    QString name = QInputDialog::getText(this, tr("Add category"), tr("Category name:"),
                                         QLineEdit::Normal, "", &ok);

    if (ok) {
        if (!name.isEmpty()) {
            BE::Rule *category = addCategory(name);

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
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onAddRuleButtonClicked()
{
    if (m_ruleTreeSelectionModel->selectedRows().isEmpty()) {
        QString title = tr("Add rule");
        QString text = tr("Select the category where the rule will belong to");
        QMessageBox::information(this, title, text);

        return;
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
        BE::Rule *emptyRule = addRule("", parentCategory);

        if (emptyRule) {
            selectRule(emptyRule);
            ui->ruleInputWidget->setFocusOnInput();
        } else {
            QString title = tr("Internal error");
            QString text = tr("The rule could not be added because of an internal error");
            QMessageBox::critical(this, title, text);
        }
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onRemoveButtonClicked()
{
    if (m_ruleTreeSelectionModel->selectedRows().isEmpty()) {
        QString text = tr("Remove rule or category");
        QString title = tr("Select the rule or category you want to remove");
        QMessageBox::critical(this, text, title);

        return;
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
            bool removed = m_ruleTreeModel->removeRow(selectedIndex.row(), selectedIndex.parent());

            if (!removed) {
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
        ui->ruleInputWidget->setInputList(rule->input());
        ui->ruleOutputWidget->setOutputList(rule->output());
    } else if (rule->type() == BE::Rule::EvasiveRule) {
        setUiMode(EditEvasivesUiMode);
        ui->categoryNameTextEdit->clear();
        ui->ruleInputWidget->clear();
        ui->ruleOutputWidget->setOutputList(rule->output());
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

void Lvk::FE::MainWindow::onRuleEdited()
{
    const BE::Rule *selRule = selectedRule();

    if (selRule && !m_ruleEdited) {
        m_ruleEdited = true;

        m_ruleBackup.setStatus(selRule->status());
        m_ruleBackup.setName(selRule->name());
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
        rule->setInput(ui->ruleInputWidget->inputList());
        rule->setOutput(ui->ruleOutputWidget->outputList());
    } else if (rule->type() == BE::Rule::EvasiveRule) {
        rule->setOutput(ui->ruleOutputWidget->outputList());
    } else if (rule->type() == BE::Rule::ContainerRule) {
        rule->setName(ui->categoryNameTextEdit->text());
    }

    m_ruleEdited = false;

    ui->teachRuleButton->setEnabled(false);
    ui->undoRuleButton->setEnabled(false);

    m_coreApp->refreshNlpEngine();
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
        ui->ruleInputWidget->setInputList(m_ruleBackup.input());
        ui->ruleOutputWidget->setOutputList(m_ruleBackup.output());
    } else if (rule->type() == BE::Rule::EvasiveRule) {
        ui->ruleOutputWidget->setOutputList(m_ruleBackup.output());
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
    BE::CoreApp::MatchList matches;

    QString response = m_coreApp->getResponse(input, matches);

    ui->testConversationText->appendConversation(input, response, !matches.isEmpty());

    ui->testInputText->setText("");

    ui->clearTestConversationButton->setEnabled(true);

    highlightMatchedRules(matches);

    // Log conversation
    BE::Conversation::Entry convEntry(QDateTime::currentDateTime(),
                                      "test", "test",
                                      input, response, !matches.isEmpty());

    // Disabled conversation history
    //onNewChatConversation(convEntry);

    logTestConversation(convEntry);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onClearTestConversationButtonPressed()
{
    ui->clearTestConversationButton->setEnabled(false);

    ui->testConversationText->clear();
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::highlightMatchedRules(const BE::CoreApp::MatchList &matches)
{
    ui->ruleInputWidget->clearHighlight();
    ui->ruleOutputWidget->clearHighlight();

    if (!matches.empty()) {
        // Assuming only one match

        const BE::Rule *rule = matches.first().first;
        int ruleNumber = matches.first().second;

        selectRule(rule);

        ui->ruleInputWidget->highlightInput(ruleNumber);
    } else {
        selectRule(evasivesRule());

        ui->ruleOutputWidget->highlightOuput(0); // FIXME hardcoded 0
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::logTestConversation(const BE::Conversation::Entry &entry)
{
    if (m_testConversationLog) {
        m_testConversationLog->write(entry.toString() + "\n");
        m_testConversationLog->flush();
    }
}

//--------------------------------------------------------------------------------------------------
// Chat connection
//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onConnectButtonPressed()
{
    if (m_connectionStatus == DisconnectedFromChat || m_connectionStatus == ConnectionError) {
        if (!ui->usernameText->text().isEmpty()) {

            m_connectionStatus =  ConnectingToChat;
            setUiMode(ChatConnectingUiMode);

            m_coreApp->connectToChat(uiChatSelected(),
                                     ui->usernameText->text(),
                                     ui->passwordText->text());
        } else {
            QMessageBox::information(this, tr("Invalid username"), tr("Please provide a username"));

            ui->usernameText->setFocus();
        }
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onDisconnectButtonPressed()
{
    if (m_connectionStatus == ConnectedToChat || m_connectionStatus == ConnectingToChat) {
        m_connectionStatus = DisconnectedFromChat;
        setUiMode(ChatDisconnectedUiMode);

        m_coreApp->disconnectFromChat();
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onConnectionOk()
{
    m_connectionStatus = ConnectedToChat;
    setUiMode(ChatConnectionOkUiMode);

    BE::Roster roster = m_coreApp->roster();

    QString account = canonicAccount(ui->usernameText->text(), uiChatSelected());
    BE::Roster blackListRoster = getBlackListSettings(account);

    ui->ruleInputWidget->setRoster(roster);
    ui->rosterWidget->setRoster(roster, blackListRoster);
    m_coreApp->setBlackListRoster(blackListRoster);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onConnectionError(int err)
{
    m_connectionStatus = ConnectionError;
    setUiMode(ChatConnectionFailedUiMode);

    ui->rosterWidget->clear();
    ui->connectionStatusLabel->setText(ui->connectionStatusLabel->text() + " #" +
                                       QString::number(err));
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onDisconnection()
{
    if (m_connectionStatus != ConnectionError) {
        m_connectionStatus = DisconnectedFromChat;
        setUiMode(ChatDisconnectedUiMode);

        ui->rosterWidget->clear();
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onNewChatConversation(const BE::Conversation::Entry &entry)
{
    ui->conversationHistory->addConversationEntry(entry);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onRosterSelectionChanged()
{
    updateBlackList();
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::CoreApp::ChatType Lvk::FE::MainWindow::uiChatSelected()
{
    return ui->gtalkChatRadio->isChecked() ? BE::CoreApp::GTalkChat : BE::CoreApp::FbChat;
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::uiSelectChat(BE::CoreApp::ChatType type)
{
    if (type == BE::CoreApp::GTalkChat) {
        ui->gtalkChatRadio->setChecked(true);
    } else if (type == BE::CoreApp::FbChat) {
        ui->fbChatRadio->setChecked(true);
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::updateBlackList()
{
    QString account = canonicAccount(ui->usernameText->text(), uiChatSelected());

    BE::Roster blackList = ui->rosterWidget->uncheckedRoster();

    m_coreApp->setBlackListRoster(blackList);

    saveBlackListSettings(blackList, account);
}

