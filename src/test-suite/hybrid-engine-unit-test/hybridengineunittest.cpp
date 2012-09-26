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

#include <QtCore/QString>
#include <QtTest/QtTest>

#include "nlp-engine/hybridengine.h"

//--------------------------------------------------------------------------------------------------
// TestHybridEngine declaration
//--------------------------------------------------------------------------------------------------

class TestHybridEngine : public QObject
{
    Q_OBJECT

public:
    TestHybridEngine();
    ~TestHybridEngine();

private Q_SLOTS:
    void testEngineSyntaticSugar_data();
    void testEngineSyntaticSugar();

    void testEngineSyntaticSugar2_data();
    void testEngineSyntaticSugar2();

    void testExactMatch_data();
    void testExactMatch();

    //TODO void testLocalizedConditionals();

private:
    Lvk::Nlp::HybridEngine *m_engine;

    void setRules1();
    void setRules2();
    void setRules3();

    int m_ruleSet;
};

//--------------------------------------------------------------------------------------------------
// TestHybridEngine definition
//--------------------------------------------------------------------------------------------------

#define RULE_1_ID                           1
#define RULE_1_INPUT_1                      "I was in [place]"
#define RULE_1_OUTPUT_1                     "Got It! You were in [place]"

#define RULE_2_ID                           2
#define RULE_2_INPUT_1                      "Do you like [VarName]"
#define RULE_2_OUTPUT_1                     "{if [VarName] = football} Yes, I like [VarName] {else} No, I don't"

#define USER_INPUT_1                        "I was in cordoba"
#define FRMD_OUTPUT_1                       "Got It! You were in cordoba"

#define USER_INPUT_2a                       "Do you like football?"
#define USER_INPUT_2b                       "Do you like tennis?"
#define FRMD_OUTPUT_2a                      "Yes, I like football"
#define FRMD_OUTPUT_2b                      "No, I don't"

#define RULE_3_ID                           3
#define RULE_3_INPUT_1                      "Do you remember where I was?"
#define RULE_3_OUTPUT_1                     "Sure, you were in [place]"

#define USER_INPUT_3                        "Do you remember where I was"
#define FRMD_OUTPUT_3                       "Sure, you were in cordoba"

#define RULE_4_ID                           4
#define RULE_4_INPUT_1                      "Basket **"
#define RULE_4_INPUT_2                      "Tell me about some sport you play"
#define RULE_4_INPUT_3                      "Basketball **"
#define RULE_4_OUTPUT_1                     "I used to play basket but I'm not very skilled"

#define USER_INPUT_4a                       "Basket"
#define USER_INPUT_4b                       "Basket is a great sport"
#define USER_INPUT_4c                       "You know basket is a great sport"
#define USER_INPUT_4d                       "What I hate most is basket"
#define USER_INPUT_4e                       "Basketball"
#define USER_INPUT_4f                       "Basketball is a great sport"
#define USER_INPUT_4g                       "You know basketball is a great sport"
#define USER_INPUT_4h                       "What I hate most is basketball"

#define RULE_5_ID                           5
#define RULE_5_INPUT_1                      "* seen [movie]"
#define RULE_5_OUTPUT_1                     "[movie]? Nop"

#define USER_INPUT_5                        "Have you seen Pulp Fiction"
#define FRMD_OUTPUT_5                       "Pulp Fiction? Nop"

#define RULE_6_ID                           6
#define RULE_6_INPUT_1                      "Basket *"
#define RULE_6_INPUT_2                      "Football +"
#define RULE_6_INPUT_3                      "* tennis"
#define RULE_6_INPUT_4                      "+ rugby"
#define RULE_6_INPUT_5                      "* soccer *"
#define RULE_6_INPUT_6                      "+ volleyball +"
#define RULE_6_INPUT_7                      "+ box * karate"
#define RULE_6_OUTPUT_1                     "I prefer chess"

#define USER_INPUT_6_1a                     "Basket"
#define USER_INPUT_6_1b                     "Basket is a great sport"
#define USER_INPUT_6_1c                     "You know basket is a great sport"
#define USER_INPUT_6_1d                     "What I hate most is basket"

