#ifndef LVK_FE_CLUEWIDGET_H
#define LVK_FE_CLUEWIDGET_H

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
     * Destroys the object
     */
    ~ClueWidget();

private:
    Ui::ClueWidget *ui;
};

/// @}

} // namespace FE

/// @}

} // namespace Lvk

#endif // LVK_FE_CLUEWIDGET_H
