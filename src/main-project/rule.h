#ifndef RULE_H
#define RULE_H

#include <QList>
#include <QString>

namespace Lvk
{

class Rule
{
public:
    Rule();
    Rule(int id);
    Rule(int id, const QList<QString> &input, const QList<QString> &output);

    void setInput(QList<QString> &input);
    void setOuput(QList<QString> &output);

    QList<QString> & input();
    QList<QString> & ouput();

private:
    int m_id;
    QList<QString> m_input;
    QList<QString> m_output;
};

} // namespace Lvk

#endif // RULE_H
