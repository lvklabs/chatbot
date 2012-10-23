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
     *
     */
    void accountChanged();

    /**
     *
     */
    void changeAccountError(int err, const QString &msg);

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
    void onAccountOk();
    void onChangeCanceled();
    void onChangeAccountPressed();
    void onConnectPressed();
    void onDisconnectPressed();
    void onConnectionOk();
    void onConnectionError(int err);
    void onDisconnection();

private:
    enum UiMode
    {
        DisconnectedUiMode,
        ConnectingUiMode,
        ConnectionFailedUiMode,
        ConnectionSSLFailedUiMode,
        ConnectionOkUiMode,
        ChangeAccountUiMode
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

    void connectSignals();
    void connectChatbot();
    void connectBlockedForUpdate(const DAS::UpdateInfo &info);
    void setUiMode(UiMode mode);
    QPixmap chatIcon();
    QString username();
};

/// @}

} // namespace FE

/// @}

} // namespace Lvk


#endif // LVK_FE_CONNECTIONWIDGET_H
