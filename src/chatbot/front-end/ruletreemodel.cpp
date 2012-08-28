/*
 * Copyright (C) 2012 Andres Pagliano, Gabriel Miretti, Gonzalo Buteler,
 * Nestor Bustamante, Pablo Perez de Angelis
 *
 * This file is part of LVK Chatbot.
 *
 * LVK Chatbot is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * LVK Chatbot is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LVK Chatbot.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "front-end/ruletreemodel.h"
#include "back-end/rule.h"

#include <QIcon>
#include <QMimeData>
#include <QDataStream>
#include <QtDebug>


//--------------------------------------------------------------------------------------------------
// Constructors & Destructors
//--------------------------------------------------------------------------------------------------

Lvk::FE::RuleTreeModel::RuleTreeModel(BE::Rule *rootRule, QObject *parent)
    : QAbstractItemModel(parent), m_rootRule(rootRule), m_isUserCheckable(false)
{
}

//--------------------------------------------------------------------------------------------------

Lvk::FE::RuleTreeModel::~RuleTreeModel()
{
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
                static_cast<BE::Rule *>(parent.internalPointer()) : m_rootRule;

    return index(row, column, parentItem);
}

//--------------------------------------------------------------------------------------------------

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

//--------------------------------------------------------------------------------------------------

int Lvk::FE::RuleTreeModel::rowCount(const QModelIndex &parent) const
{
    BE::Rule *parentItem = parent.isValid() ?
                static_cast<BE::Rule *>(parent.internalPointer()) : m_rootRule;

    return parentItem->childCount();
}

//--------------------------------------------------------------------------------------------------

int Lvk::FE::RuleTreeModel::columnCount(const QModelIndex &parent) const
{
    BE::Rule *parentItem = parent.isValid() ?
                static_cast<BE::Rule *>(parent.internalPointer()) : m_rootRule;

    return columnCountForItem(parentItem);
}

//--------------------------------------------------------------------------------------------------

QVariant Lvk::FE::RuleTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    BE::Rule *item = static_cast<BE::Rule*>(index.internalPointer());

    return dataForItem(item, index.column(), role);
}

//--------------------------------------------------------------------------------------------------

QVariant Lvk::FE::RuleTreeModel::headerData(int /*section*/, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return m_rootRule->name();
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

//--------------------------------------------------------------------------------------------------

bool Lvk::FE::RuleTreeModel::setHeaderData(int section, Qt::Orientation orientation,
                                       const QVariant &value, int role)
{
    if (orientation == Qt::Horizontal) {
        if (setDataForItem(m_rootRule, value, 0, role)) {
            emit headerDataChanged(orientation, section, section);

            return true;
        }
    }
    return false;
}

//--------------------------------------------------------------------------------------------------

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

//--------------------------------------------------------------------------------------------------

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

//--------------------------------------------------------------------------------------------------

Qt::ItemFlags Lvk::FE::RuleTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return 0;
    }

#ifndef DRAG_AND_DROP_DISABLED
    const BE::Rule *item = itemFromIndex(index);
#endif

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable /*| Qt::ItemIsEditable*/
#ifndef DRAG_AND_DROP_DISABLED
            | (item->type() == BE::Rule::OrdinaryRule ? Qt::ItemIsDragEnabled : Qt::NoItemFlags)
            | (item->type() == BE::Rule::ContainerRule ? Qt::ItemIsDropEnabled : Qt::NoItemFlags)
#endif
            | (m_isUserCheckable ? Qt::ItemIsUserCheckable | Qt::ItemIsTristate : Qt::NoItemFlags);
}

//--------------------------------------------------------------------------------------------------

Qt::DropActions Lvk::FE::RuleTreeModel::supportedDropActions() const
{
#ifndef DRAG_AND_DROP_DISABLED
    return /*Qt::CopyAction |*/ Qt::MoveAction;
#else
    return Qt::IgnoreAction;
#endif
}

//--------------------------------------------------------------------------------------------------
// RuleTreeMode
//--------------------------------------------------------------------------------------------------

