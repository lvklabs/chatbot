#ifndef RULEITEM_H
#define RULEITEM_H

#include <QList>
#include <QString>
#include <QVariant>

namespace Lvk
{

class RuleItem
{
public:
    enum ItemType { Rule, CategoryRule };

    RuleItem(RuleItem *parent = 0);

    RuleItem(const QString &name, RuleItem *parent = 0);

    RuleItem(const QString &name, const QList<QString> &input, const QList<QString> &ouput,
             RuleItem *parent = 0);

    ~RuleItem();

    RuleItem *child(int row);

    int childCount() const;

    int columnCount() const;

    int row() const;

    RuleItem *parent();

    const RuleItem *parent() const;

    QVariant data(int column, int role = Qt::DisplayRole) const;

    bool setData(const QVariant &value, int column, int role = Qt::DisplayRole);

    ItemType type() const;

    void setType(ItemType type);

    bool appendChild(RuleItem *child);

    bool insertChildren(int position, int count);

    bool removeChildren(int position, int count);

    QList<QString> input() const;

    void setInput(const QList<QString> &input);

    QList<QString> output() const;

    void setOutput(const QList<QString> &output);

private:
    QList<RuleItem*> m_childItems;
    QString m_name;
    QList<QString> m_input;
    QList<QString> m_ouput;
    RuleItem *m_parentItem;
    ItemType m_type;
};

} // namespace Lvk

#endif // RULEITEM_H
