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
#include <QHash>
#include <QRegExp>
#include <QIODevice>

#include <iostream>

#include "nlp-engine/aimlengine.h"
#include "ProgramQ/aimlparser.h"
#include "nlp-engine/rule.h"
#include "nlp-engine/defaultsanitizer.h"
#include "nlp-engine/nullsanitizer.h"

//--------------------------------------------------------------------------------------------------
// TestAimlEngine declaration
//--------------------------------------------------------------------------------------------------

class TestAimlEngine : public QObject
{
    Q_OBJECT

public:
    TestAimlEngine();

private Q_SLOTS:
    void initTestCase();

    void testAimlParserRegression();

    void testMatchWithSingleOutput_data();
    void testMatchWithSingleOutput();

    void testMatchWithRandomOutput_data();
    void testMatchWithRandomOutput();

    void testMatchWithDefaultSanitizer_data();
    void testMatchWithDefaultSanitizer();

    void testMatchWithTarget_data();
    void testMatchWithTarget();

    void testMatchPriority_data();
    void testMatchPriority();

    void testMatchWithTopic();

    void cleanupTestCase();

private:

    Lvk::Nlp::AimlEngine *m_engine;
    Lvk::Nlp::AimlEngine *m_engineWithDefSanitizer;

    void testMatch(void (TestAimlEngine::*setRulesMemb)());

    void setRules1();
    void setRules2();
    void setRules3();
    void setRules4();
    void setRules5();
    void setRules6();
};

TestAimlEngine::TestAimlEngine()
    : m_engine(0), m_engineWithDefSanitizer(0)
{
}

//--------------------------------------------------------------------------------------------------
// TestAimlEngine definition
//--------------------------------------------------------------------------------------------------

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
#define RULE_1_INPUT_3                      "Hello *"
#define RULE_1_OUTPUT_1                     "Hi!"
#define RULE_1_OUTPUT_2                     "Hello!"
#define RULE_1_OUTPUT_3                     "Hey!"

#define RULE_2_ID                           2
#define RULE_2_INPUT_1                      "What is your name?"
#define RULE_2_OUTPUT_1                     "R2D2"

#define RULE_3_ID                           3
#define RULE_3_INPUT_1                      "cars"
#define RULE_3_INPUT_2                      "_ cars"
#define RULE_3_INPUT_3                      "cars _"
#define RULE_3_INPUT_4                      "_ cars *"
#define RULE_3_OUTPUT_1                     "I love cars"

#define RULE_4_ID                           4
#define RULE_4_INPUT_1                      "Do you like *"
#define RULE_4_OUTPUT_1                     "<srai><star/></srai>"

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

#define USER_INPUT_1a                       "Hello"
#define USER_INPUT_1b                       "hello"
#define USER_INPUT_1c                       "HELLO"
#define USER_INPUT_1d                       "HELLO,"
#define USER_INPUT_1e                       "HELLO;!?"
#define USER_INPUT_2                        "Hi"
#define USER_INPUT_3                        "Hey there!"
#define USER_INPUT_4a                       "What is your name?"
#define USER_INPUT_4b                       "What   is your    name"
#define USER_INPUT_5                        "Hello there!"
#define USER_INPUT_6                        "Hello how are you!!"
#define USER_INPUT_7a                       "cars"
#define USER_INPUT_7b                       "Do you like cars?"
#define USER_INPUT_7c                       "cars are the best!"
#define USER_INPUT_7d                       "Have you seen the latest cars that BMW have launched?"
#define USER_INPUT_8a                       "Do you like cats?"
#define USER_INPUT_8b                       "Do you like robots?"
#define USER_INPUT_8c                       "Do you like soccer?"
#define USER_INPUT_9a                       "Cual es tu barrio"
#define USER_INPUT_9b                       "Cu" a_ACUTE "l es tu barrio?"
#define USER_INPUT_9c                       "C" U_ACUTE "AL " E_ACUTE "S TU BARRIO????"
#define USER_INPUT_9d                       "como    se   llama, tu barrio"

