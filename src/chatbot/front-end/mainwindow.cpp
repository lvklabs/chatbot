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
#include "front-end/ruletreemodel.h"
#include "front-end/exportdialog.h"
#include "front-end/importdialog.h"
#include "front-end/scorewidget.h"
#include "front-end/tinyscorewidget.h"
#include "front-end/sendscoredialog.h"
#include "front-end/optionswindow.h"
#include "front-end/filedialog.h"
#include "front-end/newupdatedialog.h"
#include "front-end/updateexecutor.h"
#include "front-end/memberfunctor.h"
#include "front-end/uploaderprogressdialog.h"
#include "back-end/appfacade.h"
#include "back-end/rule.h"
#include "back-end/roster.h"
#include "stats/score.h"
#include "common/conversationwriter.h"
#include "common/version.h"
#include "common/settings.h"
#include "common/settingskeys.h"
#include "common/globalstrings.h"
#include "da-server/updater.h"
#include "da-server/contestdata.h"
#include "ui_mainwindow.h"

#include <QStandardItemModel>
#include <QItemDelegate>
#include <QInputDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QIcon>
#include <QFile>
#include <QDir>
#include <QApplication>
#include <QDesktopWidget>
#include <QtDebug>


//--------------------------------------------------------------------------------------------------
// Constructors, destructor and init methods
//--------------------------------------------------------------------------------------------------

Lvk::FE::MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_appFacade(new BE::AppFacade(this)),
    m_refactor(ui, m_appFacade),
    m_ruleTreeModel(0),
    m_ruleEdited(false),
    m_ruleAdded(false),
    m_tinyScore(0)
{
    qDebug() << "Setting up main window...";

    setupUi();

    loadAllSettings();

    qDebug() << "Main window created!";

    FE::UpdateExecutor::exec(new FE::MemberFunctor<MainWindow>(this, &MainWindow::onUpdate));
}

//--------------------------------------------------------------------------------------------------

Lvk::FE::MainWindow::~MainWindow()
{
    delete m_appFacade;
    delete ui;

    qDebug() << "Main window destroyed cleanly!";
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::setupUi()
{
    ui->setupUi(this);

    ui->connectionWidget->setAppFacade(m_appFacade);
    ui->verificationWidget->setAppFacade(m_appFacade);
    ui->clueWidget->setAppFacade(m_appFacade);

    // Deprecated:
    //ui->teachTabsplitter->setBackgroundColor(QColor(0,0,0,0));
    // Instead
    QPalette palette;
    palette.setColor(ui->teachTabsplitter->backgroundRole(), QColor(0,0,0,0));
    ui->teachTabsplitter->setPalette(palette);

    ui->curScoreWidget->setUploadVisible(false);

    m_tinyScore = new TinyScoreWidget(ui->mainTabWidget);
    m_tinyScore->setVisible(false);

    clear();

    connectSignals();

    QMenu *menu = new QMenu(this);
    menu->addAction(ui->actionAddEmptyRule);
    menu->addAction(ui->actionAddVarRule);
    menu->addAction(ui->actionAddCondRule);
    ui->addRuleButton->setMenu(menu);

#ifdef DA_CONTEST
    setWindowIcon(QIcon(APP_ICON_FILE));
#endif
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
        initWithFile("");
    }

    m_appFacade->disconnectFromChat();

    if (m_ruleEdited) {
        m_ruleEdited = false;
        ruleEditFinished();
    }
    m_ruleAdded = false;

    // reset active tabs
    ui->mainTabWidget->setCurrentIndex(0);

    // verify tab widgets
    ui->verificationWidget->clear();

    // train tab widgets
    ui->ruleEditWidget->clearAll();

    // connect tab widgets
    ui->connectionWidget->clear();

    // conversation tab widgets
    ui->chatHistory->clear();

    // test tab widgets
    // TODO create a new widget to handle this:
    ui->testConversationText->clear();
    ui->testInputText->clearAll();
    ui->clearTestConvButton->setEnabled(false);
    ui->categoryGroupBox->setVisible(false);
    ui->categoryLabel->clear();
    ui->ruleView->clear();
    ui->ruleViewGroupBox->setVisible(false);
    ui->transfView->clear();
    ui->transfViewGroupBox->setVisible(false);

    // score tab widgets
    ui->curScoreWidget->clear();
    ui->bestScoreWidget->clear();
    ui->remainingTimeLabel->clear();
    m_tinyScore->clear();

    // Clue tab widgets
    ui->clueWidget->clear();

    setUiMode(FE::WelcomeTabUiMode);
}

