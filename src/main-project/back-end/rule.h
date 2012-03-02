#ifndef RULEITEM_H
#define RULEITEM_H

#include <QList>
#include <QString>
#include <QVariant>

namespace Lvk
{

namespace BE
{

class Rule
{
public:
    enum RuleType { FinalRule, ContainerRule };

    Rule(Rule *parent = 0);

    Rule(const QString &name, Rule *parent = 0);

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

    int childCount() const;

    Rule *child(int number);


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
    QList<Rule*> m_childItems;
    QString m_name;
    QList<QString> m_input;
    QList<QString> m_ouput;
    Rule *m_parentItem;
    RuleType m_type;
};

} // namespace BE

} // namespace Lvk

#endif // RULEITEM_H