//--------------------------------------------------------------------------------------------------

void TestAimlEngine::initTestCase()
{
    m_engine = new Lvk::Nlp::AimlEngine(new Lvk::Nlp::NullSanitizer());

    unsigned options = Lvk::Nlp::DefaultSanitizer::RemoveDiacritic |
                       Lvk::Nlp::DefaultSanitizer::RemovePunctuation;

    m_engineWithDefSanitizer = new Lvk::Nlp::AimlEngine(new Lvk::Nlp::DefaultSanitizer(options));
}

//--------------------------------------------------------------------------------------------------

void TestAimlEngine::cleanupTestCase()
{
    delete m_engine;
    m_engine = 0;

    delete m_engineWithDefSanitizer;
    m_engineWithDefSanitizer = 0;
}

//--------------------------------------------------------------------------------------------------

// This regression test was borrowed from ProgramQ
void TestAimlEngine::testAimlParserRegression()
{
    AIMLParser parser(new QFile("aiml_parser.log"));

    parser.loadAiml("AimlParserRegression.aiml");

    QDomDocument doc;

    {
        QFile file("AimlParserRegression.xml");

        if (!file.open(IO_ReadOnly)) {
            QFAIL("Cannot open AimlParserRegression.xml");
        }

        if (!doc.setContent(&file)) {
            QFAIL("Cannot parse AimlParserRegression.xml");
        }
    }

    QStringList skippedTestList;

    {
        QFile skipFile("AimlParserRegressionSkipList.txt");

        if (skipFile.open(QFile::ReadOnly)) {
            skippedTestList = QString(skipFile.readAll()).split("\n");
        }
    }

    QDomElement docElem = doc.documentElement();
    QDomNodeList testCaseList = docElem.elementsByTagName ("TestCase");

    int failed = 0;

    for (int i = 0; i < testCaseList.count(); i++) {
        QDomElement n = testCaseList.item(i).toElement();
        QString name = n.attribute("name");

        if (skippedTestList.contains(name)) {
            // Skip test
            continue;
        }

        QString description = n.namedItem("Description").firstChild().nodeValue();
        QString input = n.namedItem("Input").firstChild().nodeValue();

        QString expectedAnswer = "";
        QDomNode child = n.namedItem("ExpectedAnswer").firstChild();

        while (!child.isNull()) {
            if (child.isText()) {
                expectedAnswer += child.toText().nodeValue();
            }
            child = child.nextSibling();
        }

        QString answer = parser.getResponse(input);

        answer = answer.simplifyWhiteSpace().lower();
        expectedAnswer = expectedAnswer.simplifyWhiteSpace().lower();

        if (answer != expectedAnswer) {
           std::cout << "=======================================================\n";
           std::cout << "FAILED:\n";
           std::cout << " - Name           : " + name.toStdString()           + "\n";
           std::cout << " - Description    : " + description.toStdString()    + "\n";
           std::cout << " - Answer         : " + answer.toStdString()         + "\n";
           std::cout << " - Expected answer: " + expectedAnswer.toStdString() + "\n";
           failed++;
        }
    }

    QCOMPARE(failed, 0);
}
//--------------------------------------------------------------------------------------------------


void TestAimlEngine::testMatch(void (TestAimlEngine::*setRulesMemb)())
{
    QFETCH(QString, targetUser);
    QFETCH(QString, userInput);
    QFETCH(QString, expectedOutput);
    QFETCH(int, ruleId);
    QFETCH(int, ruleInputNumber);

    (this->*setRulesMemb)();

    Lvk::Nlp::Engine::MatchList matches;
    QString output;

    output = m_engine->getResponse(userInput, targetUser, matches);

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

void TestAimlEngine::setRules1()
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

    rules << Lvk::Nlp::Rule(RULE_4_ID,
                            QStringList() << RULE_4_INPUT_1,
                            QStringList() << RULE_4_OUTPUT_1);

    rules << Lvk::Nlp::Rule(RULE_5_ID,
                            QStringList() << RULE_5_INPUT_1,
                            QStringList() << RULE_5_OUTPUT_1);

    m_engine->setRules(rules);
}

