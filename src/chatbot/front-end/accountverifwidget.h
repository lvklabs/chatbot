#ifndef LVK_FE_ACCOUNTVERIFWIDGET_H
#define LVK_FE_ACCOUNTVERIFWIDGET_H

#include "back-end/appfacade.h"

#include <QWidget>
#include <QPixmap>
#include <QString>

namespace Ui {
    class AccountVerifWidget;
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
 * The AccountVerifWidget provides a widget to verify chat accounts.
 *
 * The AccountVerifWidget is composed by a username input text, a password input text,
 * and a progress bar. The account verification processes is delegated to the
 * AppFacade object configured with setAppFacade()
 */
class AccountVerifWidget : public QWidget
{
    Q_OBJECT

public:

    /**
     * Constructs a AccountVerifWidget object.
     */
    AccountVerifWidget(QWidget *parent = 0);

    /**
     * Destroys the object
     */
    ~AccountVerifWidget();

    /**
     * Sets \a appFacade as the new application facade for the widget. The widget does not
     * own the given pointers.
     */
    // TODO define a new interface instead of using AppFacade
    void setAppFacade(BE::AppFacade *appFacade);

    /**
     * Clears the widget
     */
    void clear();

    /**
     * If \a allowed is true, the verification process can be skipped. Otherwise cannot.
     * By default is allowed.
     */
    void setSkipAllowed(bool allowed);

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
     * This signal is emited whenever we are trying to performn an operation that is blocked
     * because there is a critical update. The two operations that can be blocked are
     * verifyAccount() and connectToChat()
     */
    void blockedForUpdate(const DAS::UpdateInfo &info);

private slots:
    void onVerifyPressed();
    void onAccountOk();
    void onAccountError(int err, const QString &msg);
    void onVerifyAccountSkipped();
    void onCancelChAccountPressed();

private:

    enum UiMode
    {
        VerifyAccountUiMode,
        VerifyingAccountUiMode,
        VerifyAccountFailedUiMode
    };

    Ui::AccountVerifWidget *ui;
    BE::AppFacade          *m_appFacade;
    UiMode                  m_uiMode;

    void connectSignals();
    void verifyAccount();
    void verifyBlockedForUpdate(const DAS::UpdateInfo &info);
    void setUiMode(UiMode mode);
    inline bool gtalkEnabled() const;
};

/// @}

} // namespace FE

/// @}

} // namespace Lvk


#endif // LVK_FE_ACCOUNTVERIFWIDGET_H