#define USER_INPUT_6_2a                     "Football"
#define USER_INPUT_6_2b                     "Football is a great sport"
#define USER_INPUT_6_2c                     "You know Football is a great sport"
#define USER_INPUT_6_2d                     "What I hate most is Football"

#define USER_INPUT_6_3a                     "tennis"
#define USER_INPUT_6_3b                     "tennis is a great sport"
#define USER_INPUT_6_3c                     "You know tennis is a great sport"
#define USER_INPUT_6_3d                     "What I hate most is tennis"

#define USER_INPUT_6_4a                     "rugby"
#define USER_INPUT_6_4b                     "rugby is a great sport"
#define USER_INPUT_6_4c                     "You know rugby is a great sport"
#define USER_INPUT_6_4d                     "What I hate most is rugby"

#define USER_INPUT_6_5a                     "soccer"
#define USER_INPUT_6_5b                     "soccer is a great sport"
#define USER_INPUT_6_5c                     "You know soccer is a great sport"
#define USER_INPUT_6_5d                     "What I hate most is soccer"

#define USER_INPUT_6_6a                     "volleyball"
#define USER_INPUT_6_6b                     "volleyball is a great sport"
#define USER_INPUT_6_6c                     "You know volleyball is a great sport"
#define USER_INPUT_6_6d                     "What I hate most is volleyball"

#define USER_INPUT_6_7a                     "What I hate most is box and karate"
#define USER_INPUT_6_7b                     "box and karate"

#define RULE_7_ID                           7
#define RULE_7_INPUT_1                      "\":)\""
#define RULE_7_INPUT_2                      "\":-)\""
#define RULE_7_INPUT_3                      "\":D\""
#define RULE_7_INPUT_4                      "\"Great! :D\""
#define RULE_7_OUTPUT_1                     ":)"

#define USER_INPUT_7a                       ":)"
#define USER_INPUT_7b                       ":-)"
#define USER_INPUT_7c                       ":D"
#define USER_INPUT_7d                       ":("
#define USER_INPUT_7e                       "=)"
#define USER_INPUT_7f                       "Great! :D"
#define USER_INPUT_7g                       "Great :D"
#define USER_INPUT_7h                       "Great D"

#define RULE_8_ID                           8
#define RULE_8_INPUT_1                      "\"Hey!\""
#define RULE_8_INPUT_2                      "Hello!!!"
#define RULE_8_INPUT_3                      "\"Hiii!\""
#define RULE_8_INPUT_4                      "Whats up"
#define RULE_8_OUTPUT_1                     "Hi!"

#define USER_INPUT_8a                       "HEY!"
#define USER_INPUT_8b                       "Hey!!!"
#define USER_INPUT_8c                       "Hey"
#define USER_INPUT_8d                       "Hello!"
#define USER_INPUT_8e                       "HELLO..."
#define USER_INPUT_8f                       "Hi!"
#define USER_INPUT_8g                       "whats up..."


TestHybridEngine::TestHybridEngine()
    : m_engine(new Lvk::Nlp::HybridEngine()), m_ruleSet(0)
{
}

//--------------------------------------------------------------------------------------------------

TestHybridEngine::~TestHybridEngine()
{
    delete m_engine;
}

//--------------------------------------------------------------------------------------------------

void TestHybridEngine::setRules1()
{
    Lvk::Nlp::RuleList rules;

    rules << Lvk::Nlp::Rule(RULE_1_ID,
                            QStringList() << RULE_1_INPUT_1,
                            QStringList() << RULE_1_OUTPUT_1);

    rules << Lvk::Nlp::Rule(RULE_2_ID,
                            QStringList() << RULE_2_INPUT_1,
                            QStringList() << RULE_2_OUTPUT_1);

    rules << Lvk::Nlp::Rule(RULE_3_ID,
                            QStringList() << RULE_3_INPUT_1,
                            QStringList() << RULE_3_OUTPUT_1);

    rules << Lvk::Nlp::Rule(RULE_4_ID,
                            QStringList() << RULE_4_INPUT_1 << RULE_4_INPUT_2 << RULE_4_INPUT_3,
                            QStringList() << RULE_4_OUTPUT_1);

    rules << Lvk::Nlp::Rule(RULE_5_ID, // Test with harmful chars
                            QStringList() << RULE_5_INPUT_1 ">><",
                            QStringList() << RULE_5_OUTPUT_1 "<>>");

    m_engine->setRules(rules);
    m_ruleSet = 1;
}