//--------------------------------------------------------------------------------------------------

void TestAimlEngine::testMatchWithSingleOutput_data()
{
    QTest::addColumn<QString>("userInput");
    QTest::addColumn<QString>("expectedOutput");
    QTest::addColumn<int>("ruleId");
    QTest::addColumn<int>("ruleInputNumber");

    QTest::newRow("AIML 1")  << USER_INPUT_1a << RULE_1_OUTPUT_1 << RULE_1_ID << 0;
    QTest::newRow("AIML 2")  << USER_INPUT_1b << RULE_1_OUTPUT_1 << RULE_1_ID << 0;
    QTest::newRow("AIML 3")  << USER_INPUT_1c << RULE_1_OUTPUT_1 << RULE_1_ID << 0;
    QTest::newRow("AIML 4")  << USER_INPUT_2  << RULE_1_OUTPUT_1 << RULE_1_ID << 1;
    QTest::newRow("AIML 5")  << USER_INPUT_4a << RULE_2_OUTPUT_1 << RULE_2_ID << 0;
    QTest::newRow("AIML 6")  << USER_INPUT_4b << RULE_2_OUTPUT_1 << RULE_2_ID << 0;
    QTest::newRow("AIML 7")  << USER_INPUT_5  << RULE_1_OUTPUT_1 << RULE_1_ID << 2;
    QTest::newRow("AIML 8")  << USER_INPUT_6  << RULE_1_OUTPUT_1 << RULE_1_ID << 2;
    QTest::newRow("AIML 9")  << USER_INPUT_7a << RULE_3_OUTPUT_1 << RULE_3_ID << 0;
    QTest::newRow("AIML 10") << USER_INPUT_7b << RULE_3_OUTPUT_1 << RULE_3_ID << 1;
    QTest::newRow("AIML 11") << USER_INPUT_7c << RULE_3_OUTPUT_1 << RULE_3_ID << 2;
    QTest::newRow("AIML 12") << USER_INPUT_7d << RULE_3_OUTPUT_1 << RULE_3_ID << 3;
    QTest::newRow("AIML 13") << USER_INPUT_8a << RULE_5_OUTPUT_1 << RULE_5_ID << 0;
    QTest::newRow("AIML 14") << USER_INPUT_8b << QString()       << 0 << 0;
    QTest::newRow("AIML 15") << USER_INPUT_1d << RULE_1_OUTPUT_1 << RULE_1_ID << 0;
    QTest::newRow("AIML 16") << USER_INPUT_1e << RULE_1_OUTPUT_1 << RULE_1_ID << 0;
}

//--------------------------------------------------------------------------------------------------

