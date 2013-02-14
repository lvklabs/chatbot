#ifndef RULEDEF_H
#define RULEDEF_H

#include "nlp-engine/engine.h"

#define a_ACUTE     "\xc3\xa1"
#define e_ACUTE     "\xc3\xa9"
#define i_ACUTE     "\xc3\xad"
#define o_ACUTE     "\xc3\xb3"
#define u_ACUTE     "\xc3\xba"
#define A_ACUTE     "\xc3\x81"
#define E_ACUTE     "\xc3\x89"
#define I_ACUTE     "\xc3\x8d"
#define O_ACUTE     "\xc3\x93"
#define U_ACUTE     "\xc3\x9a"
#define u_DIAERESIS "\xc3\xbc"
#define U_DIAERESIS "\xc3\x9c"

#define TARGET_USER_1                       "-1234123431@chat.facebook.com"
#define TARGET_USER_2                       "-1234123432@chat.facebook.com"
#define TARGET_USER_3                       "John R. Smith"
#define TARGET_USER_4                       "Ju" a_ACUTE "n P" e_ACUTE "rez"

#define RULE_1_ID                           1
#define RULE_1_INPUT_1                      "Hello"
#define RULE_1_INPUT_2                      "Hi"
#define RULE_1_INPUT_3                      "Hello +"
#define RULE_1_OUTPUT_1                     "Hi!"
#define RULE_1_OUTPUT_2                     "Hello!"
#define RULE_1_OUTPUT_3                     "Hey!"

#define RULE_2_ID                           2
#define RULE_2_INPUT_1                      "What is your name?"
#define RULE_2_OUTPUT_1                     "R2D2"

#define RULE_3_ID                           3
#define RULE_3_INPUT_1                      "cars"
#define RULE_3_INPUT_2                      "* cars"
#define RULE_3_INPUT_3                      "cars *"
#define RULE_3_INPUT_4                      "* cars *"
#define RULE_3_OUTPUT_1                     "I love cars"

//#define RULE_4_ID                           4
//#define RULE_4_INPUT_1                      "Do you like *"
//#define RULE_4_OUTPUT_1                     "<srai><star/></srai>"

#define RULE_5_ID                           5
#define RULE_5_INPUT_1                      "cats"
#define RULE_5_OUTPUT_1                     "I hate cats!"

#define RULE_6_ID                           6
#define RULE_6_INPUT_1                      "Cu" a_ACUTE "l es tu barrio?"
#define RULE_6_INPUT_2                      "C" O_ACUTE "MO SE LLAMA TU BARRIO?"
#define RULE_6_OUTPUT_1                     "G" u_DIAERESIS "emes"

#define RULE_7_ID                           7
#define RULE_7_INPUT_1                      "Do you like soccer?"
#define RULE_7_OUTPUT_1                     "Sure!"

#define RULE_8_ID                           8
#define RULE_8_INPUT_1                      "Do you like *?"
#define RULE_8_OUTPUT_1                     "Yes I do"

#define RULE_9_ID                           9
#define RULE_9_INPUT_1                      "* you like soccer?"
#define RULE_9_OUTPUT_1                     "Yeap"

#define RULE_10_ID                          10
#define RULE_10_INPUT_1                     "Do you like soccer?"
#define RULE_10_OUTPUT_1                    "Nope"

#define RULE_11_ID                          11
#define RULE_11_INPUT_1                     "*"
#define RULE_11_OUTPUT_1                    "Everything 1"

#define RULE_12_ID                          12
#define RULE_12_INPUT_1                     "*"
#define RULE_12_OUTPUT_1                    "Everything 2!"

#define RULE_13_ID                          13
#define RULE_13_INPUT_1                     "Do you like *?"
#define RULE_13_OUTPUT_1                    "It's my favorite sport!"

#define RULE_14_ID                          14
#define RULE_14_INPUT_1                     "Do you like *?"
#define RULE_14_OUTPUT_1                    "Depends on..."

#define RULE_15_ID                          15
#define RULE_15_INPUT_1                     "\":)\""
#define RULE_15_INPUT_2                     "\":D\""
#define RULE_15_INPUT_3                     "\"Great! :-)\""
#define RULE_15_OUTPUT_1                    ":)"

#define RULE_16_ID                          16
#define RULE_16_INPUT_1                     "\"Hello\""
#define RULE_16_OUTPUT_1                    "Hi!"

#define RULE_17_ID                          17
#define RULE_17_INPUT_1                     "J & J"
#define RULE_17_OUTPUT_1                    "J & J ????????"

#define RULE_18_ID                          18
#define RULE_18_INPUT_1                     "thing1 *"
#define RULE_18_OUTPUT_1                    "No way"

#define RULE_19_ID                          19
#define RULE_19_INPUT_1                     "thing1 *"
#define RULE_19_OUTPUT_1                    "Really?"

#define RULE_20_ID                          20
#define RULE_20_INPUT_1                     "thing2 *"
#define RULE_20_OUTPUT_1                    "Ok"

#define RULE_20_ID                          20
#define RULE_20_INPUT_1                     "thing2 *"
#define RULE_20_OUTPUT_1                    "Ok"