//--------------------------------------------------------------------------------------------------

bool Lvk::FE::MainWindow::initWithFile(const QString &filename, bool newFile)
{
    bool success = true;

    setFilename(filename);

    if (!filename.isEmpty()) {
        if (newFile) {
            success = m_appFacade->newFile(filename);
        } else {
            success = m_appFacade->load(filename);
        }
    }

    delete m_ruleTreeModel;
    m_ruleTreeModel = new FE::RuleTreeModel(m_appFacade->rootRule(), this);
    ui->categoriesTree->setModel(m_ruleTreeModel);

    m_ruleTreeSelectionModel = ui->categoriesTree->selectionModel();

    setPccWidgetsVisible(nlpEngineOption(BE::AppFacade::PreferCurCategory));

    connect(m_ruleTreeModel,
            SIGNAL(rowsRemoved(QModelIndex, int, int)),
            SLOT(onRulesRemoved(QModelIndex, int, int)));

    connect(m_ruleTreeSelectionModel,
            SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            SLOT(onRuleSelectionChanged(QItemSelection,QItemSelection)));

    return success;
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::connectSignals()
{
    // Menus
    connect(ui->actionNew,             SIGNAL(triggered()), SLOT(onNewMenuTriggered()));
    connect(ui->actionOpen,            SIGNAL(triggered()), SLOT(onOpenMenuTriggered()));
    connect(ui->actionSave,            SIGNAL(triggered()), SLOT(onSaveMenuTriggered()));
    connect(ui->actionSaveAs,          SIGNAL(triggered()), SLOT(onSaveAsMenuTriggered()));
    connect(ui->actionAbout,           SIGNAL(triggered()), SLOT(onAboutMenuTriggered()));
    connect(ui->actionExit,            SIGNAL(triggered()), SLOT(onExitMenuTriggered()));
    connect(ui->actionImport,          SIGNAL(triggered()), SLOT(onImportMenuTriggered()));
    connect(ui->actionExport,          SIGNAL(triggered()), SLOT(onExportMenuTriggered()));
    connect(ui->actionOptions,         SIGNAL(triggered()), SLOT(onOptionsMenuTriggered()));

    // init tab
    connect(ui->openChatbotButton,     SIGNAL(clicked()),   SLOT(onOpenMenuTriggered()));
    connect(ui->openLastChatbotButton, SIGNAL(clicked()),   SLOT(onOpenLastFileMenuTriggered()));
    connect(ui->createChatbotButton,   SIGNAL(clicked()),   SLOT(onNewMenuTriggered()));

    // verification tab
    connect(ui->verificationWidget,    SIGNAL(accountOk()), SLOT(onVerifyAccountOk()));
    connect(ui->verificationWidget,    SIGNAL(verificationSkipped()),
            SLOT(onVerifyAccountSkipped()));
    connect(ui->verificationWidget,    SIGNAL(verificationCanceled()),
            SLOT(onVerifyAccountCanceled()));
    connect(ui->verificationWidget,      SIGNAL(accountError(int, QString)),
            SLOT(onVerifyAccountError(int, QString)));

    // Edit rules tabs
    connect(ui->addCategoryButton,     SIGNAL(clicked()),   SLOT(onAddCategoryButtonClicked()));
    connect(ui->addRuleButton,         SIGNAL(clicked()),   SLOT(onAddRuleButtonClicked()));
    connect(ui->rmItemButton,          SIGNAL(clicked()),   SLOT(onRemoveButtonClicked()));
    connect(ui->actionAddEmptyRule,    SIGNAL(triggered()), SLOT(onAddRuleButtonClicked()));
    connect(ui->actionAddVarRule,      SIGNAL(triggered()), SLOT(onAddVarRuleAction()));
    connect(ui->actionAddCondRule,     SIGNAL(triggered()), SLOT(onAddCondRuleAction()));
    connect(ui->ruleEditWidget,        SIGNAL(teachRule()), SLOT(onTeachRule()));
    connect(ui->ruleEditWidget,        SIGNAL(undoRule()),  SLOT(onUndoRule()));
    connect(ui->ruleEditWidget,        SIGNAL(ruleEdited()),SLOT(onRuleEdited()));
    connect(ui->ruleEditWidget,        SIGNAL(ruleInputEdited(QString)),
            SLOT(onRuleInputEdited(QString)));
    connect(ui->centralSplitter,       SIGNAL(splitterMoved(int,int)),
            SLOT(onSplitterMoved(int,int)));
    connect(ui->teachTabsplitter,      SIGNAL(splitterMoved(int,int)),
            SLOT(onSplitterMoved(int,int)));

    // Test tab
    connect(ui->testInputText,         SIGNAL(testInputEntered()), SLOT(onTestInputTextEntered()));
    connect(ui->testInputText,         SIGNAL(currentItemChanged()), SLOT(onTestTargetChanged()));
    connect(ui->clearTestConvButton,   SIGNAL(clicked()),       SLOT(onClearTestConvPressed()));
    connect(ui->showRuleDefButton,     SIGNAL(clicked()),       SLOT(onTestShowRule()));

    // Chat connetion tab
    connect(ui->connectionWidget,      SIGNAL(connected()),     SLOT(onConnectionOk()));
    connect(ui->connectionWidget,      SIGNAL(disconnected()),  SLOT(onDisconnection()));
    connect(ui->connectionWidget,      SIGNAL(connectionError(int)),
            SLOT(onConnectionError(int)));
    connect(ui->connectionWidget,      SIGNAL(rosterSelectionChanged()),
            SLOT(onRosterSelChanged()));
    connect(ui->connectionWidget,      SIGNAL(accountChanged()),     SLOT(onVerifyAccountOk()));
    connect(ui->connectionWidget,      SIGNAL(changeAccountError(int, QString)),
            SLOT(onVerifyAccountError(int, QString)));

    // Conversation history tab
    connect(ui->chatHistory,          SIGNAL(teachRule(QString)),SLOT(onTeachFromHistory(QString)));
    connect(ui->chatHistory,          SIGNAL(showRule(quint64)), SLOT(onHistoryShowRule(quint64)));
    connect(ui->chatHistory,          SIGNAL(removedAll()),      SLOT(onRemovedAllHistory()));
    connect(ui->chatHistory,          SIGNAL(removed(QDate,QString)),
            SLOT(onRemovedHistory(QDate,QString)));
    connect(m_appFacade,              SIGNAL(newConversationEntry(Cmn::Conversation::Entry)),
            SLOT(onNewChatConversation(Cmn::Conversation::Entry)));

    // Score tab
    connect(ui->bestScoreWidget,      SIGNAL(upload()),          SLOT(onUploadScore()));
    connect(m_appFacade,              SIGNAL(scoreRemainingTime(int)),
            SLOT(onScoreRemainingTime(int)));

    // Clue tab
    connect(ui->clueWidget,           SIGNAL(upload()),          SLOT(onUploadScore()));
    connect(ui->clueWidget,           SIGNAL(showRule(quint64)), SLOT(onClueShowRule(quint64)));

    // Misc
    connect(ui->mainTabWidget,        SIGNAL(currentChanged(QWidget*)),
            SLOT(onCurrentTabChanged(QWidget*)));
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::setUiMode(FE::UiMode mode)
{
    m_refactor.setUiMode(mode);
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

bool Lvk::FE::MainWindow::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::Show:
    case QEvent::WindowStateChange:
    case QEvent::Resize:
    case QEvent::WindowActivate:
        updateTinyScorePos();
        break;
    default:
        //Nothing to do
        break;
    }

    return QMainWindow::event(event);
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
        Cmn::Settings settings(Cmn::Settings::UserScope(), this);
        settings.setValue(SETTING_LAST_FILE, m_filename);
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
    if (m_refactor.uiTabsLayout() == TeachTabsLayout) {
        Cmn::Settings settings;

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
        newFile("");
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
        QString filename = FileDialog::openChatbot(this);

        openFile(filename);
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onOpenLastFileMenuTriggered()
{
    openLastFile();
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onSaveMenuTriggered()
{
    saveChanges();
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onSaveAsMenuTriggered()
{
    QString title = tr("Save As...");
    QString msg = tr("With this option you are creating a new chatbot. The new chatbot "
                     "will not keep your current chat history and score.");

    QMessageBox::StandardButton button =
            QMessageBox::information(this, title, msg, QMessageBox::Ok, QMessageBox::Cancel);

    if (button == QMessageBox::Ok) {
        saveAsChanges();
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::newFile(const QString &filename_)
{

    QString filename = filename_;

    if (filename.isEmpty()) {
        filename = FileDialog::newChatbot(this);
    }

    if (!filename.isEmpty()) {
        clear();
        if (initWithFile(filename, true)) {
            setUiMode(FE::VerifyAccountUiMode);
        } else {
            QMessageBox::critical(this, tr("New File"), tr("Could not create file. "
                                 "Please verify that you have write permissions or enough space."));
        }
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::openFile(const QString &filename)
{
    if (!filename.isEmpty()) {
        if (load(filename)) {
            startEditMode();
        } else {
            clear();
        }
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::openLastFile()
{
    Cmn::Settings settings(Cmn::Settings::UserScope(), this);
    QString lastFile = settings.value(SETTING_LAST_FILE, QString()).toString();

    if (!lastFile.isEmpty()) {
        if (QFile::exists(lastFile)) {
            if (load(lastFile)) {
                startEditMode();
            } else {
                clear();
            }
        } else {
            QString msg = tr("Last file opened does not exist anymore: '%1'");
            QMessageBox::critical(this, tr("Open last file"), msg.arg(lastFile));
            Cmn::Settings(Cmn::Settings::UserScope()).remove(SETTING_LAST_FILE);
            setFilename("");
            setUiMode(FE::WelcomeTabUiMode);
        }
    }
}

//--------------------------------------------------------------------------------------------------

bool Lvk::FE::MainWindow::saveChanges()
{
    bool saved = false;

    if (m_filename.isEmpty()) {
        saved = saveAsChanges();
    } else {
        if (m_ruleEdited || m_ruleAdded) {
            teachRule(selectedRule());
        }

        saved = m_appFacade->save();

        if (!saved) {
            QMessageBox::critical(this, tr("Save File"), tr("Could not save file. "
                                 "Please verify that you have write permissions or enough space."));
        }
    }

    return saved;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::FE::MainWindow::saveAsChanges()
{
    QString filename = FileDialog::saveChatbot(this);

    bool saved = false;

    if (!filename.isEmpty()) {

        if (m_ruleEdited || m_ruleAdded) {
            teachRule(selectedRule());
        }

        saved = m_appFacade->saveAs(filename);

        if (saved) {
            setFilename(filename);
            ui->chatHistory->clear();
            updateScore();
            onScoreRemainingTime(m_appFacade->scoreRemainingTime());
        } else  {
            QMessageBox::critical(this, tr("Save File"), tr("Could not save file. "
                                 "Please verify that you have write permissions or enough space."));
        }
    }

    return saved;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::FE::MainWindow::load(const QString &filename)
{
    clear(false);

    bool success = initWithFile(filename);

    if (success) {
        ui->chatHistory->setConversation(m_appFacade->chatHistory());
        ui->ruleEditWidget->setRoster(m_appFacade->roster());
        ui->testInputText->setRoster(m_appFacade->roster());

        updateScore();
        onScoreRemainingTime(m_appFacade->scoreRemainingTime());
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
    return m_ruleEdited || m_ruleAdded || m_appFacade->hasUnsavedChanges();
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
    QString filename = FileDialog::openExport(this);

    if (!filename.isEmpty()) {
        const QString IMPORT_TITLE = tr("Import Rules");

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

        QString filename = FileDialog::saveExport(this);

        if (!filename.isEmpty()) {
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
    QString title = QString(tr("About %1...")).arg(APP_NAME);
#ifdef DA_CONTEST
    QString text = QString(tr(
        "<p><b>%1 %2</b><br/>Rev: %3</p>"
        "<p>Developed by LVK in conjuntion with FaMAF - UNC for the Manuel Sadosky Foundation</p>"
        "<a href=\"http://%4\">www.lvklabs.com</a>"
        "<br/><a href=\"http://www.famaf.unc.edu.ar/\">www.famaf.unc.edu.ar</a>"
        "<br/><a href=\"http://www.fundacionsadosky.org.ar\">www.fundacionsadosky.org.ar</a>"
        "<br/><p>Chatbot is free software: you can redistribute it and/or modify "
        "it under the terms of the GNU General Public License as published by "
        "the Free Software Foundation, either version 3 of the License, or "
        "(at your option) any later version.</p>"
        "<p>Chatbot is distributed in the hope that it will be useful, "
        "but WITHOUT ANY WARRANTY; without even the implied warranty of "
        "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the "
        "GNU General Public License for more details.</p>"))
            .arg(APP_NAME)
            .arg(APP_VERSION_STR)
            .arg(QString(APP_VERSION_REV).mid(0, 30))
            .arg(APP_URL_CONTACT);
#else
    QString text = QString(tr(
        "<p><b>%1 %2 - Open Source Edition</b><br/>Rev: %3</p>"
        "<p>Developed by LVK in conjuntion with FaMAF</p>"
        "<a href=\"http://%4\">www.lvklabs.com</a>"
        "<br/><a href=\"http://www.famaf.unc.edu.ar/\">www.famaf.unc.edu.ar</a>"
        "<br/><p>Chatbot is free software: you can redistribute it and/or modify "
        "it under the terms of the GNU General Public License as published by "
        "the Free Software Foundation, either version 3 of the License, or "
        "(at your option) any later version.</p>"
        "<p>Chatbot is distributed in the hope that it will be useful, "
        "but WITHOUT ANY WARRANTY; without even the implied warranty of "
        "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the "
        "GNU General Public License for more details.</p>"))
            .arg(APP_NAME)
            .arg(APP_VERSION_STR)
            .arg(QString(APP_VERSION_REV).mid(0, 30))
            .arg(APP_URL_CONTACT);
#endif

    QMessageBox msg(QMessageBox::NoIcon, title, text);
#ifdef DA_CONTEST
    msg.setIconPixmap(QPixmap(APP_ICON_FILE));
#endif
    msg.exec();
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onOptionsMenuTriggered()
{
    OptionsWindow::Options curOpt;
    curOpt.ignoreDupChars = nlpEngineOption(BE::AppFacade::RemoveDupChars);
    curOpt.lemmatizeSentence = nlpEngineOption(BE::AppFacade::LemmatizeSentence);
    curOpt.exactMatchSupport = nlpEngineOption(BE::AppFacade::ExactMatchSupport);
    curOpt.preferCurCategory = nlpEngineOption(BE::AppFacade::PreferCurCategory);

    OptionsWindow optWindow(curOpt, this);

    if (optWindow.exec() == QDialog::Accepted) {
        OptionsWindow::Options newOpt = optWindow.options();

        if (newOpt.ignoreDupChars != curOpt.ignoreDupChars) {
            setNlpEngineOption(BE::AppFacade::RemoveDupChars, newOpt.ignoreDupChars);
        }
        if (newOpt.lemmatizeSentence != curOpt.lemmatizeSentence) {
            setNlpEngineOption(BE::AppFacade::LemmatizeSentence, newOpt.lemmatizeSentence);
        }
        if (newOpt.exactMatchSupport != curOpt.exactMatchSupport) {
            setNlpEngineOption(BE::AppFacade::ExactMatchSupport, newOpt.exactMatchSupport);
        }
        if (newOpt.preferCurCategory != curOpt.preferCurCategory) {
            setNlpEngineOption(BE::AppFacade::PreferCurCategory, newOpt.preferCurCategory);
            setPccWidgetsVisible(newOpt.preferCurCategory);
        }
    }
}

//--------------------------------------------------------------------------------------------------

bool Lvk::FE::MainWindow::nlpEngineOption(BE::AppFacade::NlpEngineOption option)
{
    return m_appFacade->nlpEngineOptions() & option;
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::setNlpEngineOption(BE::AppFacade::NlpEngineOption opt, bool enabled)
{
    unsigned options = m_appFacade->nlpEngineOptions();

    options = enabled ? options | opt : options & ~opt;

    m_appFacade->setNlpEngineOptions(options);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::setPccWidgetsVisible(bool visible)
{
    ui->ruleEditWidget->setNextCategoryVisible(visible);
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
    addRuleWithDialog(QStringList(), QStringList());
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onRemoveButtonClicked()
{
    removeSelectedRuleWithDialog();
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onAddVarRuleAction()

{
    addRuleWithDialog(QStringList() << tr("left text [variable] right text"),
                      QStringList() << tr("left text [variable] right text"));
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onAddCondRuleAction()
{
    addRuleWithDialog(QStringList() << tr("left text [variable] right text"),
                      QStringList() << tr("{if [variable] == value} response 1"
                                          " {else} response 2"));
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::Rule * Lvk::FE::MainWindow::addRuleWithDialog(const QStringList &tmplInput,
                                                       const QStringList &tmplOutput)
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
            rule->setInput(tmplInput);
            rule->setOutput(tmplOutput);
            selectRule(rule);

            onRuleAdded();

//            if (tmplInput.size() > 0 || tmplOutput.size() > 0) {
//                onRuleEdited();
//            }

            ui->ruleEditWidget->setFocusOnInput();
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
    category->setId(m_appFacade->nextRuleId());

    if (!added) {
        delete category;
        return 0;
    }

    return category;
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::Rule *Lvk::FE::MainWindow::addRule(const QString &name, BE::Rule *category)
{
    BE::Rule *rule = new BE::Rule(name);
    rule->setType(BE::Rule::OrdinaryRule);
    rule->setId(m_appFacade->nextRuleId());

    bool appended = m_ruleTreeModel->appendItem(rule, category);

    if (!appended) {
        delete rule;
        return 0;
    }

    return rule;
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

void Lvk::FE::MainWindow::showRule(quint64 ruleId)
{
    const BE::Rule *rule = findRule(ruleId);

    if (rule) {
        m_ruleTreeSelectionModel->clearSelection();
        selectRule(rule);
    } else {
        QString title = tr("Rule not found");
        QString msg = tr("Rule not found. The rule was removed.");
        QMessageBox::information(this, title, msg, QMessageBox::Ok);
    }
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

void Lvk::FE::MainWindow::onHistoryShowRule(quint64 ruleId)
{
    showRule(ruleId);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onTeachFromHistory(const QString &msg)
{
    setUiMode(FE::EditRuleUiMode);

    BE::Rule *category = getCategoryFromDialog();

    if (category) {
        BE::Rule *rule = new BE::Rule("", QStringList() << msg, QStringList());

        bool appended = m_ruleTreeModel->appendItem(rule, category);

        if (appended) {
            selectRule(rule);
            ui->ruleEditWidget->setFocusOnOutput();
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
    QString item = QInputDialog::getItem(this, title, msg, categories, 0, false, &ok);

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
    m_ruleAdded = false;

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
    ui->ruleEditWidget->setRule(rule);
    setUiMode(FE::EditRuleUiMode);
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
    ui->connectionWidget->refresh();
    ui->clueWidget->refresh();
    setUiMode(FE::EditRuleUiMode);
    selectFirstRule();
    updateScore();
}

//--------------------------------------------------------------------------------------------------
// Rule edition & undo
//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onRuleInputEdited(const QString &input)
{
    // Refresh rule tree while user is typing
    if (!m_ruleTreeSelectionModel->selectedIndexes().isEmpty()) {
        QModelIndex selectedIndex = m_ruleTreeSelectionModel->selectedIndexes().first();
        m_ruleTreeModel->setData(selectedIndex, input.simplified(), Qt::EditRole);
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onRuleAdded()
{
    const BE::Rule *selRule = selectedRule();

    if (selRule) {
        m_ruleAdded = true;

//        ui->undoRuleButton->setEnabled(true);
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onRuleEdited()
{
    m_ruleEdited = true;

    ui->categoriesTree->setDragEnabled(false);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onTeachRule()
{
    teachRule(selectedRule());
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onUndoRule()
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

    rule->setName(ui->ruleEditWidget->name());
    rule->setTarget(ui->ruleEditWidget->targets());
    rule->setInput(ui->ruleEditWidget->input());
    rule->setOutput(ui->ruleEditWidget->output());
    rule->setNextCategory(ui->ruleEditWidget->nextCategory());

    ruleEditFinished();

    m_appFacade->refreshNlpEngine();
    m_appFacade->save();

    updateScore();

    ui->ruleEditWidget->setButtonsEnabled(false);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::undoRule(BE::Rule *rule)
{
    if (!rule) {
        return;
    }

    if (m_ruleAdded) {
        QModelIndex index = m_ruleTreeModel->indexFromItem(rule);
        if (index.isValid()) {
            // Removing the row will cause a selection change, invoking onRuleSelectionChanged,
            // setting to false will avoid a onRuleSelectionChanged's dialog being prompted
            // TODO m_ruleEdited and m_ruleAdded need a big refactoring or new design
            m_ruleEdited = false;
            m_ruleTreeModel->removeRow(index.row(), index.parent());
        }
    } else if (m_ruleEdited) {
        m_ruleTreeModel->setData(m_ruleTreeModel->indexFromItem(rule), ui->ruleEditWidget->name(),
                                 Qt::EditRole);
    }

    ruleEditFinished();
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::ruleEditFinished()
{
    m_ruleAdded = false;
    m_ruleEdited = false;
    ui->categoriesTree->setDragEnabled(true);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onRulesRemoved(const QModelIndex &parent, int first, int last)
{
    // This slot is called when the user removes a rule or when there was a drag & drop action
    qDebug() << "MainWindow: Rows removed:" << parent.row() << first << last;
    qDebug() << "MainWindow: Refreshing NLP engine...";

    m_appFacade->refreshNlpEngine();

    updateScore();

}

//--------------------------------------------------------------------------------------------------
// Local test
//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onTestInputTextEntered()
{
    if (m_ruleEdited || m_ruleAdded) {
        handleRuleEdited(selectedRule());
    }

    QString input = ui->testInputText->text();
    QString target = ui->testInputText->currentItem().username;

    if (target.isEmpty()) {
        target = "[ChatbotTest]";
    }

    // FIXME if target is currently talking with the chatbot, we can change the current topic,
    // it's not very likely but possible.
    BE::AppFacade::MatchList matches;
    QString response = m_appFacade->getResponse(input, target, matches);

    qDebug() << "Test input:" << input << " Response:" << response << " Matches:" << matches.size();

    // TODO create a new widget to handle this:
    ui->testConversationText->appendConversation(input, response, !matches.isEmpty());
    ui->testInputText->setText("");
    ui->clearTestConvButton->setEnabled(true);

    setCurrentCategory(m_appFacade->getCurrentCategory(target));
    highlightMatchedRules(matches);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onTestTargetChanged()
{
    BE::RosterItem target = ui->testInputText->currentItem();

    if (target.isNull()) {
        ui->testLabel->setText(tr("Test:"));
    } else {
        ui->testLabel->setText(tr("Test as user ") + target.fullname + ":");
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onClearTestConvPressed()
{
    ui->clearTestConvButton->setEnabled(false);
    ui->testConversationText->clear();
    ui->categoryGroupBox->setVisible(false);
    ui->categoryLabel->clear();
    ui->ruleView->clear();
    ui->ruleViewGroupBox->setVisible(false);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onTestShowRule()
{
    qDebug() << ui->ruleView->ruleId();
    if (ui->ruleView->ruleId()) {
        showRule(ui->ruleView->ruleId());
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::highlightMatchedRules(const BE::AppFacade::MatchList &matches)
{
    const BE::Rule *rule = 0;
    int inputNumber = -1;

    if (!matches.empty()) {
        // Assuming only one match
        quint64 ruleId = matches.first().first;
        inputNumber = matches.first().second;
        rule = findRule(ruleId);
    } else {
        rule = evasivesRule();
    }

    ui->ruleViewGroupBox->setVisible(true);
    ui->ruleView->setRule(rule, inputNumber);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::setCurrentCategory(quint64 catId)
{
    if (nlpEngineOption(BE::AppFacade::PreferCurCategory)) {
        const BE::Rule *category = findRule(catId);
        ui->categoryLabel->setText(category ? category->name() : tr("(none)"));
        ui->categoryGroupBox->setVisible(true);
    } else {
        ui->categoryLabel->clear();
        ui->categoryGroupBox->setVisible(false);
    }
}

//--------------------------------------------------------------------------------------------------
// Verify account
//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onVerifyAccountOk()
{
    qDebug() << "MainWindow: Verify Account Ok";

    ui->ruleEditWidget->setRoster(m_appFacade->roster());
    ui->testInputText->setRoster(m_appFacade->roster());

    if (m_refactor.uiTabsLayout() == FE::VerifyAccountTabsLayout) {
        startEditMode();
    }

    QMessageBox::information(this, tr("Account verified"), tr("Account verified!"));

    updateScore();
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onVerifyAccountError(int err, const QString &msg_)
{
    qDebug() << "MainWindow: Verify Account Error" << err;

    QString title;
    QString msg = msg_;

    if (msg.isEmpty()) {
        if (err != BE::AppFacade::SSLNotSupportedError) {
            title = tr("Account error");
            msg   = tr("The account could not be verified. "
                       "Please check your username and password and internet connection.");
        } else {
            title = tr("SSL error");
            msg   = tr("The account could not be verified. "
                       "Your system does not support secure connections.");
        }
    }

    QMessageBox::critical(this, title, msg);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onVerifyAccountSkipped()
{
    qDebug() << "MainWindow: Verify Account Skipped";

    startEditMode();
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onVerifyAccountCanceled()
{
    qDebug() << "MainWindow: Verify Account Canceled";

    setUiMode(FE::WelcomeTabUiMode);
}

//--------------------------------------------------------------------------------------------------
// Chat connection
//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onConnectionOk()
{
    qDebug() << "MainWindow: Chatbot connection OK";

    BE::Roster roster = m_appFacade->roster();

    ui->ruleEditWidget->setRoster(roster);
    ui->testInputText->setRoster(roster);

    m_refactor.setConnectionTabIcon(true);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onConnectionError(int err)
{
    qDebug() << "MainWindow: Chatbot connection error" << err;

    m_refactor.setConnectionTabIcon(false);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onDisconnection()
{
    qDebug() << "MainWindow: Chatbot disconnection";

    m_refactor.setConnectionTabIcon(false);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onNewChatConversation(const Cmn::Conversation::Entry &entry)
{
    ui->chatHistory->addConversationEntry(entry);

    updateScore();
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onRosterSelChanged()
{
    m_appFacade->setBlackRoster(ui->connectionWidget->uncheckedRoster());
}

//--------------------------------------------------------------------------------------------------
// Misc events
//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onSplitterMoved(int, int)
{
    saveSplittersSettings();
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onCurrentTabChanged(QWidget *tab)
{
    if (m_ruleEdited || m_ruleAdded) {
        // No QTabWidget does not have aboutToChange signal, so we are doing this trick:
        disconnect(ui->mainTabWidget, SIGNAL(currentChanged(QWidget*)), this,
                   SLOT(onCurrentTabChanged(QWidget*)));

        ui->mainTabWidget->setCurrentWidget(ui->teachTab);
        handleRuleEdited(selectedRule());
        ui->mainTabWidget->setCurrentWidget(tab);

        connect(ui->mainTabWidget, SIGNAL(currentChanged(QWidget*)),
                SLOT(onCurrentTabChanged(QWidget*)));
    }

    if (tab == ui->testTab) {
        ui->testInputText->setFocus();
    }
}

//--------------------------------------------------------------------------------------------------
// Score
//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::updateScore()
{
    Stats::Score cur = m_appFacade->currentScore();
    Stats::Score best = m_appFacade->bestScore();

    m_tinyScore->setScore(cur, best);
    updateTinyScorePos();

    ui->curScoreWidget->setScore(cur);
    ui->bestScoreWidget->setScore(best);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::updateTinyScorePos()
{
#ifdef DA_CONTEST
//    if (!m_tinyScore->isVisible()) {
//        m_tinyScore->setVisible(true);
//    }

//    if (m_tinyScore) {
//        m_tinyScore->move(ui->mainTabWidget->width() - m_tinyScore->width() - 5, -2);
//    }
#endif
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onUploadScore()
{
    if (m_appFacade->username().isEmpty()) {
        QString title = tr("Upload score");
        QString message = tr("Before sending your score you need to verify your account.\n"
                             "Please, go to the 'Connection' tab and verify your account");
        QMessageBox::critical(this, title, message);
    } else {
        Stats::Score best = m_appFacade->bestScore();

        ui->bestScoreWidget->setUploadEnabled(false);

        if (FE::SendScoreDialog(best, m_filename, this).exec() == QDialog::Accepted) {
            FE::MemberFunctor<MainWindow> *f = new FE::MemberFunctor<MainWindow>(this,
                                &MainWindow::uploadBlockedForUpdate,
                                &MainWindow::uploadContestData);

            UpdateExecutor::exec(f, isCritical);
        } else {
            ui->bestScoreWidget->setUploadEnabled(true);
        }
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onScoreRemainingTime(int secs)
{
    QTime time = QTime(0,0,0).addSecs(secs);

    m_tinyScore->setRemainingTime(time);

    QString status = m_appFacade->isConnected() ?
                tr("Chatbot connected") : tr("Chatbot disconnected");

    QString text = QString(tr("Remaining time: %1 (%2)")).arg(time.toString("hh:mm:ss"), status);

    ui->remainingTimeLabel->setText(text);

    if (secs == 0) {
        updateScore();
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::uploadContestData()
{
    DAS::ContestData data;
    data.filename = m_appFacade->getTempFileForUpload();
    data.username = m_appFacade->username();
    data.chatbotId = m_appFacade->chatbotId();
    data.bestScore = m_appFacade->bestScore();

    if (!data.filename.isEmpty()) {
        FE::UploaderProgressDialog dialog(data, this);

        int rc = dialog.exec();

        qDebug() << "MainWindow::uploadContestData() finished with code" << rc;
    } else {
        QMessageBox::critical(this, tr("Error"),
                              tr("Could not create file for upload. Please verify that you have "
                                 "enough space in you hard disk."));
    }

    ui->bestScoreWidget->setUploadEnabled(true);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::uploadBlockedForUpdate(const DAS::UpdateInfo &info)
{
    onUpdate(info);

    ui->bestScoreWidget->setUploadEnabled(true);
}

//--------------------------------------------------------------------------------------------------
// Clue
//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onClueShowRule(quint64 ruleId)
{
    showRule(ruleId);
}

//--------------------------------------------------------------------------------------------------
// Updates
//--------------------------------------------------------------------------------------------------

void Lvk::FE::MainWindow::onUpdate(const DAS::UpdateInfo &info)
{
    FE::NewUpdateDialog(info, this).exec();
}