void TestAimlEngine::testMatchWithSingleOutput()
{
    QFETCH(QString, userInput);
    QFETCH(QString, expectedOutput);
    QFETCH(int, ruleId);
    QFETCH(int, ruleInputNumber);

    setRules1();

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

void TestAimlEngine::setRules2()
{
    Lvk::Nlp::RuleList rules;

    rules << Lvk::Nlp::Rule(RULE_1_ID,
                            QStringList() << RULE_1_INPUT_1 << RULE_1_INPUT_2 << RULE_1_INPUT_3,
                            QStringList() << RULE_1_OUTPUT_1 << RULE_1_OUTPUT_2 << RULE_1_OUTPUT_3);

    m_engine->setRules(rules);
}

//--------------------------------------------------------------------------------------------------

void TestAimlEngine::testMatchWithRandomOutput_data()
{
    QTest::addColumn<QString>("userInput");
    QTest::addColumn<QStringList>("expectedOutput");

    QStringList RULE_1_OUTPUT_LIST(QStringList() << RULE_1_OUTPUT_1 << RULE_1_OUTPUT_2
                                                 << RULE_1_OUTPUT_3);

    QTest::newRow("AIML 1") << USER_INPUT_1a << RULE_1_OUTPUT_LIST;
    QTest::newRow("AIML 2") << USER_INPUT_1b << RULE_1_OUTPUT_LIST;
    QTest::newRow("AIML 3") << USER_INPUT_1c << RULE_1_OUTPUT_LIST;
    QTest::newRow("AIML 4") << USER_INPUT_2  << RULE_1_OUTPUT_LIST;
    QTest::newRow("AIML 5") << USER_INPUT_5  << RULE_1_OUTPUT_LIST;
    QTest::newRow("AIML 6") << USER_INPUT_6  << RULE_1_OUTPUT_LIST;
}

//--------------------------------------------------------------------------------------------------

void TestAimlEngine::testMatchWithRandomOutput()
{
    QFETCH(QString, userInput);
    QFETCH(QStringList, expectedOutput);

    QHash<QString, int> outputUseCount;

    setRules2();

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

void TestAimlEngine::setRules3()
{
    Lvk::Nlp::RuleList rules;

    rules << Lvk::Nlp::Rule(RULE_1_ID,
                            QStringList() << RULE_1_INPUT_1 << RULE_1_INPUT_2 << RULE_1_INPUT_3,
                            QStringList() << RULE_1_OUTPUT_1 << RULE_1_OUTPUT_2 << RULE_1_OUTPUT_3);

    rules << Lvk::Nlp::Rule(RULE_6_ID,
                            QStringList() << QString::fromUtf8(RULE_6_INPUT_1)
                                          << QString::fromUtf8(RULE_6_INPUT_2),
                            QStringList() << QString::fromUtf8(RULE_6_OUTPUT_1));

    m_engineWithDefSanitizer->setRules(rules);
}

//--------------------------------------------------------------------------------------------------

void TestAimlEngine::testMatchWithDefaultSanitizer_data()
{
    QTest::addColumn<QString>("userInput");
    QTest::addColumn<QString>("expectedOutput");
    QTest::addColumn<int>("ruleId");
    QTest::addColumn<int>("ruleInputNumber");

    QTest::newRow("AIML 1 w/DefSanitizer") << QString::fromUtf8(USER_INPUT_9a)
                                           << QString::fromUtf8(RULE_6_OUTPUT_1)
                                           << RULE_6_ID << 0;
    QTest::newRow("AIML 2 w/DefSanitizer") << QString::fromUtf8(USER_INPUT_9b)
                                           << QString::fromUtf8(RULE_6_OUTPUT_1)
                                           << RULE_6_ID << 0;
    QTest::newRow("AIML 3 w/DefSanitizer") << QString::fromUtf8(USER_INPUT_9c)
                                           << QString::fromUtf8(RULE_6_OUTPUT_1)
                                           << RULE_6_ID << 0;
    QTest::newRow("AIML 4 w/DefSanitizer") << QString::fromUtf8(USER_INPUT_9d)
                                           << QString::fromUtf8(RULE_6_OUTPUT_1)
                                           << RULE_6_ID << 1;
}

//--------------------------------------------------------------------------------------------------

void TestAimlEngine::testMatchWithDefaultSanitizer()
{
    QFETCH(QString, userInput);
    QFETCH(QString, expectedOutput);
    QFETCH(int, ruleId);
    QFETCH(int, ruleInputNumber);

    setRules3();

    Lvk::Nlp::Engine::MatchList matches;

    QString output = m_engineWithDefSanitizer->getResponse(userInput, matches);

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

void TestAimlEngine::setRules4()
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

    m_engine->setRules(rules);
}

//--------------------------------------------------------------------------------------------------

void TestAimlEngine::testMatchWithTarget_data()
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

void TestAimlEngine::testMatchWithTarget()
{
    testMatch(&TestAimlEngine::setRules4);
}

//--------------------------------------------------------------------------------------------------

void TestAimlEngine::setRules5()
{
    Lvk::Nlp::RuleList rules;

    rules << Lvk::Nlp::Rule(RULE_11_ID,
                            QStringList() << RULE_11_INPUT_1,
                            QStringList() << RULE_11_OUTPUT_1,
                            QStringList());

    rules << Lvk::Nlp::Rule(RULE_13_ID,
                            QStringList() << RULE_13_INPUT_1,
                            QStringList() << RULE_13_OUTPUT_1,
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

    m_engine->setRules(rules);
}

//--------------------------------------------------------------------------------------------------

void TestAimlEngine::testMatchPriority_data()
{
    QTest::addColumn<QString>("targetUser");
    QTest::addColumn<QString>("userInput");
    QTest::addColumn<QString>("expectedOutput");
    QTest::addColumn<int>("ruleId");
    QTest::addColumn<int>("ruleInputNumber");

    // 1. Prefer rules with target over rules without target
    // 2. Prefer rules without * over ruels with *

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

void TestAimlEngine::testMatchPriority()
{
    testMatch(&TestAimlEngine::setRules5);
}

//--------------------------------------------------------------------------------------------------

void TestAimlEngine::setRules6()
{
    Lvk::Nlp::RuleList rules;

    // TOPIC 1 --------------------

    QString topicSoccer = "soccer";

    rules << Lvk::Nlp::Rule(RULE_7_ID,
                            QStringList() << RULE_7_INPUT_1,
                            QStringList() << RULE_7_OUTPUT_1);

    rules[0].setTopic(topicSoccer);

    rules << Lvk::Nlp::Rule(RULE_11_ID,
                            QStringList() << RULE_11_INPUT_1,
                            QStringList() << RULE_11_OUTPUT_1);

    rules[1].setTopic(topicSoccer);


    // TOPIC 2 --------------------

    QString topicCars = "\"cars\"";

    rules << Lvk::Nlp::Rule(RULE_12_ID,
                            QStringList() << RULE_12_INPUT_1,
                            QStringList() << RULE_12_OUTPUT_1);

    rules[2].setTopic(topicCars);

    rules << Lvk::Nlp::Rule(RULE_3_ID,
                            QStringList() << RULE_3_INPUT_1 << RULE_3_INPUT_2 << RULE_3_INPUT_3,
                            QStringList() << RULE_3_OUTPUT_1);

    rules[3].setTopic(topicCars);


    m_engine->setRules(rules);
}

//--------------------------------------------------------------------------------------------------

void TestAimlEngine::testMatchWithTopic()
{
    setRules6();

    Lvk::Nlp::Engine::MatchList matches;
    QString response;

    // Verify that we prefer rules with the current topic

    for (int i = 0; i < 2; ++i) {
        response = m_engine->getResponse(USER_INPUT_8c, matches);
        QCOMPARE(response, QString(RULE_7_OUTPUT_1));
        QCOMPARE(matches.size(), 1);
        QCOMPARE(matches[0].first, (Lvk::Nlp::RuleId)RULE_7_ID);

        response = m_engine->getResponse(USER_INPUT_3, matches);
        QCOMPARE(response, QString(RULE_11_OUTPUT_1));
        QCOMPARE(matches.size(), 1);
        QCOMPARE(matches[0].first, (Lvk::Nlp::RuleId)RULE_11_ID);

        response = m_engine->getResponse(USER_INPUT_7b, matches);
        QCOMPARE(response, QString(RULE_3_OUTPUT_1));
        QCOMPARE(matches.size(), 1);
        QCOMPARE(matches[0].first, (Lvk::Nlp::RuleId)RULE_3_ID);

        response = m_engine->getResponse(USER_INPUT_3, matches);
        QCOMPARE(response, QString(RULE_12_OUTPUT_1));
        QCOMPARE(matches.size(), 1);
        QCOMPARE(matches[0].first, (Lvk::Nlp::RuleId)RULE_12_ID);
    }
}

//--------------------------------------------------------------------------------------------------
// Test entry point
//--------------------------------------------------------------------------------------------------

QTEST_MAIN(TestAimlEngine)

#include "testaimlengine.moc"
