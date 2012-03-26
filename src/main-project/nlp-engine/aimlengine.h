#ifndef LVK_NLP_AIMLENGINE_H
#define LVK_NLP_AIMLENGINE_H

#include "nlpengine.h"

class AIMLParser;

namespace Lvk
{

namespace Nlp
{

class Sanitizer;

/**
 * \brief NLP Engine based on AIML (Artificial Intelligence Markup Language)
 */

class AimlEngine : public Engine
{
public:
    AimlEngine();

    AimlEngine(Sanitizer *sanitizer);

    ~AimlEngine();

    virtual const RuleList &rules() const;

    virtual RuleList &rules();

    virtual void setRules(const RuleList &rules);

    virtual QString getResponse(const QString &input, MatchList &matches);

    virtual QList<QString> getAllResponses(const QString &input, MatchList &matches);

private:
    AimlEngine(AimlEngine&);
    AimlEngine& operator=(AimlEngine&);

    RuleList m_rules;
    AIMLParser *m_aimlParser;
    Sanitizer *m_sanitizer;

    void init();
    void buildAiml(QString &aiml);
};

} // namespace Nlp

} // namespace Lvk

#endif // LVK_NLP_AIMLENGINE_H
