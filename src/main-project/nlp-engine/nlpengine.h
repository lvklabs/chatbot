#ifndef LVK_NLP_NLPENGINE_H
#define LVK_NLP_NLPENGINE_H

#include <QString>
#include <QPair>

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
    typedef QList< QPair<RuleId, int> > MatchList; //! list of pairs (rule_id, matched_input_number)

    virtual const RuleList &rules() const = 0;

    virtual RuleList &rules() = 0;

    virtual void setRules(const RuleList &rules) = 0;

    virtual QString getResponse(const QString &input, MatchList &matches) = 0;

    virtual QList<QString> getAllResponses(const QString &input, MatchList &matches) = 0;

    virtual ~Engine() {}
};

} // namespace Nlp

} // namespace Lvk

#endif // LVK_NLP_NLPENGINE_H
