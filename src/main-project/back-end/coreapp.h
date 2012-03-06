#ifndef LVK_BE_COREAPP_H
#define LVK_BE_COREAPP_H

#include <QString>
#include <QStringList>
#include <QHash>
#include <QPair>

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

    typedef QList< QPair<Rule *, int> > MatchList;

    QString getResponse(const QString &input, MatchList &matches);

    void refreshNlpEngine();

private:
    QString m_filename;
    Rule *m_rootRule;
    Nlp::Engine *m_nlpEngine;
    Nlp::RuleId m_nextRuleId;
    QHash<Nlp::RuleId, Rule *> m_rulesHash;
    QStringList m_evasives;

    void buildNlpRulesOf(Rule* parentRule, Nlp::RuleList &nlpRules);
};

} // namespace BE

} // namespace Lvk

#endif // LVK_BE_COREAPP_H
