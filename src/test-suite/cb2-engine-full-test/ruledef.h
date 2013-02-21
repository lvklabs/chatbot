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
#define RULE_8_INPUT_2                      ":D"
#define RULE_8_INPUT_3                      ":-)"
#define RULE_8_INPUT_4                      "=)"
#define RULE_8_INPUT_5                      ":P"
#define RULE_8_OUTPUT_1                     ":)"

#define RULE_9_ID                           9
#define RULE_9_INPUT_1                      "'Perros'"
#define RULE_9_INPUT_2                      "'Perrito'"
#define RULE_9_OUTPUT_1                     "?"

#define RULE_10_ID                          10
#define RULE_10_INPUT_1                     " * tengo miedo a [frase]"
#define RULE_10_OUTPUT_1                    "desde cuando le tenes miedo a [frase]?"

#define RULE_11_ID                          11
#define RULE_11_INPUT_1                     " a * b * c * d * e * f *"
#define RULE_11_OUTPUT_1                    "ok"

#define RULE_12_ID                          12
#define RULE_12_INPUT_1                     "[var]"
#define RULE_12_OUTPUT_1                    "r[var]"

#define RULE_13_ID                          13
#define RULE_13_INPUT_1                     "me gusta jugar a [var]"
#define RULE_13_OUTPUT_1                    "{if [var] == futbol} a mi tambien {if [var] == voley} a mi no {else} Si? [var]?"

#define RULE_14_ID                          14
#define RULE_14_INPUT_1                     "w1 [var] w2"
#define RULE_14_OUTPUT_1                    "{if [var] == v1 v2} r1 {if v2 == [var]} r2 {if [var] == [var]} r3"

#define RULE_15_ID                          15
#define RULE_15_INPUT_1                     "Tengo [num] a\xc3\xb1os?"
#define RULE_15_OUTPUT_1                    "{if [num] == 18} igual que yo! {  if  [num]  >  30  } sos un viejo"\
                                            "{if [num] >= 19} sos mas grande que yo {  if [num] == muchos  } sos un viejo"\
                                            "{if [num] >= 1} sos mas chico que yo {if [num]<1} imposible!"

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

    rules << Lvk::Nlp::Rule(RULE_9_ID,
                            QStringList() << RULE_9_INPUT_1 << RULE_9_INPUT_2,
                            QStringList() << RULE_9_OUTPUT_1);

    rules << Lvk::Nlp::Rule(RULE_10_ID,
                            QStringList() << RULE_10_INPUT_1,
                            QStringList() << RULE_10_OUTPUT_1);

    rules << Lvk::Nlp::Rule(RULE_11_ID,
                            QStringList() << RULE_11_INPUT_1,
                            QStringList() << RULE_11_OUTPUT_1);

    rules << Lvk::Nlp::Rule(RULE_13_ID,
                            QStringList() << RULE_13_INPUT_1,
                            QStringList() << RULE_13_OUTPUT_1);

    rules << Lvk::Nlp::Rule(RULE_14_ID,
                            QStringList() << RULE_14_INPUT_1,
                            QStringList() << RULE_14_OUTPUT_1);

    rules << Lvk::Nlp::Rule(RULE_15_ID,
                            QStringList() << QString::fromUtf8(RULE_15_INPUT_1),
                            QStringList() << RULE_15_OUTPUT_1);

    engine->setRules(rules);
}


inline void setRules2(Lvk::Nlp::Engine *engine)
{
    Lvk::Nlp::RuleList rules;

    rules << Lvk::Nlp::Rule(RULE_6_ID,
                            QStringList() << RULE_6_INPUT_1 << RULE_6_INPUT_2,
                            QStringList() << RULE_6_OUTPUT_1);

    rules << Lvk::Nlp::Rule(RULE_12_ID,
                            QStringList() << RULE_12_INPUT_1,
                            QStringList() << RULE_12_OUTPUT_1);

    engine->setRules(rules);
}


#endif // RULEDEF_H
