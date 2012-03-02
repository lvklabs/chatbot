#include "ruletreemodel.h"
#include "ruleitem.h"

#include <QIcon>


//--------------------------------------------------------------------------------------------------
// Constructors & Destructors
//--------------------------------------------------------------------------------------------------

Lvk::FE::RuleTreeModel::RuleTreeModel(const QString &/*data*/, QObject *parent)
    : QAbstractItemModel(parent), m_rootItem(new BE::Rule())
{
    setupModelData();
}

Lvk::FE::RuleTreeModel::~RuleTreeModel()
{
    delete m_rootItem;
}

//--------------------------------------------------------------------------------------------------
// QAbstractItemModel - read only models
//--------------------------------------------------------------------------------------------------

QModelIndex Lvk::FE::RuleTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    BE::Rule *parentItem = parent.isValid() ?
                static_cast<BE::Rule *>(parent.internalPointer()) : m_rootItem;

    return index(row, column, parentItem);
}


QModelIndex Lvk::FE::RuleTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return QModelIndex();
    }

    BE::Rule *childItem = static_cast<BE::Rule*>(index.internalPointer());
    BE::Rule *parentItem = childItem->parent();

    if (!parentItem) {
        return QModelIndex();
    }

    return createIndex(rowForItem(parentItem), 0, parentItem);
}

int Lvk::FE::RuleTreeModel::rowCount(const QModelIndex &parent) const
{
    BE::Rule *parentItem = parent.isValid() ?
                static_cast<BE::Rule *>(parent.internalPointer()) : m_rootItem;

    return parentItem->childCount();
}

int Lvk::FE::RuleTreeModel::columnCount(const QModelIndex &parent) const
{
    BE::Rule *parentItem = parent.isValid() ?
                static_cast<BE::Rule *>(parent.internalPointer()) : m_rootItem;

    return columnCountForItem(parentItem);
}

QVariant Lvk::FE::RuleTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    BE::Rule *item = static_cast<BE::Rule*>(index.internalPointer());

    return dataForItem(item, index.column(), role);
}

QVariant Lvk::FE::RuleTreeModel::headerData(int /*section*/, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return m_rootItem->name();
    }

    return QVariant();
}

//--------------------------------------------------------------------------------------------------
// QAbstractItemModel - editable models
//--------------------------------------------------------------------------------------------------

bool Lvk::FE::RuleTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    BE::Rule *item = itemFromIndex(index);

    if (item) {
        if (setDataForItem(item, value, 0, role)) {
            emit dataChanged(index, index);

            return true;
        }
    }
    return false;
}

bool Lvk::FE::RuleTreeModel::setHeaderData(int section, Qt::Orientation orientation,
                                       const QVariant &value, int role)
{
    if (orientation == Qt::Horizontal) {
        if (setDataForItem(m_rootItem, value, 0, role)) {
            emit headerDataChanged(orientation, section, section);

            return true;
        }
    }
    return false;
}

bool Lvk::FE::RuleTreeModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    BE::Rule *parentItem = itemFromIndex(parent);

    bool inserted = false;

    if (parentItem) {
        beginInsertRows(parent, position, position + rows - 1);
        inserted = parentItem->insertChildren(position, rows);
        endInsertRows();
    }

    return inserted;
}

bool Lvk::FE::RuleTreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    BE::Rule *parentItem = itemFromIndex(parent);

    bool removed = false;

    if (parentItem) {
        beginRemoveRows(parent, position, position + rows - 1);
        removed = parentItem->removeChildren(position, rows);
        endRemoveRows();
    }

    return removed;
}

Qt::ItemFlags Lvk::FE::RuleTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return 0;
    }

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable /*| Qt::ItemIsEditable*/;
}

//--------------------------------------------------------------------------------------------------
// RuleTreeMode
//--------------------------------------------------------------------------------------------------

Lvk::BE::Rule * Lvk::FE::RuleTreeModel::invisibleRootItem()
{
    return m_rootItem;
}

