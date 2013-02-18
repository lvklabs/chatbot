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

#define RULE_4_ID                           4
#define RULE_4_INPUT_1                      "Te gusta [algo]?"
#define RULE_4_INPUT_2                      "Te gusta [algo] mucho?"
#define RULE_4_OUTPUT_1                     "Si me gusta [algo]"

#define RULE_5_ID                           5
#define RULE_5_INPUT_1                      "Te gusta [algo] mas que [algo2]?"
#define RULE_5_OUTPUT_1                     "Entre [algo] y [algo2] no se..."

#define RULE_6_ID                           6
#define RULE_6_INPUT_1                      "Simplemente [var]"
#define RULE_6_INPUT_2                      "Solamente [var]"
#define RULE_6_OUTPUT_1                     "R[var]"

#define RULE_7_ID                           7
#define RULE_7_INPUT_1                      "w1 [var1] w2 [var2] w3 [var3] w4"
#define RULE_7_OUTPUT_1                     "w5 [var2] w6 R[var3] w7 [var1] w8"

#define RULE_8_ID                           8
#define RULE_8_INPUT_1                      ":)"
#define RULE_8_INPUT_2                      ":'("
#define RULE_8_INPUT_3                      ":-)"
#define RULE_8_INPUT_4                      "=)"
#define RULE_8_INPUT_5                      ":P"
#define RULE_8_OUTPUT_1                     ":)"

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

    rules << Lvk::Nlp::Rule(RULE_4_ID,
                            QStringList() << RULE_4_INPUT_1 << RULE_4_INPUT_2,
                            QStringList() << RULE_4_OUTPUT_1);

    rules << Lvk::Nlp::Rule(RULE_5_ID,
                            QStringList() << RULE_5_INPUT_1,
                            QStringList() << RULE_5_OUTPUT_1);

    rules << Lvk::Nlp::Rule(RULE_6_ID,
                            QStringList() << RULE_6_INPUT_1 << RULE_6_INPUT_2,
                            QStringList() << RULE_6_OUTPUT_1);

    rules << Lvk::Nlp::Rule(RULE_7_ID,
                            QStringList() << RULE_7_INPUT_1,
                            QStringList() << RULE_7_OUTPUT_1);

    rules << Lvk::Nlp::Rule(RULE_8_ID,
                            QStringList() << RULE_8_INPUT_1  << RULE_8_INPUT_2 << RULE_8_INPUT_3
                                          << RULE_8_INPUT_4  << RULE_8_INPUT_5,
                            QStringList() << RULE_8_OUTPUT_1);

    engine->setRules(rules);
}


#endif // RULEDEF_H
