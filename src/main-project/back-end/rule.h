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
 * \brief This class provides a NLP rule plus some metadata
 */

class Rule
{
private:

    template <typename T>
    class generic_iterator
    {
    public:
        generic_iterator(T rule = 0) : m_rule(rule) { }

        T operator*() { return m_rule; }

        generic_iterator<T> operator++()
        {
            // Depth-first search

            if (m_rule->childCount() > 0) {
                m_rule = m_rule->child(0);
            } else {
                do {
                    T nextSibling = m_rule->nextSibling();

                    if (nextSibling) {
                        m_rule = nextSibling;
                        break;
                    }

                    m_rule = m_rule->parent();
                } while (m_rule);
            }

            return *this;
        }

        bool operator==(const generic_iterator& it)
        {
            return m_rule == it.m_rule;
        }

        bool operator!=(const generic_iterator& it)
        {
            return !operator==(it);
        }

    private:
        T m_rule;
    };


public:
    enum Type { OrdinaryRule, EvasiveRule, ContainerRule };

    enum Status { Saved, Unsaved };

    Rule(Rule *parent = 0);

    Rule(const QString &name, Rule *parent = 0);

    Rule(const QString &name, Type type, Rule *parent = 0);

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


    Rule *nextSibling();

    const Rule *nextSibling() const;


    Type type() const;

    void setType(Type type);


    const QString &name() const;

    void setName(const QString &name);


    QList<QString> &input();

    const QList<QString> &input() const;

    void setInput(const QList<QString> &input);


    QList<QString> &output();

    const QList<QString> &output() const;

    void setOutput(const QList<QString> &output);


    bool enabled();

    void setEnabled(bool enabled);


    Status status();

    void setStatus(Status status);

    /**
      * \brief Rule iterator
      */
    typedef generic_iterator<Rule *> iterator;

    /**
      * \brief Rule const iterator
      */
    typedef generic_iterator<const Rule *> const_iterator;


    iterator begin() { return iterator(this); }

    iterator end() { return iterator(0); }

    const_iterator begin() const { return const_iterator(this); }

    const_iterator end() const { return const_iterator(0); }

private:
    Rule(Rule&);
    Rule& operator=(Rule&);

    QList<Rule*> m_childItems;
    QString m_name;
    QList<QString> m_input;
    QList<QString> m_ouput;
    Rule *m_parentItem;
    Type m_type;
    bool m_enabled;
    Status m_status;
};

QDataStream &operator<<(QDataStream &stream, const Rule &rule);
QDataStream &operator>>(QDataStream &stream, Rule &rule);

} // namespace BE

} // namespace Lvk

#endif // LVK_BE_ITEM_H
