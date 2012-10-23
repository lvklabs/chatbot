#ifndef LVK_FE_CONNECTIONWIDGET_H
#define LVK_FE_CONNECTIONWIDGET_H

#include "back-end/appfacade.h"

#include <QWidget>
#include <QPixmap>
#include <QString>

namespace Ui {
    class ConnectionWidget;
}

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace DAS
{
    class UpdateInfo;
}

namespace FE
{

/// \ingroup Lvk
/// \addtogroup FE
/// @{

/**
 * The ConnectionWidget provides a widget to verify accounts and connect chatbots.
 *
 * The ConnectionWidget is composed by a username input text, a password input text, a progress bar
 * and a connection/disconnection button. The account verification and connection processes are
 * delegated to the given AppFacade object with setAppFacade()
 */
class ConnectionWidget : public QWidget
{
    Q_OBJECT

public:

    /**
     * Constructs a ConnectionWidget object.
     */
    ConnectionWidget(QWidget *parent = 0);

    /**
     * Destroys the object
     */
    ~ConnectionWidget();

    /**
     * Sets \a appFacade as the new application facade for the widget. The widget does not
     * own the given pointers.
     */
    // TODO define a new interface instead of using AppFacade
    void setAppFacade(BE::AppFacade *appFacade);

    /**
     *
     */
    void setVerifyMode(bool verify);

    /**
     *
     */
    const Lvk::BE::Roster & roster();

    /**
     *
     */
    Lvk::BE::Roster uncheckedRoster();


    /**
     * Refresh the widgets.
     */
    void refresh();

    /**
     * Clears the widget
     */
    void clear();

signals:

    /**
     * \copydoc BE::AppFacade::accountOk
     */
    void accountOk();

    /**
     * This signal es emited whenever the user skips the account verification process
     */
    void verificationSkipped();

    /**
     * This signal es emited whenever the user cancels the account verification process
     */
    void verificationCanceled();

    /**
     * \copydoc BE::AppFacade::accountError
     */
    void accountError(int err, const QString &msg);

    /**
     * \copydoc BE::AppFacade::connected
     */
    void connected();

    /**
     * \copydoc BE::AppFacade::disconnected
     */
    void disconnected();

    /**
     * \copydoc BE::AppFacade::connectionError
     */
    void connectionError(int err);

    /**
     * This signal is emited whenever we are trying to performn an operation that is blocked
     * because there is a critical update. The two operations that can be blocked are
     * verifyAccount() and connectToChat()
     */
    void blockedForUpdate(const DAS::UpdateInfo &info);

    /**
     * This signal is emited whenever the selection of contacts in the roster list has changed.
     */
    void rosterSelectionChanged();

private slots:
    void onVerifyPressed();
    void onAccountOk();
    void onAccountError(int err, const QString &msg);
    void onVerifyAccountSkipped();
    void onChangeAccountPressed();
    void onCancelChAccountPressed();

    void onConnectPressed();
    void onDisconnectPressed();
    void onConnectionOk();
    void onConnectionError(int err);
    void onDisconnection();

private:
    enum UiMode
    {
        VerifyAccountUiMode,
        VerifyingAccountUiMode,
        VerifyAccountFailedUiMode,
        ChangeAccountUiMode,
        DisconnectedUiMode,
        ConnectingUiMode,
        ConnectionFailedUiMode,
        ConnectionSSLFailedUiMode,
        ConnectionOkUiMode
    };

    enum ConnectionStatus {
        DisconnectedFromChat,
        ConnectingToChat,
        ConnectedToChat,
        ConnectionError
    };


    Ui::ConnectionWidget *ui;
    BE::AppFacade        *m_appFacade;
    ConnectionStatus      m_connectionStatus;
    UiMode                m_uiMode;
    QPixmap               m_fbIcon;
    QPixmap               m_gIcon;
    bool                  m_verify;


    void connectSignals();
    void verifyAccount();
    void verifyBlockedForUpdate(const DAS::UpdateInfo &info);
    void connectChatbot();
    void connectBlockedForUpdate(const DAS::UpdateInfo &info);
    void setUiMode(UiMode mode);
    BE::ChatType chatTypeSelected();
    bool gtalkEnabled();
    QPixmap chatIcon();
    QString username();
};

/// @}

} // namespace FE

/// @}

} // namespace Lvk


#endif // LVK_FE_CONNECTIONWIDGET_H