//--------------------------------------------------------------------------------------------------

void TestHybridEngine::setRules2()
{
    Lvk::Nlp::RuleList rules;

    rules << Lvk::Nlp::Rule(RULE_6_ID,
                            QStringList() << RULE_6_INPUT_1 << RULE_6_INPUT_2 << RULE_6_INPUT_3
                                          << RULE_6_INPUT_4 << RULE_6_INPUT_5 << RULE_6_INPUT_6
                                          << RULE_6_INPUT_7,
                            QStringList() << RULE_6_OUTPUT_1);

    m_engine->setRules(rules);
    m_ruleSet = 2;
}

//--------------------------------------------------------------------------------------------------

void TestHybridEngine::setRules3()
{
    Lvk::Nlp::RuleList rules;

    rules << Lvk::Nlp::Rule(RULE_7_ID,
                            QStringList() << RULE_7_INPUT_1 << RULE_7_INPUT_2 << RULE_7_INPUT_3
                                          << RULE_7_INPUT_4,
                            QStringList() << RULE_7_OUTPUT_1);

    rules << Lvk::Nlp::Rule(RULE_8_ID,
                            QStringList() << RULE_8_INPUT_1 << RULE_8_INPUT_2 << RULE_8_INPUT_3
                                          << RULE_8_INPUT_4,
                            QStringList() << RULE_8_OUTPUT_1);

    m_engine->setRules(rules);
    m_ruleSet = 3;
}
//--------------------------------------------------------------------------------------------------

void TestHybridEngine::testEngineSyntaticSugar_data()
{
    QTest::addColumn<QString>("userInput");
    QTest::addColumn<QString>("expectedOutput");
    QTest::addColumn<int>("ruleId");
    QTest::addColumn<int>("ruleInputNumber");

    QTest::newRow("Variable  1")   << USER_INPUT_1  << FRMD_OUTPUT_1   << RULE_1_ID << 0;
    QTest::newRow("Conditional 1") << USER_INPUT_2a << FRMD_OUTPUT_2a  << RULE_2_ID << 0;
    QTest::newRow("Conditional 2") << USER_INPUT_2b << FRMD_OUTPUT_2b  << RULE_2_ID << 0;
    QTest::newRow("Variable  2")   << USER_INPUT_3  << FRMD_OUTPUT_3   << RULE_3_ID << 0;

    QTest::newRow("Keyword Op 1")  << USER_INPUT_4a << RULE_4_OUTPUT_1 << RULE_4_ID << 0;
    QTest::newRow("Keyword Op 2")  << USER_INPUT_4b << RULE_4_OUTPUT_1 << RULE_4_ID << 0;
    QTest::newRow("Keyword Op 3")  << USER_INPUT_4c << RULE_4_OUTPUT_1 << RULE_4_ID << 0;
    QTest::newRow("Keyword Op 4")  << USER_INPUT_4d << RULE_4_OUTPUT_1 << RULE_4_ID << 0;
    QTest::newRow("Keyword Op 5")  << USER_INPUT_4e << RULE_4_OUTPUT_1 << RULE_4_ID << 2;
    QTest::newRow("Keyword Op 6")  << USER_INPUT_4f << RULE_4_OUTPUT_1 << RULE_4_ID << 2;
    QTest::newRow("Keyword Op 7")  << USER_INPUT_4g << RULE_4_OUTPUT_1 << RULE_4_ID << 2;
    QTest::newRow("Keyword Op 8")  << USER_INPUT_4h << RULE_4_OUTPUT_1 << RULE_4_ID << 2;

    // Not supported
    //QTest::newRow("Mix * sh Var")  << USER_INPUT_5  << FRMD_OUTPUT_5   << RULE_5_ID << 0;
}

