#include "ruletreemodel.h"

//--------------------------------------------------------------------------------------------------
// Constructors & Destructors
//--------------------------------------------------------------------------------------------------

Lvk::RuleTreeModel::RuleTreeModel(const QString &/*data*/, QObject *parent)
    : QAbstractItemModel(parent), m_rootItem(new Lvk::RuleItem())
{
    setupModelData();
}

Lvk::RuleTreeModel::~RuleTreeModel()
{
    delete m_rootItem;
}

//--------------------------------------------------------------------------------------------------
// QAbstractItemModel - read only models
//--------------------------------------------------------------------------------------------------

QModelIndex Lvk::RuleTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    Lvk::RuleItem *parentItem = parent.isValid() ?
                static_cast<Lvk::RuleItem *>(parent.internalPointer()) : m_rootItem;

    return index(row, column, parentItem);
}


QModelIndex Lvk::RuleTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return QModelIndex();
    }

    Lvk::RuleItem *childItem = static_cast<Lvk::RuleItem*>(index.internalPointer());
    Lvk::RuleItem *parentItem = childItem->parent();

    if (!parentItem) {
        return QModelIndex();
    }

    return createIndex(parentItem->row(), 0, parentItem);
}

int Lvk::RuleTreeModel::rowCount(const QModelIndex &parent) const
{
    Lvk::RuleItem *parentItem = parent.isValid() ?
                static_cast<Lvk::RuleItem *>(parent.internalPointer()) : m_rootItem;

    return parentItem->childCount();
}

int Lvk::RuleTreeModel::columnCount(const QModelIndex &parent) const
{
    Lvk::RuleItem *parentItem = parent.isValid() ?
                static_cast<Lvk::RuleItem *>(parent.internalPointer()) : m_rootItem;

    return parentItem->columnCount();
}

QVariant Lvk::RuleTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    Lvk::RuleItem *item = static_cast<Lvk::RuleItem*>(index.internalPointer());

    return item->data(index.column(), role);
}

QVariant Lvk::RuleTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return m_rootItem->data(section);
    }

    return QVariant();
}

//--------------------------------------------------------------------------------------------------
// QAbstractItemModel - editable models
//--------------------------------------------------------------------------------------------------

bool Lvk::RuleTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Lvk::RuleItem *item = itemFromIndex(index);

    if (item) {
        if (item->setData(value, 0, role)) {
            emit dataChanged(index, index);

            return true;
        }
    }
    return false;
}

bool Lvk::RuleTreeModel::setHeaderData(int section, Qt::Orientation orientation,
                                       const QVariant &value, int role)
{
    if (orientation == Qt::Horizontal) {
        if (m_rootItem->setData(value, 0, role)) {
            emit headerDataChanged(orientation, section, section);

            return true;
        }
    }
    return false;
}

bool Lvk::RuleTreeModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    Lvk::RuleItem *parentItem = itemFromIndex(parent);

    bool inserted = false;

    if (parentItem) {
        beginInsertRows(parent, position, position + rows - 1);
        inserted = parentItem->insertChildren(position, rows);
        endInsertRows();
    }

    return inserted;
}

bool Lvk::RuleTreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    Lvk::RuleItem *parentItem = itemFromIndex(parent);

    bool removed = false;

    if (parentItem) {
        beginRemoveRows(parent, position, position + rows - 1);
        removed = parentItem->removeChildren(position, rows);
        endRemoveRows();
    }

    return removed;
}

Qt::ItemFlags Lvk::RuleTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return 0;
    }

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable /*| Qt::ItemIsEditable*/;
}

//--------------------------------------------------------------------------------------------------
// RuleTreeMode
//--------------------------------------------------------------------------------------------------

Lvk::RuleItem * Lvk::RuleTreeModel::invisibleRootItem()
{
    return m_rootItem;
}

Lvk::RuleItem * Lvk::RuleTreeModel::itemFromIndex(const QModelIndex &index)
{
    if (!index.isValid()) {
        return 0;
    }

    return static_cast<Lvk::RuleItem *>(index.internalPointer());
}

QModelIndex Lvk::RuleTreeModel::index(int row, int column, Lvk::RuleItem *parentItem) const
{
    Lvk::RuleItem *childItem = parentItem->child(row);

    return childItem ? createIndex(row, column, childItem) : QModelIndex();
}

QModelIndex Lvk::RuleTreeModel::indexFromItem(Lvk::RuleItem *item)
{
    return item != m_rootItem ? createIndex(item->row(), 0, item) : QModelIndex();
}

bool Lvk::RuleTreeModel::appendItem(Lvk::RuleItem *item)
{
    Lvk::RuleItem *parentItem = item->parent();

    bool appended = false;

    if (parentItem) {
        int row = parentItem->childCount();
        beginInsertRows(indexFromItem(parentItem), row, row);
        appended = parentItem->appendChild(item);
        endInsertRows();
    }

    return appended;
}

void Lvk::RuleTreeModel::setupModelData()
{
    ////////////////////////////////////////////////////////////////////////
    // TODO read from file:

    Lvk::RuleItem *catGreetings    = new Lvk::RuleItem("Saludos");

    catGreetings->setType(Lvk::RuleItem::CategoryRule);

    m_rootItem->appendChild(catGreetings);

    QList<QString> rule1InputList;
    QList<QString> rule1OutputList;
    rule1InputList << QString("Hola") << QString("Hola *");
    rule1OutputList << QString("Hola $USERNAME");

    QList<QString> rule2InputList;
    QList<QString> rule2OutputList;
    rule2InputList << QString("Buenas") << QString("Buena dia") << QString("Buena dia");
    rule2OutputList << QString("Buen dia $USERNAME");

    Lvk::RuleItem * rule1 = new Lvk::RuleItem("", rule1InputList, rule1OutputList);
    Lvk::RuleItem * rule2 = new Lvk::RuleItem("", rule2InputList, rule2OutputList);

    catGreetings->appendChild(rule1);
    catGreetings->appendChild(rule2);

    ////////////////////////////////////////////////////////////////////////
}