#define RULE_21_ID                          21
#define RULE_21_INPUT_1                     "* \"jugaba en\" *"
#define RULE_21_INPUT_2                     "* \"jugaba, para\" *"
#define RULE_21_OUTPUT_1                    "Por que ahora no?"

#define RULE_22_ID                          22
#define RULE_22_INPUT_1                     "* jugar en *"
#define RULE_22_INPUT_2                     "* jugar, para *"
#define RULE_22_OUTPUT_1                    "Ok"

#define RULE_23_ID                          23
#define RULE_23_INPUT_1                     "\":-)\""
#define RULE_23_OUTPUT_1                    ":D"

#define RULE_24_ID                          24
#define RULE_24_INPUT_1                     "w1 \"w2 w3"
#define RULE_24_INPUT_2                     "w1 \"\" w2 w3 w4"
#define RULE_24_OUTPUT_1                    "w4"

//--------------------------------------------------------------------------------------------------

inline void setRules1(Lvk::Nlp::Engine *engine)
{
    Lvk::Nlp::RuleList rules;

    rules << Lvk::Nlp::Rule(RULE_1_ID,
                            QStringList() << RULE_1_INPUT_1 << RULE_1_INPUT_2 << RULE_1_INPUT_3,
                            QStringList() << RULE_1_OUTPUT_1);

    rules << Lvk::Nlp::Rule(RULE_2_ID,
                            QStringList() << RULE_2_INPUT_1,
                            QStringList() << RULE_2_OUTPUT_1);

    rules << Lvk::Nlp::Rule(RULE_3_ID,
                            QStringList() << RULE_3_INPUT_1 << RULE_3_INPUT_2 << RULE_3_INPUT_3
                                          << RULE_3_INPUT_4,
                            QStringList() << RULE_3_OUTPUT_1);

//    rules << Lvk::Nlp::Rule(RULE_4_ID,
//                            QStringList() << RULE_4_INPUT_1,
//                            QStringList() << RULE_4_OUTPUT_1);

    rules << Lvk::Nlp::Rule(RULE_5_ID,
                            QStringList() << RULE_5_INPUT_1,
                            QStringList() << RULE_5_OUTPUT_1);

    rules << Lvk::Nlp::Rule(RULE_6_ID,
                            QStringList() << QString::fromUtf8(RULE_6_INPUT_1)
                                          << QString::fromUtf8(RULE_6_INPUT_2),
                            QStringList() << QString::fromUtf8(RULE_6_OUTPUT_1));

    rules << Lvk::Nlp::Rule(RULE_17_ID,
                            QStringList() << RULE_17_INPUT_1,
                            QStringList() << RULE_17_OUTPUT_1);

    rules << Lvk::Nlp::Rule(RULE_17_ID,
                            QStringList() << RULE_17_INPUT_1,
                            QStringList() << RULE_17_OUTPUT_1);

    engine->setRules(rules);
}

//--------------------------------------------------------------------------------------------------

inline void setRules2(Lvk::Nlp::Engine *engine)
{
    Lvk::Nlp::RuleList rules;

    rules << Lvk::Nlp::Rule(RULE_1_ID,
                            QStringList() << RULE_1_INPUT_1 << RULE_1_INPUT_2 << RULE_1_INPUT_3,
                            QStringList() << RULE_1_OUTPUT_1 << RULE_1_OUTPUT_2 << RULE_1_OUTPUT_3);

    engine->setRules(rules);
}

//--------------------------------------------------------------------------------------------------

inline void setRules4(Lvk::Nlp::Engine *engine)
{
    Lvk::Nlp::RuleList rules;

    rules << Lvk::Nlp::Rule(RULE_1_ID,
                            QStringList() << RULE_1_INPUT_1 << RULE_1_INPUT_2 << RULE_1_INPUT_3,
                            QStringList() << RULE_1_OUTPUT_1,
                            QStringList() << TARGET_USER_1);

    rules << Lvk::Nlp::Rule(RULE_2_ID,
                            QStringList() << RULE_1_INPUT_1 << RULE_1_INPUT_2 << RULE_1_INPUT_3,
                            QStringList() << RULE_1_OUTPUT_1,
                            QStringList() << TARGET_USER_2);

    rules << Lvk::Nlp::Rule(RULE_3_ID,
                            QStringList() << RULE_3_INPUT_1,
                            QStringList() << RULE_3_OUTPUT_1,
                            QStringList() << TARGET_USER_2 << QString::fromUtf8(TARGET_USER_4));

    rules << Lvk::Nlp::Rule(RULE_7_ID,
                            QStringList() << RULE_7_INPUT_1,
                            QStringList() << RULE_7_OUTPUT_1);

    rules << Lvk::Nlp::Rule(RULE_6_ID,
                            QStringList() << QString::fromUtf8(RULE_6_INPUT_1)
                                          << QString::fromUtf8(RULE_6_INPUT_2),
                            QStringList() << QString::fromUtf8(RULE_6_OUTPUT_1),
                            QStringList() << TARGET_USER_1 << TARGET_USER_2 << TARGET_USER_3);

    engine->setRules(rules);
}

