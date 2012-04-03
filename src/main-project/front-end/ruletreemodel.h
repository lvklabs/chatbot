/*
 * Copyright (C) 2012 Andres Pagliano, Gabriel Miretti, Gonzalo Buteler,
 * Nestor Bustamante, Pablo Perez de Angelis
 *
 * This file is part of LVK Botmaster.
 *
 * LVK Botmaster is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * LVK Botmaster is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LVK Botmaster.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef LVK_FE_RULETREEMODEL_H
#define LVK_FE_RULETREEMODEL_H

#include <QAbstractItemModel>

namespace Lvk
{

namespace BE
{
    class Rule;
}

namespace FE
{

/**
 * \brief This class implementes the abstract class QAbstractItemModel to display a tree of @class
 *        Rule in a QTreeView class
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

    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    virtual bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value,
                               int role = Qt::EditRole);

    virtual bool insertRows(int position, int rows, const QModelIndex &parent = QModelIndex());

    virtual bool removeRows(int position, int rows, const QModelIndex &parent = QModelIndex());

    // RuleTreeModel

    BE::Rule *invisibleRootItem();

    BE::Rule *itemFromIndex(const QModelIndex &index);

    QModelIndex index(int row, int column, BE::Rule *parentItem) const;

    QModelIndex indexFromItem(const BE::Rule *item);

    bool appendItem(BE::Rule *item);

    void removeAllRows(const QModelIndex &parent = QModelIndex());

    void removeAllRows(BE::Rule *parent);

    void clear();

private:
    RuleTreeModel(RuleTreeModel&);
    RuleTreeModel& operator=(RuleTreeModel&);

    int rowForItem(const BE::Rule *item) const;
    int columnCountForItem(const BE::Rule *item) const;
    QVariant dataForItem(const BE::Rule *item, int column, int role) const;
    bool setDataForItem(BE::Rule *item, const QVariant &value, int column, int role);

    BE::Rule *m_rootRule;
};

} // namespace FE

} // namespace Lvk

#endif // LVK_FE_RULETREEMODEL_H
