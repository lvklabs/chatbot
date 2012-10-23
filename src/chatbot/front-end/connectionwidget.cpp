#include "front-end/connectionwidget.h"
#include "front-end/updateexecutor.h"
#include "front-end/memberfunctor.h"
#include "common/globalstrings.h"
#include "ui_connectionwidget.h"

#include <QMessageBox>
#include <QtDebug>
#include <cassert>

//--------------------------------------------------------------------------------------------------
// ConnectionWidget
//--------------------------------------------------------------------------------------------------

Lvk::FE::ConnectionWidget::ConnectionWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::ConnectionWidget), m_appFacade(0),
      m_connectionStatus(DisconnectedFromChat), m_uiMode(DisconnectedUiMode),
      m_fbIcon(FB_ICON_FILE), m_gIcon(GMAIL_ICON_FILE)
{
    ui->setupUi(this);

    ui->verifactionWidget->setSkipAllowed(false);

    clear();

    connectSignals();
}

//--------------------------------------------------------------------------------------------------

Lvk::FE::ConnectionWidget::~ConnectionWidget()
{
    delete ui;
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ConnectionWidget::connectSignals()
{
    connect(ui->connectButton,         SIGNAL(clicked()),       SLOT(onConnectPressed()));
    connect(ui->disconnectButton,      SIGNAL(clicked()),       SLOT(onDisconnectPressed()));
    connect(ui->changeAccountButton,   SIGNAL(clicked()),       SLOT(onChangeAccountPressed()));

    connect(ui->passwordText,          SIGNAL(returnPressed()), SLOT(onConnectPressed()));

    connect(ui->rosterWidget,          SIGNAL(selectionChanged()),
            SIGNAL(rosterSelectionChanged()));

    connect(ui->verifactionWidget,     SIGNAL(accountOk()),     SLOT(onAccountOk()));
    connect(ui->verifactionWidget,     SIGNAL(verificationCanceled()),
            SLOT(onChangeCanceled()));
    connect(ui->verifactionWidget,     SIGNAL(accountError(int,QString)),
            SIGNAL(changeAccountError(int,QString)));
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ConnectionWidget::setAppFacade(BE::AppFacade *appFacade)
{
    m_appFacade = appFacade;

    if (m_appFacade) {
        connect(m_appFacade, SIGNAL(connected()),               SLOT(onConnectionOk()));
        connect(m_appFacade, SIGNAL(disconnected()),            SLOT(onDisconnection()));
        connect(m_appFacade, SIGNAL(connectionError(int)),      SLOT(onConnectionError(int)));
    }

    ui->verifactionWidget->setAppFacade(appFacade);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ConnectionWidget::onAccountOk()
{
    qDebug() << "ConnectionWidget: Verify Account Ok";

    setUiMode(DisconnectedUiMode);

    emit accountChanged();
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ConnectionWidget::onChangeCanceled()
{
    qDebug() << "ConnectionWidget: Verify Account Ok";

    setUiMode(DisconnectedUiMode);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ConnectionWidget::onChangeAccountPressed()
{
    if (!username().isEmpty()) {
        QString title = tr("Change Account");
#ifdef DA_CONTEST
        QString msg   = tr("If you change your account you will lose your chatbot score and some"
                           " rules might not work anymore.\n"
                           "Are you sure you want to change your account?");
#else
        QString msg   = tr("If you change your account some rules might not work anymore.\n"
                           "Are you sure you want to change your account?");
#endif

        QMessageBox::StandardButtons buttons = QMessageBox::Yes | QMessageBox::No;

        if (QMessageBox::question(this, title, msg, buttons) == QMessageBox::Yes) {
            setUiMode(ChangeAccountUiMode);
        }
    } else {
        setUiMode(ChangeAccountUiMode);
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ConnectionWidget::onConnectPressed()
{
    if (m_connectionStatus == DisconnectedFromChat || m_connectionStatus == ConnectionError) {
        qDebug() << "ConnectionWidget: Connecting chatbot...";

        m_connectionStatus =  ConnectingToChat;

        setUiMode(ConnectingUiMode);

    #ifdef DA_CONTEST
        FE::MemberFunctor<ConnectionWidget> *f = new FE::MemberFunctor<ConnectionWidget>(this,
                            &ConnectionWidget::connectBlockedForUpdate,
                            &ConnectionWidget::connectChatbot);

        UpdateExecutor::exec(f, isCritical);
    #else
        connectChatbot();
    #endif

    } else if (m_connectionStatus == ConnectingToChat) {
        qDebug() << "ConnectionWidget: Aborting chatbot connection...";
        onDisconnectPressed();
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ConnectionWidget::connectChatbot()
{
    assert(m_appFacade);

    m_appFacade->connectToChat(ui->passwordText->text());
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ConnectionWidget::connectBlockedForUpdate(const DAS::UpdateInfo &info)
{
    onDisconnection();

    emit blockedForUpdate(info);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ConnectionWidget::onDisconnectPressed()
{
    assert(m_appFacade);

    if (m_connectionStatus == ConnectedToChat || m_connectionStatus == ConnectingToChat) {
        qDebug() << "ConnectionWidget: Disconnecting chatbot...";

        m_appFacade->disconnectFromChat();

        m_connectionStatus = DisconnectedFromChat;

        setUiMode(DisconnectedUiMode);
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ConnectionWidget::onConnectionOk()
{
    assert(m_appFacade);

    qDebug() << "ConnectionWidget: Chatbot connection OK";

    ui->rosterWidget->setRoster(m_appFacade->roster(), m_appFacade->blackRoster());

    m_connectionStatus = ConnectedToChat;

    setUiMode(ConnectionOkUiMode);

    emit connected();
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ConnectionWidget::onConnectionError(int err)
{
    if (m_connectionStatus != DisconnectedFromChat) {
        qDebug() << "ConnectionWidget: Chatbot connection error" << err;

        m_connectionStatus = ConnectionError;

        if (err != BE::AppFacade::SSLNotSupportedError) {
            setUiMode(ConnectionFailedUiMode);
        } else {
            setUiMode(ConnectionSSLFailedUiMode);
        }

        emit connectionError(err);
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ConnectionWidget::onDisconnection()
{
    qDebug() << "ConnectionWidget: Chatbot disconnected";

    m_connectionStatus = DisconnectedFromChat;

    setUiMode(DisconnectedUiMode);

    emit disconnected();
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ConnectionWidget::setUiMode(Lvk::FE::ConnectionWidget::UiMode mode)
{
    switch (mode) {

    case DisconnectedUiMode:
        ui->connectToChatStackWidget->setCurrentIndex(0);
        ui->curUsernameLabel->setText(username().isEmpty() ? QObject::tr("(None)") : username());
        ui->chatTypeIcon->setPixmap(chatIcon());
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

    case ConnectingUiMode:
        ui->connectToChatStackWidget->setCurrentIndex(0);
        ui->passwordText->setEnabled(false);
        ui->changeAccountButton->setEnabled(false);
        ui->connectButton->setText(QObject::tr("Cancel connection"));
        ui->connectionProgressBar->setVisible(true);
        ui->connectionStatusLabel->setText(QObject::tr("Connecting..."));
        ui->connectionStatusLabel->setStyleSheet("");
        break;

    case ConnectionFailedUiMode:
        ui->connectToChatStackWidget->setCurrentIndex(0);
        ui->passwordText->setEnabled(true);
        ui->changeAccountButton->setEnabled(true);
        ui->connectButton->setText(QObject::tr("Connect"));
        ui->connectionProgressBar->setVisible(false);
        ui->connectionStatusLabel->setText(QObject::tr("Connection error. Please verify your"
                                                       " username and password."));
        ui->connectionStatusLabel->setStyleSheet("color:red");
        break;

    case ConnectionSSLFailedUiMode:
        ui->connectToChatStackWidget->setCurrentIndex(0);
        ui->passwordText->setEnabled(true);
        ui->changeAccountButton->setEnabled(true);
        ui->connectButton->setText(QObject::tr("Connect"));
        ui->connectionProgressBar->setVisible(false);
        ui->connectionStatusLabel->setText(QObject::tr("Connection error. You system does not"
                                                       " support secure connections."));
        ui->connectionStatusLabel->setStyleSheet("color:red");
        break;

    case ConnectionOkUiMode:
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
        ui->connectToChatStackWidget->setCurrentIndex(2);
        ui->verifactionWidget->clear();
        break;
    }

    m_uiMode = mode;
}

//--------------------------------------------------------------------------------------------------

QPixmap Lvk::FE::ConnectionWidget::chatIcon()
{
    return m_appFacade && m_appFacade->chatType() == BE::GTalkChat ? m_gIcon : m_fbIcon;
}

//--------------------------------------------------------------------------------------------------

QString Lvk::FE::ConnectionWidget::username()
{
    return m_appFacade ? m_appFacade->username() : "";
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ConnectionWidget::clear()
{
    ui->rosterWidget->clear();
    ui->passwordText->clear();
    ui->verifactionWidget->clear();

    m_connectionStatus = DisconnectedFromChat;

    setUiMode(DisconnectedUiMode);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ConnectionWidget::refresh()
{
    setUiMode(m_uiMode);
}

//--------------------------------------------------------------------------------------------------

const Lvk::BE::Roster & Lvk::FE::ConnectionWidget::roster()
{
    return ui->rosterWidget->roster();
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::Roster  Lvk::FE::ConnectionWidget::uncheckedRoster()
{
    return ui->rosterWidget->uncheckedRoster();
}