//--------------------------------------------------------------------------------------------------

inline void setRules5(Lvk::Nlp::Engine *engine)
{
    Lvk::Nlp::RuleList rules;

    rules << Lvk::Nlp::Rule(RULE_11_ID,
                            QStringList() << RULE_11_INPUT_1,
                            QStringList() << RULE_11_OUTPUT_1,
                            QStringList());

    rules << Lvk::Nlp::Rule(RULE_7_ID,
                            QStringList() << RULE_7_INPUT_1,
                            QStringList() << RULE_7_OUTPUT_1,
                            QStringList());

    rules << Lvk::Nlp::Rule(RULE_8_ID,
                            QStringList() << RULE_8_INPUT_1,
                            QStringList() << RULE_8_OUTPUT_1,
                            QStringList());

    rules << Lvk::Nlp::Rule(RULE_9_ID,
                            QStringList() << RULE_9_INPUT_1,
                            QStringList() << RULE_9_OUTPUT_1,
                            QStringList());

    rules << Lvk::Nlp::Rule(RULE_10_ID,
                            QStringList() << RULE_10_INPUT_1,
                            QStringList() << RULE_10_OUTPUT_1,
                            QStringList() << TARGET_USER_1);

    rules << Lvk::Nlp::Rule(RULE_12_ID,
                            QStringList() << RULE_12_INPUT_1,
                            QStringList() << RULE_12_OUTPUT_1,
                            QStringList());

    rules << Lvk::Nlp::Rule(RULE_14_ID,
                            QStringList() << RULE_14_INPUT_1,
                            QStringList() << RULE_14_OUTPUT_1,
                            QStringList() << TARGET_USER_1);

    engine->setRules(rules);
}

//--------------------------------------------------------------------------------------------------

inline void setRules6(Lvk::Nlp::Engine *engine)
{
    Lvk::Nlp::RuleList rules;

    // TOPIC 1 --------------------

    QString topicSoccer = "soccer";

    rules << Lvk::Nlp::Rule(RULE_7_ID,
                            QStringList() << RULE_7_INPUT_1,
                            QStringList() << RULE_7_OUTPUT_1);

    rules[0].setTopic(topicSoccer);

    rules << Lvk::Nlp::Rule(RULE_18_ID,
                            QStringList() << RULE_18_INPUT_1,
                            QStringList() << RULE_18_OUTPUT_1);

    rules[1].setTopic(topicSoccer);


    // TOPIC 2 --------------------

    QString topicCars = "\"cars\"";

    rules << Lvk::Nlp::Rule(RULE_19_ID,
                            QStringList() << RULE_19_INPUT_1,
                            QStringList() << RULE_19_OUTPUT_1);

    rules[2].setTopic(topicCars);

    rules << Lvk::Nlp::Rule(RULE_3_ID,
                            QStringList() << RULE_3_INPUT_1 << RULE_3_INPUT_2 << RULE_3_INPUT_3,
                            QStringList() << RULE_3_OUTPUT_1);

    rules[3].setTopic(topicCars);

    rules << Lvk::Nlp::Rule(RULE_20_ID,
                            QStringList() << RULE_20_INPUT_1,
                            QStringList() << RULE_20_OUTPUT_1);

    rules[4].setTopic(topicCars);

    engine->setRules(rules);
}

//--------------------------------------------------------------------------------------------------

inline void setRules7(Lvk::Nlp::Engine *engine)
{
    Lvk::Nlp::RuleList rules;

    rules << Lvk::Nlp::Rule(RULE_15_ID,
                            QStringList() << RULE_15_INPUT_1 << RULE_15_INPUT_2 << RULE_15_INPUT_3,
                            QStringList() << RULE_15_OUTPUT_1,
                            QStringList());

    rules << Lvk::Nlp::Rule(RULE_16_ID,
                            QStringList() << RULE_16_INPUT_1,
                            QStringList() << RULE_16_OUTPUT_1,
                            QStringList());

    engine->setRules(rules);
}

//--------------------------------------------------------------------------------------------------

inline void setRules8(Lvk::Nlp::Engine *engine)
{
    Lvk::Nlp::RuleList rules;

    rules << Lvk::Nlp::Rule(RULE_21_ID,
                            QStringList() << RULE_21_INPUT_1 << RULE_21_INPUT_2,
                            QStringList() << RULE_21_OUTPUT_1);

    rules << Lvk::Nlp::Rule(RULE_22_ID,
                            QStringList() << RULE_22_INPUT_1 << RULE_22_INPUT_2,
                            QStringList() << RULE_22_OUTPUT_1);

    rules << Lvk::Nlp::Rule(RULE_23_ID,
                            QStringList() << RULE_23_INPUT_1,
                            QStringList() << RULE_23_OUTPUT_1);

    rules << Lvk::Nlp::Rule(RULE_24_ID,
                            QStringList() << RULE_24_INPUT_1 << RULE_24_INPUT_2,
                            QStringList() << RULE_24_OUTPUT_1);

    engine->setRules(rules);
}

#endif // RULEDEF_H
