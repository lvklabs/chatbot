#ifndef LVK_BE_ITEM_H
#define LVK_BE_ITEM_H

#include <QList>
#include <QString>
#include <QVariant>

namespace Lvk
{

namespace BE
{

/**
 * \brief This class provides the abstraction of a pattern-template rule.
 */

class Rule
{
public:
    enum RuleType { OrdinaryRule, EvasiveRule, ContainerRule };

    Rule(Rule *parent = 0);

    Rule(const QString &name, Rule *parent = 0);

    Rule(const QString &name, RuleType type, Rule *parent = 0);

    Rule(const QString &name, const QList<QString> &input, const QList<QString> &ouput,
             Rule *parent = 0);

    ~Rule();


    Rule *parent();

    const Rule *parent() const;


    QList<Rule*> &children();

    const QList<Rule*> &children() const;

    bool appendChild(Rule *child);

    bool insertChildren(int position, int count);

    bool removeChildren(int position, int count);

    void removeAllChild();

    int childCount() const;


    Rule *child(int number);

    const Rule *child(int number) const;


    RuleType type() const;

    void setType(RuleType type);


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
    Rule(Rule&);
    Rule& operator=(Rule&);

    QList<Rule*> m_childItems;
    QString m_name;
    QList<QString> m_input;
    QList<QString> m_ouput;
    Rule *m_parentItem;
    RuleType m_type;
};

QDataStream &operator<<(QDataStream &stream, const Rule &rule);
QDataStream &operator>>(QDataStream &stream, Rule &rule);

} // namespace BE

} // namespace Lvk

#endif // LVK_BE_ITEM_H
