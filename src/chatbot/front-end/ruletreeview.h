#ifndef LVK_FE_RULETREEVIEW_H
#define LVK_FE_RULETREEVIEW_H

#include <QTreeView>

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
 * \brief The RuleTreeView class provides a tree view for rules based
 */
class RuleTreeView : public QTreeView
{
    Q_OBJECT

public:

    /**
     * Constructs an RuleTreeView widget which is a child of \a parent.
     */
    explicit RuleTreeView(QWidget *parent = 0);
    
protected:
    void dropEvent(QDropEvent *event);
};

/// @}

} // namespace FE

/// @}

} // namespace Lvk

#endif // LVK_FE_RULETREEVIEW_H
