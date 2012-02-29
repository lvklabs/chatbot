#include "rule.h"

Lvk::Rule::Rule()
    : m_id(0)
{
}

Lvk::Rule::Rule(int id)
    : m_id(id)
{
}

Lvk::Rule::Rule(int id, const QList<QString> &input, const QList<QString> &output)
    : m_id(id), m_input(input), m_output(output)
{
}

void Lvk::Rule::setInput(QList<QString> &input)
{
    m_input = input;
}

void Lvk::Rule::setOuput(QList<QString> &output)
{
    m_output = output;
}

QList<QString> & Lvk::Rule::input()
{
    return m_input;
}

QList<QString> & Lvk::Rule::ouput()
{
    return m_output;
}