Lvk::BE::Rule * Lvk::FE::RuleTreeModel::rootItem()
{
    return m_rootRule;
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::RuleTreeModel::setRootItem(BE::Rule *root)
{
    emit layoutAboutToBeChanged();
    m_rootRule = root;
    emit layoutChanged();
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::Rule * Lvk::FE::RuleTreeModel::itemFromIndex(const QModelIndex &index)
{
    if (!index.isValid()) {
        return m_rootRule;
    }

    return static_cast<BE::Rule *>(index.internalPointer());
}

//--------------------------------------------------------------------------------------------------

const Lvk::BE::Rule * Lvk::FE::RuleTreeModel::itemFromIndex(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return m_rootRule;
    }

    return static_cast<const BE::Rule *>(index.internalPointer());
}

//--------------------------------------------------------------------------------------------------

QModelIndex Lvk::FE::RuleTreeModel::index(int row, int column, BE::Rule *parentItem) const
{
    BE::Rule *childItem = parentItem->child(row);

    return childItem ? createIndex(row, column, childItem) : QModelIndex();
}

//--------------------------------------------------------------------------------------------------

QModelIndex Lvk::FE::RuleTreeModel::indexFromItem(const BE::Rule *item)
{
    return item != m_rootRule ? createIndex(rowForItem(item), 0, (void *)item) : QModelIndex();
}

//--------------------------------------------------------------------------------------------------

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

void Lvk::FE::RuleTreeModel::removeAllRows(const QModelIndex &parent)
{
    removeRows(0, rowCount(parent), parent);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::RuleTreeModel::removeAllRows(BE::Rule *parent)
{
    removeAllRows(indexFromItem(parent));
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::RuleTreeModel::notifyDataAboutToChange()
{
    emit layoutAboutToBeChanged();
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::RuleTreeModel::notifyDataChanged()
{
    emit layoutChanged();
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::RuleTreeModel::clear()
{
    removeAllRows(m_rootRule);
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

//--------------------------------------------------------------------------------------------------

int Lvk::FE::RuleTreeModel::columnCountForItem(const BE::Rule */*item*/) const
{
    return 1;
}

//--------------------------------------------------------------------------------------------------

QVariant Lvk::FE::RuleTreeModel::dataForItem(const BE::Rule *item, int column, int role) const
{
    if (column != 0) {
        return false;
    }

    switch (role) {
    case Qt::EditRole:
    case Qt::DisplayRole:
        switch (item->type()) {
        case BE::Rule::OrdinaryRule:
            return item->input().isEmpty() ? QString("") : item->input().first();
        case BE::Rule::ContainerRule:
        case BE::Rule::EvasiveRule:
            return item->name();
        default:
            return QVariant();
        }

    case Qt::DecorationRole:
        switch (item->type()) {
        case BE::Rule::OrdinaryRule:
            return QIcon(":/icons/rule.png");
        case BE::Rule::ContainerRule:
            return QIcon(":/icons/category.png");
        case BE::Rule::EvasiveRule:
            return QIcon(":/icons/category_locked.png");
        default:
            return QVariant();
        }

    case Qt::CheckStateRole:
        return m_isUserCheckable ? item->checkState() : QVariant();

    default:
        return QVariant();
    }
}

//--------------------------------------------------------------------------------------------------

bool Lvk::FE::RuleTreeModel::setDataForItem(BE::Rule *item, const QVariant &value, int column,
                                            int role)
{
    if (column != 0) {
        return false;
    }

    switch (role) {
    case Qt::EditRole:
    case Qt::DisplayRole:

        switch (item->type()) {

        case BE::Rule::OrdinaryRule:
            if (item->input().size() == 0) {
                item->input().append(value.toString());
            } else {
                item->input().first() = value.toString();
            }
            return true;

        case BE::Rule::ContainerRule:
            item->setName(value.toString());
            return true;

        case BE::Rule::EvasiveRule:
            item->setName(value.toString());
            return true;

        default:
            return false;
        }

    case Qt::CheckStateRole:
        updateCheckStateOnTree(item, static_cast<Qt::CheckState>(value.toInt()));
        return true;

    default:
        return false;
    }

}

//--------------------------------------------------------------------------------------------------

bool Lvk::FE::RuleTreeModel::isUserCheckable()
{
    return m_isUserCheckable;
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::RuleTreeModel::setIsUserCheckable(bool checkable)
{
    m_isUserCheckable = checkable;
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::RuleTreeModel::updateCheckStateOnTree(BE::Rule *item, Qt::CheckState state)
{
    setCheckState(item, state);
    updateCheckStateOnChildren(item, state);
    updateCheckStateOnParent(item, state);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::RuleTreeModel::updateCheckStateOnChildren(BE::Rule *item, Qt::CheckState state)
{
    foreach (BE::Rule *child, item->children()) {
        setCheckState(child, state);
        updateCheckStateOnChildren(child, state);
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::RuleTreeModel::updateCheckStateOnParent(BE::Rule *item, Qt::CheckState state)
{
    BE::Rule *parent = item->parent();

    if (parent) {
        setCheckState(parent, state);

        if (state != Qt::PartiallyChecked) {
            foreach (BE::Rule *sibling, parent->children()) {
                if (sibling->checkState() != state) {
                    setCheckState(parent, Qt::PartiallyChecked);
                    break;
                }
            }
        }

        updateCheckStateOnParent(parent, parent->checkState());
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::RuleTreeModel::setCheckState(BE::Rule *item, Qt::CheckState state)
{
    if (item->checkState() != state) {
        item->setCheckState(state);

        QModelIndex index = indexFromItem(item);
        emit dataChanged(index, index);
    }
}

//--------------------------------------------------------------------------------------------------

bool Lvk::FE::RuleTreeModel::dropMimeData(const QMimeData */*data*/, Qt::DropAction /*action*/,
                                          int /*row*/, int /*column*/, const QModelIndex&/*parent*/)
{
    return false;
}

