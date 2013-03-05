#ifndef LVK_FE_CLUEWIDGET_H
#define LVK_FE_CLUEWIDGET_H

#include "back-end/appfacade.h"

#include <QWidget>

namespace Ui {
    class ClueWidget;
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
 * The ClueWidget class provides the main widget to display in the "Clue" tab
 */
class ClueWidget : public QWidget
{
    Q_OBJECT

public:

    /**
     * Constructs a ClueWidget object.
     */
    explicit ClueWidget(QWidget *parent = 0);

    /**
     * Sets \a appFacade as the new application facade for the widget. The widget does not
     * own the given pointers.
     */
    // TODO define a new interface instead of using AppFacade
    void setAppFacade(BE::AppFacade *appFacade);

    /**
     * Destroys the object
     */
    ~ClueWidget();

public slots:

    /**
     * Refreshes the widget
     */
    void refresh();


    /**
     * Clears the widget
     */
    void clear();

private:
    Ui::ClueWidget *ui;
    BE::AppFacade  *m_appFacade;

    void loadCharacter();
};

/// @}

} // namespace FE

/// @}

} // namespace Lvk

#endif // LVK_FE_CLUEWIDGET_H
