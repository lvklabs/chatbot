#ifndef RULEDEF_H
#define RULEDEF_H

#include "nlp-engine/engine.h"

#define RULE_1_ID                           1
#define RULE_1_INPUT_1                      "Hola"
#define RULE_1_INPUT_2                      "Hola +"
#define RULE_1_OUTPUT_1                     "Hola!"

#define RULE_2_ID                           2
#define RULE_2_INPUT_1                      "Chau *"
#define RULE_2_INPUT_2                      "* Chau"
#define RULE_2_OUTPUT_1                     "Chau!"

#define RULE_3_ID                           3
#define RULE_3_INPUT_1                      "* jugar * futbol *"
#define RULE_3_OUTPUT_1                     "Yo tambien"

//--------------------------------------------------------------------------------------------------

inline void setRules1(Lvk::Nlp::Engine *engine)
{
    Lvk::Nlp::RuleList rules;

    rules << Lvk::Nlp::Rule(RULE_1_ID,
                            QStringList() << RULE_1_INPUT_1 << RULE_1_INPUT_2,
                            QStringList() << RULE_1_OUTPUT_1);

    rules << Lvk::Nlp::Rule(RULE_2_ID,
                            QStringList() << RULE_2_INPUT_1 << RULE_2_INPUT_2,
                            QStringList() << RULE_2_OUTPUT_1);

    rules << Lvk::Nlp::Rule(RULE_3_ID,
                            QStringList() << RULE_3_INPUT_1,
                            QStringList() << RULE_3_OUTPUT_1);

    engine->setRules(rules);
}


#endif // RULEDEF_H
