#ifndef LVK_FE_SENDSCOREDIALOG_H
#define LVK_FE_SENDSCOREDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
    class SendScoreDialog;
}

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace FE
{

/// \ingroup Lvk
/// \addtogroup FE
/// @{

/**
 * \brief The SendScoreDialog class provides a dialog to ask for confirmation before sending
 *        the score and allows the user to see details of what is being sent.
 */
class SendScoreDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * Constructs an SendScoreDialog widget with the given \a details and which is a child
     * of \a parent.
     */
    explicit SendScoreDialog(QString details = "", QWidget *parent = 0);

    /**
     * Destroys the object.
     */
    ~SendScoreDialog();

private:
    Ui::SendScoreDialog *ui;

    void alignCenter(int w, int h);

private slots:
    void onLinkActivated(const QString&);
};

/// @}

} // namespace FE

/// @}

} // namespace Lvk

#endif // LVK_FE_SENDSCOREDIALOG_H
