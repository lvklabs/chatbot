#ifndef LVK_BE_COREAPP_H
#define LVK_BE_COREAPP_H

#include <QString>

#include "nlprule.h"

namespace Lvk
{

namespace Nlp
{
    class Engine;
}

namespace BE
{

class Rule;


/**
 * \brief This class provides the core functionality of the application.
 */

class CoreApp
{
public:
    CoreApp();

    ~CoreApp();

    bool load(const QString &filename);

    bool saveAs(const QString &filename);

    bool save();

    void close();

    Rule *rootRule();

    QString getResponse(const QString &input, QList<Rule *> &matched);

    void refreshNlpEngine();

private:
    QString m_filename;
    Rule *m_rootRule;
    Nlp::Engine *m_nlpEngine;

    void buildNlpRulesOf(const Rule* parentRule, Nlp::RuleList &nlpRules);
};

} // namespace BE

} // namespace Lvk

#endif // LVK_BE_COREAPP_H