//--------------------------------------------------------------------------------------------------

void TestHybridEngine::testEngineSyntaticSugar()
{
    QFETCH(QString, userInput);
    QFETCH(QString, expectedOutput);
    QFETCH(int, ruleId);
    QFETCH(int, ruleInputNumber);

    if (m_ruleSet != 1) {
        setRules1();
    }

    // --- Get Responses ---

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

    // --- Get All Responses ---

    matches.clear();

    QList<QString> outputs = m_engine->getAllResponses(userInput, "", matches);

    if (!expectedOutput.isNull()) {
        QVERIFY(outputs.size() > 0);
        QCOMPARE(outputs[0], expectedOutput);
        QCOMPARE(matches.size(), 1);
        QCOMPARE(matches[0].first, static_cast<Lvk::Nlp::RuleId>(ruleId));
        QCOMPARE(matches[0].second, ruleInputNumber);
    } else {
        QCOMPARE(outputs.size(), 0);
        QCOMPARE(matches.size(), 0);
    }
}

//--------------------------------------------------------------------------------------------------

void TestHybridEngine::testEngineSyntaticSugar2_data()
{
    QTest::addColumn<QString>("userInput");
    QTest::addColumn<QString>("expectedOutput");
    QTest::addColumn<int>("ruleId");
    QTest::addColumn<int>("ruleInputNumber");

    QTest::newRow("wrd * 1")  << USER_INPUT_6_1a << RULE_6_OUTPUT_1 << RULE_6_ID << 0;
    QTest::newRow("wrd * 2")  << USER_INPUT_6_1b << RULE_6_OUTPUT_1 << RULE_6_ID << 0;
    QTest::newRow("wrd * 3")  << USER_INPUT_6_1c << QString()       << RULE_6_ID << 0;
    QTest::newRow("wrd * 4")  << USER_INPUT_6_1d << QString()       << RULE_6_ID << 0;

    QTest::newRow("wrd + 1")  << USER_INPUT_6_2a << QString()       << RULE_6_ID << 0;
    QTest::newRow("wrd + 2")  << USER_INPUT_6_2b << RULE_6_OUTPUT_1 << RULE_6_ID << 1;
    QTest::newRow("wrd + 3")  << USER_INPUT_6_2c << QString()       << RULE_6_ID << 0;
    QTest::newRow("wrd + 4")  << USER_INPUT_6_2d << QString()       << RULE_6_ID << 0;

    QTest::newRow("* wrd 1")  << USER_INPUT_6_3a << RULE_6_OUTPUT_1 << RULE_6_ID << 2;
    QTest::newRow("* wrd 2")  << USER_INPUT_6_3b << QString()       << RULE_6_ID << 0;
    QTest::newRow("* wrd 3")  << USER_INPUT_6_3c << QString()       << RULE_6_ID << 0;
    QTest::newRow("* wrd 4")  << USER_INPUT_6_3d << RULE_6_OUTPUT_1 << RULE_6_ID << 2;

    QTest::newRow("+ wrd 1")  << USER_INPUT_6_4a << QString()       << RULE_6_ID << 0;
    QTest::newRow("+ wrd 2")  << USER_INPUT_6_4b << QString()       << RULE_6_ID << 0;
    QTest::newRow("+ wrd 3")  << USER_INPUT_6_4c << QString()       << RULE_6_ID << 0;
    QTest::newRow("+ wrd 4")  << USER_INPUT_6_4d << RULE_6_OUTPUT_1 << RULE_6_ID << 3;

    QTest::newRow("* w * 1")  << USER_INPUT_6_5a << RULE_6_OUTPUT_1 << RULE_6_ID << 4;
    QTest::newRow("* w * 2")  << USER_INPUT_6_5b << RULE_6_OUTPUT_1 << RULE_6_ID << 4;
    QTest::newRow("* w * 3")  << USER_INPUT_6_5c << RULE_6_OUTPUT_1 << RULE_6_ID << 4;
    QTest::newRow("* w * 4")  << USER_INPUT_6_5d << RULE_6_OUTPUT_1 << RULE_6_ID << 4;

    QTest::newRow("+ w + 1")  << USER_INPUT_6_6a << QString()       << RULE_6_ID << 0;
    QTest::newRow("+ w + 2")  << USER_INPUT_6_6b << QString()       << RULE_6_ID << 0;
    QTest::newRow("+ w + 3")  << USER_INPUT_6_6c << RULE_6_OUTPUT_1 << RULE_6_ID << 5;
    QTest::newRow("+ w + 4")  << USER_INPUT_6_6d << QString()       << RULE_6_ID << 0;

    QTest::newRow("+ w * 1")  << USER_INPUT_6_7a << RULE_6_OUTPUT_1 << RULE_6_ID << 6;
    QTest::newRow("+ w * 2")  << USER_INPUT_6_7b << QString()       << RULE_6_ID << 0;
}

