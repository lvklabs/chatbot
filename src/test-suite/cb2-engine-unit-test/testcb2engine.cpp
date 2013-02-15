/*
 * Copyright (C) 2012 Andres Pagliano, Gabriel Miretti, Gonzalo Buteler,
 * Nestor Bustamante, Pablo Perez de Angelis
 *
 * This file is part of LVK Botmaster.
 *
 * LVK Botmaster is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * LVK Botmaster is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LVK Botmaster.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <QString>
#include <QtTest/QtTest>
#include <QHash>
#include <QRegExp>
#include <QIODevice>

#include <iostream>

#include "nlp-engine/cb2engine.h"
#include "nlp-engine/rule.h"
#include "nlp-engine/nlpproperties.h"
#include "nlp-engine/defaultsanitizer.h"
#include "nlp-engine/nullsanitizer.h"
#include "nlp-engine/nulllemmatizer.h"
#include "nlp-engine/sanitizerfactory.h"

#include "ruledef.h"
#include "mocklemmatizer.h"

#define EnableTestMatchWithSingleOutput
#define EnableTestMatchWithSingleOutputWithLemmatizer
#define EnableTestMatchWithTarget
#define EnableTestMatchPriority
//#define EnableTestExactMatch
#define EnableTestMatchWithSecuentialOutput

#define USER_INPUT_1a                       "Hello"
#define USER_INPUT_1b                       "hello"
#define USER_INPUT_1c                       "HELLO"
#define USER_INPUT_1d                       "HELLO,"
#define USER_INPUT_1e                       "HELLO;!?"
#define USER_INPUT_1f                       "Heeeeellooooooo"
#define USER_INPUT_2                        "Hi"
#define USER_INPUT_3                        "Hey there!"
#define USER_INPUT_4a                       "What is your name?"
#define USER_INPUT_4b                       "What   is your    name?"
#define USER_INPUT_4c                       "What   is your    name"
#define USER_INPUT_5                        "Hello there!"
#define USER_INPUT_6                        "Hello how are you!!"
#define USER_INPUT_7a                       "cars"
#define USER_INPUT_7b                       "Do you like cars ?"
#define USER_INPUT_7c                       "cars are the best!"
#define USER_INPUT_7d                       "Have you seen the latest cars that BMW have launched?"
#define USER_INPUT_8a                       "Do you like cats?"
#define USER_INPUT_8b                       "Do you like robots?"
#define USER_INPUT_8c                       "Do you like soccer?"
#define USER_INPUT_9a                       "Cual es tu barrio"
#define USER_INPUT_9b                       "Cu" a_ACUTE "l es tu barrio?"
#define USER_INPUT_9c                       "C" U_ACUTE "AL " E_ACUTE "S TU BARRIO?"
#define USER_INPUT_9d                       "como    se   llama, tu barrio"
#define USER_INPUT_10a                      ":)"
#define USER_INPUT_10b                      ":D"
#define USER_INPUT_10c                      "Great! :-)"
#define USER_INPUT_10d                      "Great :-)"
#define USER_INPUT_10e                      "Great"
#define USER_INPUT_17                       "J & J"
#define USER_INPUT_18                       "thing1 is great"
#define USER_INPUT_19                       "thing1 is great!!"
#define USER_INPUT_20                       "thing2 rules!"
#define USER_INPUT_21a                      "Yo jugaba en river"
#define USER_INPUT_21b                      "Yo jugaba para river"
#define USER_INPUT_21c                      "Yo jugaba, para river"
#define USER_INPUT_21d                      "Yo juego en river"
#define USER_INPUT_22a                      "jugar en"
#define USER_INPUT_22b                      "jugar para"
#define USER_INPUT_23a                      ":-)"
#define USER_INPUT_23b                      ":)"
#define USER_INPUT_24a                       "w1 w2 w3"
#define USER_INPUT_24b                       "w1 w2 w3 w4"


//--------------------------------------------------------------------------------------------------
// TestCb2Engine declaration
//--------------------------------------------------------------------------------------------------

class TestCb2Engine : public QObject
{
    Q_OBJECT

public:
    TestCb2Engine();

private Q_SLOTS:
    void initTestCase();

    void testMatchWithSingleOutput_data();
    void testMatchWithSingleOutput();

    void testMatchWithSingleOutputWithLemmatizer_data();
    void testMatchWithSingleOutputWithLemmatizer();

    void testMatchWithSecuentialOutput_data();
    void testMatchWithSecuentialOutput();

    void testMatchWithRandomOutput_data();
    void testMatchWithRandomOutput();

    void testMatchWithTarget_data();
    void testMatchWithTarget();

    void testMatchPriority_data();
    void testMatchPriority();

    void testExactMatch_data();
    void testExactMatch();

    void testMatchWithTopic();

    void cleanupTestCase();

private:

    Lvk::Nlp::Cb2Engine *m_engine;

    void testMatch(void (*setRulesFunc)(Lvk::Nlp::Engine *engine), Lvk::Nlp::Cb2Engine *engine);
};

TestCb2Engine::TestCb2Engine()
    : m_engine(0)
{
}

//--------------------------------------------------------------------------------------------------
// TestCb2Engine definition
//--------------------------------------------------------------------------------------------------


void TestCb2Engine::initTestCase()
{
    m_engine = new Lvk::Nlp::Cb2Engine(new Lvk::Nlp::NullSanitizer());
}

//--------------------------------------------------------------------------------------------------

void TestCb2Engine::cleanupTestCase()
{
    delete m_engine;
    m_engine = 0;
}

//--------------------------------------------------------------------------------------------------

void TestCb2Engine::testMatch(void (*setRulesFunc)(Lvk::Nlp::Engine *engine),
                              Lvk::Nlp::Cb2Engine *engine)
{
    QFETCH(QString, targetUser);
    QFETCH(QString, userInput);
    QFETCH(QString, expectedOutput);
    QFETCH(int, ruleId);
    QFETCH(int, ruleInputNumber);

    engine->setLemmatizer(new MockLemmatizer());

    setRulesFunc(engine);

    Lvk::Nlp::Engine::MatchList matches;
    QString output;

    output = engine->getResponse(userInput, targetUser, matches);

    if (!expectedOutput.isNull()) {
        QCOMPARE(output, expectedOutput);
        QCOMPARE(matches.size(), 1);
        QCOMPARE(matches[0].first, static_cast<Lvk::Nlp::RuleId>(ruleId));
        QCOMPARE(matches[0].second, ruleInputNumber);
    } else {
        QVERIFY(output.isEmpty());
        QCOMPARE(matches.size(), 0);
    }
}

//--------------------------------------------------------------------------------------------------

void TestCb2Engine::testMatchWithSingleOutput_data()
{
    QTest::addColumn<QString>("userInput");
    QTest::addColumn<QString>("expectedOutput");
    QTest::addColumn<int>("ruleId");
    QTest::addColumn<int>("ruleInputNumber");

    QTest::newRow("so 1")  << USER_INPUT_1a << RULE_1_OUTPUT_1  << RULE_1_ID << 0;
    QTest::newRow("so 2")  << USER_INPUT_1b << QString()        << 0 << 0;
    QTest::newRow("so 3")  << USER_INPUT_1c << QString()        << 0 << 0;
    QTest::newRow("so 4")  << USER_INPUT_2  << RULE_1_OUTPUT_1  << RULE_1_ID << 1;
    QTest::newRow("so 5")  << USER_INPUT_4a << RULE_2_OUTPUT_1  << RULE_2_ID << 0;
    QTest::newRow("so 6")  << USER_INPUT_4b << RULE_2_OUTPUT_1  << RULE_2_ID << 0;
    QTest::newRow("so 6")  << USER_INPUT_4c << QString()        << 0 << 0;
    QTest::newRow("so 7")  << USER_INPUT_5  << RULE_1_OUTPUT_1  << RULE_1_ID << 2;
    QTest::newRow("so 8")  << USER_INPUT_6  << RULE_1_OUTPUT_1  << RULE_1_ID << 2;
    QTest::newRow("so 9")  << USER_INPUT_7a << RULE_3_OUTPUT_1  << RULE_3_ID << 0;
    QTest::newRow("so 10") << USER_INPUT_7b << RULE_3_OUTPUT_1  << RULE_3_ID << 1;
    QTest::newRow("so 11") << USER_INPUT_7c << RULE_3_OUTPUT_1  << RULE_3_ID << 2;
    QTest::newRow("so 12") << USER_INPUT_7d << RULE_3_OUTPUT_1  << RULE_3_ID << 3;
    QTest::newRow("so 17") << USER_INPUT_17 << RULE_17_OUTPUT_1 << RULE_17_ID << 0;
}

//--------------------------------------------------------------------------------------------------

void TestCb2Engine::testMatchWithSingleOutput()
{
#ifndef EnableTestMatchWithSingleOutput
    QSKIP("Skip macro on", SkipAll);
#endif

    QFETCH(QString, userInput);
    QFETCH(QString, expectedOutput);
    QFETCH(int, ruleId);
    QFETCH(int, ruleInputNumber);

    m_engine->setLemmatizer(new Lvk::Nlp::NullLemmatizer());

    setRules1(m_engine);

    Lvk::Nlp::Engine::MatchList matches;

    QString output = m_engine->getResponse(userInput, matches);

    if (!expectedOutput.isNull()) {
        QCOMPARE(output, expectedOutput);
        QCOMPARE(matches.size(), 1);
        QCOMPARE(matches[0].first, static_cast<Lvk::Nlp::RuleId>(ruleId));
        QCOMPARE(matches[0].second, ruleInputNumber);
    } else {
        QVERIFY(output.isEmpty());
        QCOMPARE(matches.size(), 0);
    }
}

//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------

void TestCb2Engine::testMatchWithSingleOutputWithLemmatizer_data()
{
    QTest::addColumn<QString>("userInput");
    QTest::addColumn<QString>("expectedOutput");
    QTest::addColumn<int>("ruleId");
    QTest::addColumn<int>("ruleInputNumber");

    QTest::newRow("so 1")  << USER_INPUT_1a << RULE_1_OUTPUT_1  << RULE_1_ID << 0;
    QTest::newRow("so 2")  << USER_INPUT_1b << RULE_1_OUTPUT_1  << RULE_1_ID << 0;
    QTest::newRow("so 3")  << USER_INPUT_1c << RULE_1_OUTPUT_1  << RULE_1_ID << 0;
    QTest::newRow("so 4")  << USER_INPUT_2  << RULE_1_OUTPUT_1  << RULE_1_ID << 1;
    QTest::newRow("so 5")  << USER_INPUT_4a << RULE_2_OUTPUT_1  << RULE_2_ID << 0;
    QTest::newRow("so 6")  << USER_INPUT_4b << RULE_2_OUTPUT_1  << RULE_2_ID << 0;
    QTest::newRow("so 6b") << USER_INPUT_4c << RULE_2_OUTPUT_1  << RULE_2_ID << 0;
    QTest::newRow("so 7")  << USER_INPUT_5  << RULE_1_OUTPUT_1  << RULE_1_ID << 2;
    QTest::newRow("so 8")  << USER_INPUT_6  << RULE_1_OUTPUT_1  << RULE_1_ID << 2;
    QTest::newRow("so 9")  << USER_INPUT_7a << RULE_3_OUTPUT_1  << RULE_3_ID << 0;
    QTest::newRow("so 10") << USER_INPUT_7b << RULE_3_OUTPUT_1  << RULE_3_ID << 1;
    QTest::newRow("so 11") << USER_INPUT_7c << RULE_3_OUTPUT_1  << RULE_3_ID << 2;
    QTest::newRow("so 12") << USER_INPUT_7d << RULE_3_OUTPUT_1  << RULE_3_ID << 3;
    QTest::newRow("so 14") << USER_INPUT_8b << QString()        << 0 << 0;
    QTest::newRow("so 15") << USER_INPUT_1d << RULE_1_OUTPUT_1  << RULE_1_ID << 0;
    QTest::newRow("so 17") << USER_INPUT_17 << RULE_17_OUTPUT_1 << RULE_17_ID << 0;

    QTest::newRow("ds 1") << QString::fromUtf8(USER_INPUT_9a)
                                           << QString::fromUtf8(RULE_6_OUTPUT_1)
                                           << RULE_6_ID << 0;
    QTest::newRow("ds 2") << QString::fromUtf8(USER_INPUT_9b)
                                           << QString::fromUtf8(RULE_6_OUTPUT_1)
                                           << RULE_6_ID << 0;
    QTest::newRow("ds 3") << QString::fromUtf8(USER_INPUT_9c)
                                           << QString::fromUtf8(RULE_6_OUTPUT_1)
                                           << RULE_6_ID << 0;
    QTest::newRow("ds 4") << QString::fromUtf8(USER_INPUT_9d)
                                           << QString::fromUtf8(RULE_6_OUTPUT_1)
                                           << RULE_6_ID << 1;

}

//--------------------------------------------------------------------------------------------------

void TestCb2Engine::testMatchWithSingleOutputWithLemmatizer()
{
#ifndef EnableTestMatchWithSingleOutputWithLemmatizer
    QSKIP("Skip macro on", SkipAll);
#endif

    QFETCH(QString, userInput);
    QFETCH(QString, expectedOutput);
    QFETCH(int, ruleId);
    QFETCH(int, ruleInputNumber);

    m_engine->setLemmatizer(new MockLemmatizer());

    setRules1(m_engine);

    Lvk::Nlp::Engine::MatchList matches;

    QString output = m_engine->getResponse(userInput, matches);

    if (!expectedOutput.isNull()) {
        QCOMPARE(output, expectedOutput);
        QCOMPARE(matches.size(), 1);
        QCOMPARE(matches[0].first, static_cast<Lvk::Nlp::RuleId>(ruleId));
        QCOMPARE(matches[0].second, ruleInputNumber);
    } else {
        QVERIFY(output.isEmpty());
        QCOMPARE(matches.size(), 0);
    }
}

//--------------------------------------------------------------------------------------------------

void TestCb2Engine::testMatchWithRandomOutput_data()
{
    QTest::addColumn<QString>("userInput");
    QTest::addColumn<QStringList>("expectedOutput");

    QStringList RULE_1_OUTPUT_LIST(QStringList() << RULE_1_OUTPUT_1 << RULE_1_OUTPUT_2
                                                 << RULE_1_OUTPUT_3);

    QTest::newRow("ro 1") << USER_INPUT_1a << RULE_1_OUTPUT_LIST;
    QTest::newRow("ro 2") << USER_INPUT_1b << RULE_1_OUTPUT_LIST;
    QTest::newRow("ro 3") << USER_INPUT_1c << RULE_1_OUTPUT_LIST;
    QTest::newRow("ro 4") << USER_INPUT_2  << RULE_1_OUTPUT_LIST;
    QTest::newRow("ro 5") << USER_INPUT_5  << RULE_1_OUTPUT_LIST;
    QTest::newRow("ro 6") << USER_INPUT_6  << RULE_1_OUTPUT_LIST;
}

//--------------------------------------------------------------------------------------------------

void TestCb2Engine::testMatchWithRandomOutput()
{
    QSKIP("Not supported", SkipAll);

    QFETCH(QString, userInput);
    QFETCH(QStringList, expectedOutput);

    QHash<QString, int> outputUseCount;

    setRules2(m_engine);

    for (int i = 0; i < 50; ++i) {

        Lvk::Nlp::Engine::MatchList matches;

        QString output = m_engine->getResponse(userInput, matches);

        QVERIFY(expectedOutput.contains(output));
        QCOMPARE(matches.size(), 1);

        outputUseCount[output] = outputUseCount[output] + 1;
    }

    QCOMPARE(outputUseCount.size(), expectedOutput.size());
}

//--------------------------------------------------------------------------------------------------

void TestCb2Engine::testMatchWithSecuentialOutput_data()
{
    QTest::addColumn<QString>("userInput");
    QTest::addColumn<QStringList>("expectedOutput");

    QStringList RULE_1_OUTPUT_LIST(QStringList() << RULE_1_OUTPUT_1 << RULE_1_OUTPUT_2
                                                 << RULE_1_OUTPUT_3);

    QTest::newRow("so 1") << USER_INPUT_1a << RULE_1_OUTPUT_LIST;
    QTest::newRow("so 2") << USER_INPUT_2 << RULE_1_OUTPUT_LIST;
}

//--------------------------------------------------------------------------------------------------

void TestCb2Engine::testMatchWithSecuentialOutput()
{
#ifndef EnableTestMatchWithSecuentialOutput
    QSKIP("Skip macro on", SkipAll);
#endif

    QFETCH(QString, userInput);
    QFETCH(QStringList, expectedOutput);

    setRules2(m_engine);

    Lvk::Nlp::Engine::MatchList matches;

    for (int i = 0; i < 10; ++i) {
        QString output = m_engine->getResponse(userInput, matches);

        QCOMPARE(matches.size(), 1);
        QCOMPARE(expectedOutput[i % expectedOutput.size()], output);
    }
}

//--------------------------------------------------------------------------------------------------

void TestCb2Engine::testMatchWithTarget_data()
{
    QTest::addColumn<QString>("targetUser");
    QTest::addColumn<QString>("userInput");
    QTest::addColumn<QString>("expectedOutput");
    QTest::addColumn<int>("ruleId");
    QTest::addColumn<int>("ruleInputNumber");

    QTest::newRow("Match, user 1")      << TARGET_USER_1 << USER_INPUT_1a
                                        << RULE_1_OUTPUT_1 << RULE_1_ID << 0;

    QTest::newRow("Match, user 2")      << TARGET_USER_2 << USER_INPUT_1a
                                        << RULE_1_OUTPUT_1 << RULE_2_ID << 0;

    QTest::newRow("No Match, user 3")   << TARGET_USER_3 << USER_INPUT_1a
                                        << QString() << -1 << -1;

    QTest::newRow("No Match, user 4")   << QString::fromUtf8(TARGET_USER_4)
                                        << USER_INPUT_1a << QString() << -1 << -1;

    QTest::newRow("NoTargets, match 1") << TARGET_USER_1 << USER_INPUT_8c
                                        << RULE_7_OUTPUT_1 << RULE_7_ID << 0;

    QTest::newRow("NoTargets, match 2") << QString::fromUtf8(TARGET_USER_4) << USER_INPUT_8c
                                        << RULE_7_OUTPUT_1 << RULE_7_ID << 0;

}

//--------------------------------------------------------------------------------------------------

void TestCb2Engine::testMatchWithTarget()
{
#ifndef EnableTestMatchWithTarget
    QSKIP("Skip macro on", SkipAll);
#endif

    testMatch(setRules4, m_engine);
}

//--------------------------------------------------------------------------------------------------

void TestCb2Engine::testMatchPriority_data()
{
    QTest::addColumn<QString>("targetUser");
    QTest::addColumn<QString>("userInput");
    QTest::addColumn<QString>("expectedOutput");
    QTest::addColumn<int>("ruleId");
    QTest::addColumn<int>("ruleInputNumber");

    // 1. Prefer rules with target over rules without target
    // 2. Prefer rules without * over rules with *

    QTest::newRow("P1 r simple w/target")        << TARGET_USER_1 << USER_INPUT_8c
                                                 << RULE_10_OUTPUT_1 << RULE_10_ID << 0;

    QTest::newRow("P2 r w/* w/target")           << TARGET_USER_1 << USER_INPUT_8a
                                                 << RULE_14_OUTPUT_1 << RULE_14_ID << 0;

    QTest::newRow("P3 r simple wo/target")       << TARGET_USER_2 << USER_INPUT_8c
                                                 << RULE_7_OUTPUT_1 << RULE_7_ID << 0;

    QTest::newRow("P4 r w/* wo/target")          << TARGET_USER_2 << USER_INPUT_8a
                                                 << RULE_8_OUTPUT_1 << RULE_8_ID << 0;
}

//--------------------------------------------------------------------------------------------------

void TestCb2Engine::testMatchPriority()
{
#ifndef EnableTestMatchPriority
    QSKIP("Skip macro on", SkipAll);
#endif

    testMatch(setRules5, m_engine);
}

//--------------------------------------------------------------------------------------------------

void TestCb2Engine::testMatchWithTopic()
{
    QSKIP("Not supported", SkipAll);

    m_engine->setLemmatizer(new MockLemmatizer());

    setRules6(m_engine);

    m_engine->setProperty(NLP_PROP_PREFER_CUR_TOPIC, true);

    Lvk::Nlp::Engine::MatchList matches;
    QString response;

    // Verify that we prefer rules with the current topic

    for (int i = 0; i < 2; ++i) {
        response = m_engine->getResponse(USER_INPUT_8c, matches);
        QCOMPARE(response, QString(RULE_7_OUTPUT_1));
        QCOMPARE(matches.size(), 1);
        QCOMPARE(matches[0].first, (Lvk::Nlp::RuleId)RULE_7_ID);

        response = m_engine->getResponse(USER_INPUT_18, matches);
        QCOMPARE(response, QString(RULE_18_OUTPUT_1));
        QCOMPARE(matches.size(), 1);
        QCOMPARE(matches[0].first, (Lvk::Nlp::RuleId)RULE_18_ID);

        response = m_engine->getResponse(USER_INPUT_20, matches);
        QCOMPARE(response, QString(RULE_20_OUTPUT_1));
        QCOMPARE(matches.size(), 1);
        QCOMPARE(matches[0].first, (Lvk::Nlp::RuleId)RULE_20_ID);

        response = m_engine->getResponse(USER_INPUT_19, matches);
        QCOMPARE(response, QString(RULE_19_OUTPUT_1));
        QCOMPARE(matches.size(), 1);
        QCOMPARE(matches[0].first, (Lvk::Nlp::RuleId)RULE_19_ID);
    }
}

//--------------------------------------------------------------------------------------------------

void TestCb2Engine::testExactMatch_data()
{
    QTest::addColumn<QString>("targetUser");
    QTest::addColumn<QString>("userInput");
    QTest::addColumn<QString>("expectedOutput");
    QTest::addColumn<int>("ruleId");
    QTest::addColumn<int>("ruleInputNumber");

    QTest::newRow("em 1") << "" << USER_INPUT_21a << RULE_21_OUTPUT_1 << RULE_21_ID << 0;
    QTest::newRow("em 2") << "" << USER_INPUT_21b << RULE_22_OUTPUT_1 << RULE_22_ID << 1;
    QTest::newRow("em 3") << "" << USER_INPUT_21c << RULE_21_OUTPUT_1 << RULE_21_ID << 1;
    QTest::newRow("em 4") << "" << USER_INPUT_21d << RULE_22_OUTPUT_1 << RULE_22_ID << 0;
    QTest::newRow("em 5") << "" << USER_INPUT_22a << RULE_22_OUTPUT_1 << RULE_22_ID << 0;
    QTest::newRow("em 6") << "" << USER_INPUT_22b << RULE_22_OUTPUT_1 << RULE_22_ID << 1;
    QTest::newRow("em 7") << "" << USER_INPUT_23a << RULE_23_OUTPUT_1 << RULE_23_ID << 0;
    QTest::newRow("em 8") << "" << USER_INPUT_23b << QString()        << 0          << 0;
    QTest::newRow("em 9") << "" << USER_INPUT_24a << RULE_24_OUTPUT_1 << RULE_24_ID << 0;
    QTest::newRow("em 9") << "" << USER_INPUT_24b << RULE_24_OUTPUT_1 << RULE_24_ID << 1;
}

//--------------------------------------------------------------------------------------------------

void TestCb2Engine::testExactMatch()
{
#ifndef EnableTestExactMatch
    QSKIP("Skip macro on", SkipAll);
#endif

    testMatch(setRules8, m_engine);
}

//--------------------------------------------------------------------------------------------------
// Test entry point
//--------------------------------------------------------------------------------------------------

QTEST_MAIN(TestCb2Engine)

#include "testcb2engine.moc"