Lvk::BE::Rule * Lvk::FE::RuleTreeModel::itemFromIndex(const QModelIndex &index)
{
    if (!index.isValid()) {
        return 0;
    }

    return static_cast<BE::Rule *>(index.internalPointer());
}

QModelIndex Lvk::FE::RuleTreeModel::index(int row, int column, BE::Rule *parentItem) const
{
    BE::Rule *childItem = parentItem->child(row);

    return childItem ? createIndex(row, column, childItem) : QModelIndex();
}

QModelIndex Lvk::FE::RuleTreeModel::indexFromItem(BE::Rule *item)
{
    return item != m_rootItem ? createIndex(rowForItem(item), 0, item) : QModelIndex();
}

bool Lvk::FE::RuleTreeModel::appendItem(BE::Rule *item)
{
    BE::Rule *parentItem = item->parent();

    bool appended = false;

    if (parentItem) {
        int row = parentItem->childCount();
        beginInsertRows(indexFromItem(parentItem), row, row);
        appended = parentItem->appendChild(item);
        endInsertRows();
    }

    return appended;
}

//--------------------------------------------------------------------------------------------------
// helpers
//--------------------------------------------------------------------------------------------------

int Lvk::FE::RuleTreeModel::rowForItem(const BE::Rule *item) const
{
    if (item->parent()) {
        return item->parent()->children().indexOf((BE::Rule *)item);
    }

    return 0;
}

int Lvk::FE::RuleTreeModel::columnCountForItem(const BE::Rule */*item*/) const
{
    return 1;
}

QVariant Lvk::FE::RuleTreeModel::dataForItem(const BE::Rule *item, int column, int role) const
{
    switch (column) {
    case 0:
        switch (role) {
        case Qt::EditRole:
        case Qt::DisplayRole:
            if (item->type() == BE::Rule::FinalRule) {
                return item->input().size() == 0 ? QString("") : item->input()[0];
            } else {
                return item->name();
            }

        case Qt::DecorationRole:
            return item->type() == BE::Rule::FinalRule ? QIcon(":/icons/rule_16x16.png")
                                                      : QIcon(":/icons/category_16x16.png");
        default:
            return QVariant();
        }

    default:
        return QVariant();
    }
}

bool Lvk::FE::RuleTreeModel::setDataForItem(BE::Rule *item, const QVariant &value, int column,
                                            int role)
{
    switch (column) {
    case 0:
        switch (role) {
        case Qt::EditRole:
        case Qt::DisplayRole:
            if (item->type() == BE::Rule::FinalRule) {
                if (item->input().size() == 0) {
                    item->input().append(value.toString());
                } else {
                    item->input()[0] = value.toString();
                }
            } else {
                item->setName(value.toString());
            }
            return true;

        default:
            return false;
        }

    default:
        return false;
    }
}

//--------------------------------------------------------------------------------------------------
// load data
//--------------------------------------------------------------------------------------------------

void Lvk::FE::RuleTreeModel::setupModelData()
{
    ////////////////////////////////////////////////////////////////////////
    // TODO read from file:

    BE::Rule *catGreetings    = new BE::Rule("Saludos");

    catGreetings->setType(BE::Rule::ContainerRule);

    m_rootItem->appendChild(catGreetings);

    QList<QString> rule1InputList;
    QList<QString> rule1OutputList;
    rule1InputList << QString("Hola") << QString("Hola *");
    rule1OutputList << QString("Hola $USERNAME");

    QList<QString> rule2InputList;
    QList<QString> rule2OutputList;
    rule2InputList << QString("Buenas") << QString("Buena dia") << QString("Buena dia");
    rule2OutputList << QString("Buen dia $USERNAME");

    BE::Rule * rule1 = new BE::Rule("", rule1InputList, rule1OutputList);
    BE::Rule * rule2 = new BE::Rule("", rule2InputList, rule2OutputList);

    catGreetings->appendChild(rule1);
    catGreetings->appendChild(rule2);

    ////////////////////////////////////////////////////////////////////////
}
