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
 * This class is most commonly used with QTreeView to display a hierarchy of rules.
 *
 * \see BE::Rule
 */

class RuleTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    RuleTreeModel(BE::Rule *rootRule, QObject *parent = 0);

    ~RuleTreeModel();

    // QAbstractItemModel - read only models

    virtual QVariant data(const QModelIndex &index, int role) const;

    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;

    virtual QModelIndex parent(const QModelIndex &index) const;

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;

    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

    // QAbstractItemModel - editable models

    virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    virtual Qt::DropActions supportedDropActions() const;

    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    virtual bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value,
                               int role = Qt::EditRole);

    virtual bool insertRows(int position, int rows, const QModelIndex &parent = QModelIndex());

    virtual bool removeRows(int position, int rows, const QModelIndex &parent = QModelIndex());

    // RuleTreeModel

    BE::Rule *rootItem();

    void setRootItem(BE::Rule *root);

    BE::Rule *itemFromIndex(const QModelIndex &index);

    const BE::Rule *itemFromIndex(const QModelIndex &index) const;

    QModelIndex index(int row, int column, BE::Rule *parentItem) const;

    QModelIndex indexFromItem(const BE::Rule *item);

    bool appendItem(BE::Rule *item);

    void removeAllRows(const QModelIndex &parent = QModelIndex());

    void removeAllRows(BE::Rule *parent);


    void clear();


    void notifyDataAboutToChange(); // FIXME refactor to remove this!

    void notifyDataChanged();  // FIXME refactor to remove this!


    void setIsUserCheckable(bool checkable);

    bool isUserCheckable();

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
