#ifndef LVK_NLP_NLPENGINE_H
#define LVK_NLP_NLPENGINE_H

#include <QString>

#include "nlprule.h"

namespace Lvk
{

namespace Nlp
{

/**
 * \brief Abstract interface for NLP Engines
 */

class Engine
{
public:
    virtual const RuleList &rules() const = 0;

    virtual RuleList &rules() = 0;

    virtual void setRules(const RuleList &rules) = 0;

    virtual void addRule(const Rule &rule) = 0;

    virtual void removeRule(const Rule &rule) = 0;

    virtual void updateRule(const Rule &rule) = 0;

    virtual QString getResponse(const QString &input) = 0;

    virtual QString getResponse(const QString &input, RuleList &matched) = 0;
};

} // namespace Nlp

} // namespace Lvk

#endif // LVK_NLP_NLPENGINE_H
