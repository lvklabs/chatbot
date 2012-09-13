#ifndef LVK_FE_WELCOMEWIDGET_H
#define LVK_FE_WELCOMEWIDGET_H

#include <QWidget>

namespace Ui {
    class WelcomeWidget;
}

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace FE
{

class MainWindow;

/// \ingroup Lvk
/// \addtogroup FE
/// @{

// TODO rename to WelcomeWindow
/**
 * \brief The WelcomeWidget class provides a welcome screen with the options create,
 *        open and open last chatbot and a fancy background. The class bootstraps MainWindow.
 */
class WelcomeWidget : public QWidget
{
    Q_OBJECT

public:

    /**
     * Constructs an WelcomeWidget widget which is a child of \a parent.
     */
    explicit WelcomeWidget(QWidget *parent = 0);

    /**
     * Destroys the object.
     */
    ~WelcomeWidget();

private slots:
    void onCreateChatbot();
    void onOpenChatbot();
    void onOpenLastChatbot();

private:
    Ui::WelcomeWidget *ui;
    MainWindow *m_mw;
};

/// @}

} // namespace FE

/// @}

} // namespace Lvk

#endif // LVK_FE_WELCOMEWIDGET_H