//--------------------------------------------------------------------------------------------------

void TestHybridEngine::testEngineSyntaticSugar2()
{
    //QSKIP("Regex-like operators + and * are no implemented yet", SkipAll);

    QFETCH(QString, userInput);
    QFETCH(QString, expectedOutput);
    QFETCH(int, ruleId);
    QFETCH(int, ruleInputNumber);

    if (m_ruleSet != 2) {
        setRules2();
    }

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

void TestHybridEngine::testExactMatch_data()
{
    QTest::addColumn<QString>("userInput");
    QTest::addColumn<QString>("expectedOutput");
    QTest::addColumn<int>("ruleId");
    QTest::addColumn<int>("ruleInputNumber");

    QTest::newRow(" 1")  << USER_INPUT_7a << RULE_7_OUTPUT_1 << RULE_7_ID << 0;
    QTest::newRow(" 2")  << USER_INPUT_7b << RULE_7_OUTPUT_1 << RULE_7_ID << 1;
    QTest::newRow(" 3")  << USER_INPUT_7c << RULE_7_OUTPUT_1 << RULE_7_ID << 2;
    QTest::newRow(" 4")  << USER_INPUT_7d << QString()       << 0         << 0;
    QTest::newRow(" 5")  << USER_INPUT_7e << QString()       << 0         << 0;
    QTest::newRow(" 6")  << USER_INPUT_7f << RULE_7_OUTPUT_1 << RULE_7_ID << 3;
    QTest::newRow(" 7")  << USER_INPUT_7g << QString()       << 0         << 0;
    QTest::newRow(" 8")  << USER_INPUT_7h << QString()       << 0         << 0;

    QTest::newRow(" 9")  << USER_INPUT_8a << RULE_8_OUTPUT_1 << RULE_8_ID << 0;
    QTest::newRow("10")  << USER_INPUT_8b << QString()       << 0         << 0;
    QTest::newRow("11")  << USER_INPUT_8c << QString()       << 0         << 0;
    QTest::newRow("12")  << USER_INPUT_8d << RULE_8_OUTPUT_1 << RULE_8_ID << 1;
    QTest::newRow("13")  << USER_INPUT_8e << RULE_8_OUTPUT_1 << RULE_8_ID << 1;
    QTest::newRow("14")  << USER_INPUT_8f << QString()       << 0         << 0;
    QTest::newRow("15")  << USER_INPUT_8g << RULE_8_OUTPUT_1 << RULE_8_ID << 3;
}

//--------------------------------------------------------------------------------------------------

void TestHybridEngine::testExactMatch()
{
    QFETCH(QString, userInput);
    QFETCH(QString, expectedOutput);
    QFETCH(int, ruleId);
    QFETCH(int, ruleInputNumber);

    if (m_ruleSet != 3) {
        setRules3();
    }

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
// Test entry point
//--------------------------------------------------------------------------------------------------

QTEST_APPLESS_MAIN(TestHybridEngine)


#include "hybridengineunittest.moc"
