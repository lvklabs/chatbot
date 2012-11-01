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

#ifndef LVK_FE_RULETREEMODEL_H
#define LVK_FE_RULETREEMODEL_H

#include <QAbstractItemModel>

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace BE
{
    class Rule;
}

namespace FE
{

/// \ingroup Lvk
/// \addtogroup FE
/// @{

/**
 * \brief The RuleTreeModel class provides a model for storing a tree of rules.
 *
 * RuleTreeModel implements the QAbstractItemModel interface, which means that the model can be
 * used to provide data in any view that supports that interface (such as QListView, QTableView
 * and QTreeView).
 *
 * This class is most commonly used with QTreeView to display a tree of rules.
 *
 * \see BE::Rule
 */

class RuleTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:

    /**
     * Constructs an RuleTreeModel widget which is a child of \a parent and sets as root rule
     * \a rootRule.
     */
    RuleTreeModel(BE::Rule *rootRule, QObject *parent = 0);

    /**
     * Destroys the object.
     */
    ~RuleTreeModel();

    // QAbstractItemModel interface

    /**
     * Returns the data stored under the given \a role for the item referred to by the \a index.
     */
    virtual QVariant data(const QModelIndex &index, int role) const;

    /**
     * Returns the data for the given \a role and \a section in the header with the specified
     * \a orientation.
     */
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    /**
     * Returns the index of the item in the model specified by the given \a row, \a column and
     * \a parent index.
     */
    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;

    /**
     * Returns the parent of the model item with the given \a index. If the item has no parent, an
     * invalid QModelIndex is returned.
     */
    virtual QModelIndex parent(const QModelIndex &index) const;

    /**
     * Returns the number of rows under the given \a parent. When the parent is valid it means that
     * rowCount is returning the number of children of parent.
     */
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;

    /**
     * Returns the number of columns for the children of the given \a parent.
     */
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

    /**
     * Returns the item flags for the given \a index.
     */
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    /**
     * Returns the drop actions supported by this model.
     */
    virtual Qt::DropActions supportedDropActions() const;

    /**
     * Sets the \a role data for the item at \a index to \a value.
     * Returns true if successful; otherwise returns false.
     */
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    /**
     * Sets the data for the given \a role and \a section in the header with the specified
     * \a orientation to the \a value supplied.
     * Returns true if the header's data was updated; otherwise returns false.
     */
    virtual bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value,
                               int role = Qt::EditRole);

    /**
     * Inserts \a count rows into the model before the given \a row. Items in the new row will be
     * children of the item represented by the parent model index.
     * If row is 0, the rows are prepended to any existing rows in the parent.
     * If row is rowCount(), the rows are appended to any existing rows in the parent.
     * If parent has no children, a single column with count rows is inserted.
     * Returns true if the rows were successfully inserted; otherwise returns false.
     */
    virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());

    /**
     * Removes \a count rows starting with the given \a row under \a parent parent from the
     * model.
     * Returns true if the rows were successfully removed; otherwise returns false.
     */
    virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

    // RuleTreeModel

    /**
     * Returns the root rule of the model.
     */
    BE::Rule *rootItem();

    /**
     * Sets the root rule of the model.
     */
    void setRootItem(BE::Rule *root);

    /**
     * Returns the BE::Rule object corresponding to the given \a index.
     * Returns an object if the index is valid. Otherwise; returns 0.
     */
    BE::Rule *itemFromIndex(const QModelIndex &index);

    /**
     * Returns the const BE::Rule object corresponding to the given \a index.
     * Returns an object if the index is valid. Otherwise; returns 0.
     */
    const BE::Rule *itemFromIndex(const QModelIndex &index) const;

    /**
     * Returns the model index of the item in the model specified by the given \a row, \a column
     * and \a parent rule.
     */
    QModelIndex index(int row, int column, BE::Rule *parent) const;

    /**
     * Returns the model index of the \a item.
     */
    QModelIndex indexFromItem(const BE::Rule *item);

    /**
     * Appends the given \a item in the model with the given \a parent
     * If the item has no parent, the item is not inserted.
     * Returns true if the item is appended. Otherwise; false.
     */
    bool appendItem(BE::Rule *item, BE::Rule *parent);

    /**
     * Removes all rows in the given \a parent.
     */
    void removeAllRows(const QModelIndex &parent = QModelIndex());

    /**
     * Removes all rows in the given \a parent.
     */
    void removeAllRows(BE::Rule *parent);

    /**
     * Clears the model.
     */
    void clear();

    /**
     * Sets if the items in the model are \a checkable or not.
     */
    void setIsUserCheckable(bool checkable);

    /**
     * Returns if the items in the model are \a checkable or not.
     */
    bool isUserCheckable();

    void notifyDataAboutToChange(); // FIXME refactor to remove this!

    void notifyDataChanged();  // FIXME refactor to remove this!


signals:

    /**
     * This signal is emited whenever a drag and drop operation finishes. \a accepted is true
     * if the drop operation was accepted, otherwise \a is false.
     */
    void dropFinished(bool accepted);

protected:

    QMimeData * mimeData(const QModelIndexList & indexes) const;

    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column,
                      const QModelIndex & parent);

private:

    RuleTreeModel(RuleTreeModel&);
    RuleTreeModel& operator=(RuleTreeModel&);

    int rowForItem(const BE::Rule *item) const;
    int columnCountForItem(const BE::Rule *item) const;
    QVariant dataForItem(const BE::Rule *item, int column, int role) const;
    bool setDataForItem(BE::Rule *item, const QVariant &value, int column, int role);
    void updateCheckStateOnTree(BE::Rule *item, Qt::CheckState state);
    void updateCheckStateOnChildren(BE::Rule *item, Qt::CheckState state);
    void updateCheckStateOnParent(BE::Rule *item, Qt::CheckState state);
    void setCheckState(BE::Rule *item, Qt::CheckState state);

    BE::Rule *m_rootRule;
    bool m_isUserCheckable;
};

/// @}

} // namespace FE

/// @}

} // namespace Lvk

#endif // LVK_FE_RULETREEMODEL_H
