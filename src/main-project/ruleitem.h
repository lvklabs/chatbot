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


    RuleItem *parent();

    const RuleItem *parent() const;


    QList<RuleItem*> &children();

    const QList<RuleItem*> &children() const;

    bool appendChild(RuleItem *child);

    bool insertChildren(int position, int count);

    bool removeChildren(int position, int count);

    int childCount() const;

    RuleItem *child(int number);


    ItemType type() const;

    void setType(ItemType type);


    QString &name();

    const QString &name() const;

    void setName(const QString &name);


    QList<QString> &input();

    const QList<QString> &input() const;

    void setInput(const QList<QString> &input);


    QList<QString> &output();

    const QList<QString> &output() const;

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
