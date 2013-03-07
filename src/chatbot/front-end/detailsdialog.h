#ifndef LVK_FE_DETAILSDIALOG_H
#define LVK_FE_DETAILSDIALOG_H

#include <QDialog>
#include <QString>
#include <QStyle>

namespace Ui {
    class DetailsDialog;
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
 * \brief The DetailsDialog class provides a dialog that allows the user to see more details
 *        besides the dialog message by clicking on a link
 */
class DetailsDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * Constructs an DetailsDialog widget with the message \a msg and \a details
     * and which is a child of \a parent. \a linkMsg is the message displayed on the link that
     * performs the details expansion.
     */
    explicit DetailsDialog(const QString &msg, const QString &linkMsg, const QString &details,
                           QWidget *parent = 0);

    /**
     * Shows or hides the cancel button. By default is visible.
     */
    void setCancelButtonVisible(bool visible);

    /**
     * Sets the dialog \a pixmap. By default is SP_MessageBoxQuestion.
     */
    void setPixmap(QStyle::StandardPixmap pixmap, int w = 48, int h = 48);

    /**
     * Destroys the object.
     */
    ~DetailsDialog();

private slots:
    void onLinkActivated(const QString&);

private:
    Ui::DetailsDialog *ui;
    QString m_msg;

    void alignCenter(int w, int h);
};

/// @}

} // namespace FE

/// @}

} // namespace Lvk

#endif // LVK_FE_DETAILSDIALOG_H
