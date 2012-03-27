#ifndef LVK_NLP_SIMPLEAIMLENGINE_H
#define LVK_NLP_SIMPLEAIMLENGINE_H

#include "aimlengine.h"

namespace Lvk
{

namespace Nlp
{

/**
 * \brief SimpleAimlEngine is a wrapper over the AimlEngine to provide a simpler syntax than AIML
 *
 * For instance instead of writing:
 *
 * Input: Do you like *
 * Output: <think><set name="like"><star/></set></think>
 *         <condition>
 *             <li name="like" value="football"> Yes I like <star/></li>
 *             <li>No, I don't</li>
 *         </condition>
 *
 * With SimpleAimlEngine we can write:
 *
 * Input: Do you like [VarName]
 * Output: {if [VarName] = football}
 *         Yes I like [VarName]
 *         {else}
 *         No, I don't
 */

class SimpleAimlEngine : public AimlEngine
{
public:
    SimpleAimlEngine();

    SimpleAimlEngine(Sanitizer *sanitizer);

    virtual const RuleList &rules() const;

    virtual RuleList &rules();

    virtual void setRules(const RuleList &rules);

    class InvalidSyntaxException;

private:
    RuleList m_rules;

    void buildPureAimlRules(RuleList &aimlRules) const;
    void buildPureAimlRule(Lvk::Nlp::Rule &pureAimlRules, const Lvk::Nlp::Rule &rule) const;
    void buildPureAimlInputList(QStringList &pureAimlInputList,
                                QString &varNameOnInput,
                                const QStringList &inputList) const;
    void buildPureAimlOutputList(QStringList &pureAimlOutputList,
                                 const QString &varNameOnInput,
                                 const QStringList &outputList) const;
};

} // namespace Nlp

} // namespace Lvk

#endif // LVK_NLP_SIMPLEAIMLENGINE_H
