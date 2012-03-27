#include "simpleaimlengine.h"
#include "nlprule.h"

#include <exception>

#define VAR_NAME_REGEX  "\\[([A-Za-z_]+)\\]"
#define IF_REGEX        "\\{\\s*if\\s*" VAR_NAME_REGEX "\\s*=\\s*([^}]+)\\}([^{]*)"
#define ELSE_REGEX      "{\\s*else\\s*}(.*)"

//--------------------------------------------------------------------------------------------------
// SimpleAimlEngine::InvalidSyntaxException
//--------------------------------------------------------------------------------------------------

class Lvk::Nlp::SimpleAimlEngine::InvalidSyntaxException : public std::exception
{
public:
    InvalidSyntaxException(const QString &what) throw()
        : m_what(what.toUtf8())
    {
    }

    ~InvalidSyntaxException() throw() {}

    virtual const char* what() throw()
    {
        return m_what.constData();
    }

private:
    QByteArray m_what;
};

//--------------------------------------------------------------------------------------------------
// SimpleAimlEngine
//--------------------------------------------------------------------------------------------------

Lvk::Nlp::SimpleAimlEngine::SimpleAimlEngine()
    : AimlEngine()
{
}

//--------------------------------------------------------------------------------------------------

Lvk::Nlp::SimpleAimlEngine::SimpleAimlEngine(Sanitizer *sanitizer)
    : AimlEngine(sanitizer)
{
}

//--------------------------------------------------------------------------------------------------

const Lvk::Nlp::RuleList & Lvk::Nlp::SimpleAimlEngine::rules() const
{
    return m_rules;
}

//--------------------------------------------------------------------------------------------------

Lvk::Nlp::RuleList & Lvk::Nlp::SimpleAimlEngine::rules()
{
    return m_rules;
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::SimpleAimlEngine::setRules(const Lvk::Nlp::RuleList &rules)
{
    m_rules = rules;

    RuleList pureAimlRules;
    buildPureAimlRules(pureAimlRules);

    AimlEngine::setRules(pureAimlRules);
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::SimpleAimlEngine::buildPureAimlRules(Lvk::Nlp::RuleList &pureAimlRules) const
{
    pureAimlRules.clear();

    for (int i = 0; i < m_rules.size(); ++i) {
        try {
            Lvk::Nlp::Rule pureAimlRule;
            buildPureAimlRule(pureAimlRule, m_rules[i]);
            pureAimlRules.append(pureAimlRule);
        } catch (std::exception &) {
            // Nothing to to. If the rule is invalid, we just skip it
        }
    }
}

void Lvk::Nlp::SimpleAimlEngine::buildPureAimlRule(Lvk::Nlp::Rule &pureAimlRule,
                                                   const Lvk::Nlp::Rule &rule) const
{
    QString varNameOnInput;

    pureAimlRule.setId(rule.id());
    buildPureAimlInputList(pureAimlRule.input(), varNameOnInput, rule.input());
    buildPureAimlOutputList(pureAimlRule.output(), varNameOnInput, rule.output());
}

void Lvk::Nlp::SimpleAimlEngine::buildPureAimlInputList(QStringList &pureAimlInputList,
                                                        QString &varNameOnInput,
                                                        const QStringList &inputList) const
{
    QRegExp varNameRegex(VAR_NAME_REGEX);

    pureAimlInputList.clear();
    varNameOnInput.clear();

    /*
     * For each input, convert strings like:
     *    Do you like [VarName]
     *
     * To pure AIML:
     *    Do you like *
     */

    for (int i = 0; i < inputList.size(); ++i) {
        const QString &input = inputList[i];
        QString pureAimlInput = input;

        int pos = varNameRegex.indexIn(input);

        // If variable decl found
        if (pos != -1) {

            // Two or more variable decl in the same input
            if (pos != varNameRegex.lastIndexIn(input)) {
                throw InvalidSyntaxException(
                        QObject::tr("A rule input cannot contain two or more variables"));
            }

            if (!varNameOnInput.isNull() && varNameOnInput != varNameRegex.cap(1)) {
                throw InvalidSyntaxException(
                        QObject::tr("Rules cannot contain two or more different variable names"));
            }

            varNameOnInput = varNameRegex.cap(1);

            pureAimlInput.replace(varNameRegex, " * ");
        }

        pureAimlInputList.append(pureAimlInput);
    }
}

void Lvk::Nlp::SimpleAimlEngine::buildPureAimlOutputList(QStringList &pureAimlOutputList,
                                                         const QString &varNameOnInput,
                                                         const QStringList &outputList) const
{
    QRegExp varNameRegex(VAR_NAME_REGEX);
//    QRegExp ifRegex(IF_REGEX);
//    QRegExp elseRegex(ELSE_REGEX);

    pureAimlOutputList.clear();

    /*
     * For each output, convert strings like:
     *    {if [VarName] = football}
     *       Yes I like [VarName]
     *    {else}
     *       No, I don't
     *
     * To pure AIML:
     *    <think><set name="like"><star/></set></think>
     *    <condition>
     *       <li name="like" value="football"> Yes I like <star/></li>
     *       <li>No, I don't</li>
     *    </condition>
     */

    for (int i = 0; i < outputList.size(); ++i) {
        const QString &output = outputList[i];
        QString pureAimlOutput = output;

        int pos = 0;

        while (pos != -1) {
            pos = varNameRegex.indexIn(output, pos);

            if (pos != -1) {
                QString varName = varNameRegex.cap(1);

                if (varName == varNameOnInput) {
                    pureAimlOutput.replace("[" + varName + "]", "<star/>", Qt::CaseInsensitive);
                } else {
                    pureAimlOutput.replace("[" + varName + "]", "<get name=\"" + varName + "\" />",
                                           Qt::CaseInsensitive);
                }

                pos++;
            }

        }

        if (!varNameOnInput.isNull()) {
            pureAimlOutput.prepend("<think><set name=\"" + varNameOnInput + "\"><star/></set></think>");
        }

        pureAimlOutputList.append(pureAimlOutput);
    }
}
