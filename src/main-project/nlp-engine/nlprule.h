#ifndef LVK_NLP_NLPRULE_H
#define LVK_NLP_NLPRULE_H

#include <QList>
#include <QString>
#include <QStringList>

namespace Lvk
{

namespace Nlp
{

/**
 * \brief Rule ID
 */

typedef unsigned long RuleId;

/**
 * \brief NLP Rule abstraction
 */

class Rule
{
public:

    Rule(RuleId id = 0) : m_id(id) {}

    Rule(RuleId id, const QStringList &input, const QStringList &output)
        : m_id(id), m_input(input), m_output(output) {}


    RuleId id() { return m_id; }

    void setId(RuleId id) { m_id = id; }


    const QStringList &input() const { return m_input; }

    QStringList &input() { return m_input; }

    void setInput(const QStringList &input) { m_input = input; }


    const QStringList output() const { return m_output; }

    QStringList output() { return m_output; }

    void setOuput(const QStringList &output) { m_output = output; }

private:

    RuleId m_id;
    QStringList m_input;
    QStringList m_output;
};


/**
 * \brief List of NLP Rules
 */

typedef QList<Rule> RuleList;

} // namespace Nlp

} // namespace Lvk

#endif // LVK_NLP_NLPRULE_H
