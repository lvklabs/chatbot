#include "front-end/ruletreeview.h"
#include "front-end/ruletreemodel.h"

#include <QtDebug>
#include <QDropEvent>

//--------------------------------------------------------------------------------------------------
// RuleTreeView
//--------------------------------------------------------------------------------------------------

Lvk::FE::RuleTreeView::RuleTreeView(QWidget *parent)
    : QTreeView(parent)
{
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::RuleTreeView::dropEvent(QDropEvent *event)
{
    QTreeView::dropEvent(event);

    ////////////////////////////////////////////////////////////////////
    // Workaround for Qt bug on the Drag & Drop mechanism:
    // https://bugreports.qt-project.org/browse/QTBUG-6679
    // https://bugreports.qt-project.org/browse/QTBUG-26229
    // The workaround is taken from:
    // http://www.qtcentre.org/threads/49819-Refuse-from-removing-row-in-Model-after-unsuccessful-drop
    if (RuleTreeModel *m = dynamic_cast<RuleTreeModel *>(model())) {
        if (!m->dropAccepted()) {
            event->setDropAction(Qt::IgnoreAction);
        }
    }
    // End workaround
    ////////////////////////////////////////////////////////////////////
}
