#ifndef LVK_FE_TINYSCOREWIDGET_H
#define LVK_FE_TINYSCOREWIDGET_H

#include <QWidget>
#include <QTime>

#include "back-end/score.h"

namespace Ui {
    class TinyScoreWidget;
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
 * \brief The TinyScoreWidget class provides a tiny widget to display the score of the chatbot
 *        all the time.
 */
class TinyScoreWidget : public QWidget
{
    Q_OBJECT

public:

    /**
     * Constructs an TinyScoreWidget widget which is a child of \a parent.
     */
    explicit TinyScoreWidget(QWidget *parent = 0);

    /**
     * Destroys the object.
     */
    ~TinyScoreWidget();

public slots:

    /**
     * Clears the widget content.
     */
    void clear();

    /**
     * Sets the current and best score.
     */
    void setScore(const BE::Score &current, const BE::Score &best);

    /**
     * Sets \a time as the remaining time.
     */
    void setRemainingTime(const QTime &time);

protected:
    bool event(QEvent *event);

private:
    Ui::TinyScoreWidget *ui;
};

/// @}

} // namespace FE

/// @}

} // namespace Lvk

#endif // LVK_FE_TINYSCOREWIDGET_H
