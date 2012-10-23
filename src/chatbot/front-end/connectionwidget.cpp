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
      m_fbIcon(FB_ICON_FILE), m_gIcon(GMAIL_ICON_FILE), m_verify(false)
{
    ui->setupUi(this);

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
    connect(ui->verifyAccountButton,   SIGNAL(clicked()),       SLOT(onVerifyPressed()));
    connect(ui->connectButton,         SIGNAL(clicked()),       SLOT(onConnectPressed()));
    connect(ui->disconnectButton,      SIGNAL(clicked()),       SLOT(onDisconnectPressed()));
    connect(ui->changeAccountButton,   SIGNAL(clicked()),       SLOT(onChangeAccountPressed()));
    connect(ui->verifyLaterButton,     SIGNAL(clicked()),       SLOT(onVerifyAccountSkipped()));
    connect(ui->cancelChAccountButton, SIGNAL(clicked()),       SLOT(onCancelChAccountPressed()));

    connect(ui->passwordText_v,        SIGNAL(returnPressed()), SLOT(onVerifyPressed()));
    connect(ui->passwordText,          SIGNAL(returnPressed()), SLOT(onConnectPressed()));

    connect(ui->rosterWidget,          SIGNAL(selectionChanged()),
            SIGNAL(rosterSelectionChanged()));
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ConnectionWidget::setAppFacade(BE::AppFacade *appFacade)
{
    m_appFacade = appFacade;

    if (m_appFacade) {
        connect(m_appFacade, SIGNAL(connected()),               SLOT(onConnectionOk()));
        connect(m_appFacade, SIGNAL(disconnected()),            SLOT(onDisconnection()));
        connect(m_appFacade, SIGNAL(connectionError(int)),      SLOT(onConnectionError(int)));
        connect(m_appFacade, SIGNAL(accountOk()),               SLOT(onAccountOk()));
        connect(m_appFacade, SIGNAL(accountError(int, QString)),SLOT(onAccountError(int, QString)));
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ConnectionWidget::onVerifyPressed()
{
    QString title;
    QString errMsg;

    QString username = ui->usernameText_v->text();

    if (username.isEmpty()) {
        title = tr("Invalid username");
        errMsg = tr("Please provide a username");
    }

    // To connect to a facebook chat we need the facebook username. We cannot connect using the
    // user's email. The 'Dale Aceptar' verification mechanism provide us of the facebook
    // username so this check only makes sense for non-'Dale Aceptar' versions because
    #ifndef DA_CONTEST
    if (uiChatSelected() == BE::FbChat && username.contains("@") &&
               !username.contains("@facebook.com")) {
        title = tr("Invalid username");
        errMsg = tr("To connect you need to provide your Facebook username instead of your "
                    "email.<br/><br/>You don't have or remember your username? "
                    "<a href=\"http://www.facebook.com/username\">Click here</a>");
    }
    #endif

    if (errMsg.isEmpty()) {
        setUiMode(VerifyingAccountUiMode);

    #ifdef DA_CONTEST
        FE::MemberFunctor<ConnectionWidget> *f = new FE::MemberFunctor<ConnectionWidget>(this,
                            &ConnectionWidget::verifyBlockedForUpdate,
                            &ConnectionWidget::verifyAccount);

        UpdateExecutor::exec(f, isCritical);
    #else
        verifyAccount();
    #endif
    } else {
        QMessageBox::information(this, title, errMsg);

        ui->usernameText_v->setFocus();
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ConnectionWidget::verifyAccount()
{
    assert(m_appFacade);

    qDebug() << "ConnectionWidget: Verifying Account...";

    QString username = ui->usernameText_v->text();
    QString password = ui->passwordText_v->text();

    m_appFacade->verifyAccount(chatTypeSelected(), username, password);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ConnectionWidget::verifyBlockedForUpdate(const DAS::UpdateInfo &info)
{
    m_uiMode == m_verify ? setUiMode(VerifyAccountUiMode) : setUiMode(ChangeAccountUiMode);

    emit blockedForUpdate(info);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ConnectionWidget::onAccountOk()
{
    qDebug() << "ConnectionWidget: Verify Account Ok";

    setUiMode(DisconnectedUiMode);

    emit accountOk();
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ConnectionWidget::onAccountError(int err, const QString &msg)
{
    qDebug() << "ConnectionWidget: Verify Account Error" << err << msg;

    setUiMode(VerifyAccountFailedUiMode);

    emit accountError(err, msg);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ConnectionWidget::onVerifyAccountSkipped()
{
    qDebug() << "ConnectionWidget: Verify Account Skipped";

    ui->fbChatRadio_v->setChecked(true);
    ui->usernameText_v->setText("");
    ui->passwordText_v->setText("");

    setUiMode(DisconnectedUiMode);

    emit verificationSkipped();
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ConnectionWidget::onChangeAccountPressed()
{
    ui->usernameText_v->clear();
    ui->passwordText_v->clear();

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

void Lvk::FE::ConnectionWidget::onCancelChAccountPressed()
{
    assert(m_appFacade);

    qDebug() << "ConnectionWidget: Verify Account Canceled";

    m_appFacade->cancelVerifyAccount();

    if (m_verify) {
        if (m_uiMode == VerifyingAccountUiMode) {
            setUiMode(VerifyAccountUiMode);
        } else {
            setUiMode(VerifyAccountUiMode);
            emit verificationCanceled();
        }
    } else {
        if (m_uiMode == VerifyingAccountUiMode) {
            setUiMode(ChangeAccountUiMode);
        } else {
            setUiMode(DisconnectedUiMode);
            emit verificationCanceled();
        }
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
        ui->verifyExplanationLabel->setText(QObject::tr("Please insert your username and password"
                                                        " and press \"Verify account\" button."));
        ui->verifyAccountButton->setEnabled(true);
        ui->usernameText_v->setEnabled(true);
        ui->passwordText_v->setEnabled(true);
        ui->fbChatRadio_v->setEnabled(true);
        ui->gtalkChatRadio_v->setEnabled(gtalkEnabled());
        ui->connectionProgressBar_v->setVisible(false);
        ui->connectionStatusLabel_v->setVisible(false);
        ui->verifyLaterButton->setVisible(false);
        ui->passwordText->setText("");
        break;

    case VerifyAccountUiMode:
        ui->connectToChatStackWidget->setCurrentIndex(2);
        ui->verifyExplanationLabel->setText(QObject::tr("To create a chatbot you need a Facebook"
                                                        " or Gmail account.\nPlease insert your"
                                                        " username and password and press "
                                                        "\"Verify account\" button."));
        ui->verifyAccountButton->setEnabled(true);
        ui->usernameText_v->setEnabled(true);
        ui->passwordText_v->setEnabled(true);
        ui->fbChatRadio_v->setEnabled(true);
        ui->gtalkChatRadio_v->setEnabled(gtalkEnabled());
        ui->verifyLaterButton->setEnabled(true);
        ui->connectionProgressBar_v->setVisible(false);
        ui->connectionStatusLabel_v->setVisible(false);
        ui->verifyLaterButton->setVisible(true);
        ui->passwordText->setText("");
        break;

    case VerifyingAccountUiMode:
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
        ui->verifyAccountButton->setEnabled(true);
        ui->usernameText_v->setEnabled(true);
        ui->passwordText_v->setEnabled(true);
        ui->fbChatRadio_v->setEnabled(true);
        ui->gtalkChatRadio_v->setEnabled(gtalkEnabled());
        ui->verifyLaterButton->setEnabled(true);
        ui->connectionProgressBar_v->setVisible(false);
        ui->connectionStatusLabel_v->setVisible(false);
        break;
    }

    m_uiMode = mode;
}


//--------------------------------------------------------------------------------------------------

bool Lvk::FE::ConnectionWidget::gtalkEnabled()
{
#ifdef DA_CONTEST
    return false;
#else
    return true;
#endif
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

Lvk::BE::ChatType Lvk::FE::ConnectionWidget::chatTypeSelected()
{
    return ui->gtalkChatRadio_v->isChecked() ? BE::GTalkChat : BE::FbChat;
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ConnectionWidget::clear()
{
    ui->fbChatRadio_v->setChecked(true);
    ui->rosterWidget->clear();
    ui->passwordText->clear();
    ui->passwordText_v->clear();
    ui->usernameText_v->clear();

    m_connectionStatus = DisconnectedFromChat;
    m_verify = false;

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

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ConnectionWidget::setVerifyMode(bool verify)
{
    if (m_verify != verify) {
        m_verify = verify;

        if (m_verify) {
            setUiMode(VerifyAccountUiMode);
        } else {
            setUiMode(DisconnectedUiMode);
        }
    }
}
