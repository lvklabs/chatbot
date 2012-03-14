#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QHash>
#include <QRegExp>

#include "aimlengine.h"
#include "nlprule.h"

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

    void testMatchWithSingleOutput_data();
    void testMatchWithSingleOutput();

    void testMatchWithRandomOutput_data();
    void testMatchWithRandomOutput();

    void cleanupTestCase();

private:

    Lvk::Nlp::AimlEngine *m_engine;

    void setRules1();
    void setRules2();
};

TestAimlEngine::TestAimlEngine()
    : m_engine(0)
{
}

//--------------------------------------------------------------------------------------------------
// TestAimlEngine definition
//--------------------------------------------------------------------------------------------------

#define RULE_1_INPUT_1                      "Hello"
#define RULE_1_INPUT_2                      "Hi"
#define RULE_1_INPUT_3                      "Hello *"
#define RULE_1_OUTPUT_1                     "Hi!"
#define RULE_1_OUTPUT_2                     "Hello!"
#define RULE_1_OUTPUT_3                     "Hey!"

#define RULE_2_INPUT_1                      "What is your name?"
#define RULE_2_OUTPUT_1                     "R2D2"

#define RULE_3_INPUT_1                      "cars"
#define RULE_3_INPUT_2                      "cars _"
#define RULE_3_INPUT_3                      "_ cars"
#define RULE_3_INPUT_4                      "_ cars *"
#define RULE_3_OUTPUT_1                     "I love cars"

#define RULE_4_INPUT_1                      "Do you like *"
#define RULE_4_OUTPUT_1                     "<srai><star/></srai>"

#define RULE_5_INPUT_1                      "cats"
#define RULE_5_OUTPUT_1                     "I hate cats!"

#define USER_INPUT_1a                       "Hello"
#define USER_INPUT_1b                       "hello"
#define USER_INPUT_1c                       "HELLO"
#define USER_INPUT_2                        "Hi"
#define USER_INPUT_3                        "Hey there!"
#define USER_INPUT_4a                       "What is your name?"
#define USER_INPUT_4b                       "What is your name"
#define USER_INPUT_5                        "Hello there!"
#define USER_INPUT_6                        "Hello how are you!!"
#define USER_INPUT_7a                       "cars"
#define USER_INPUT_7b                       "Do you like cars?"
#define USER_INPUT_7c                       "cars are the best!"
#define USER_INPUT_7d                       "Have you seen the latest cars that BMW have launched?"
#define USER_INPUT_8a                       "Do you like cats?"
#define USER_INPUT_8b                       "Do you like robots?"


//--------------------------------------------------------------------------------------------------

void TestAimlEngine::initTestCase()
{
    m_engine = new Lvk::Nlp::AimlEngine();
}

//--------------------------------------------------------------------------------------------------

void TestAimlEngine::cleanupTestCase()
{
    delete m_engine;
    m_engine = 0;
}

//--------------------------------------------------------------------------------------------------

void TestAimlEngine::setRules1()
{
    int nextId = 0;

    Lvk::Nlp::RuleList rules;

    rules << Lvk::Nlp::Rule(nextId++,
                            QStringList() << RULE_1_INPUT_1 << RULE_1_INPUT_2 << RULE_1_INPUT_3,
                            QStringList() << RULE_1_OUTPUT_1);

    rules << Lvk::Nlp::Rule(nextId++,
                            QStringList() << RULE_2_INPUT_1,
                            QStringList() << RULE_2_OUTPUT_1);

    rules << Lvk::Nlp::Rule(nextId++,
                            QStringList() << RULE_3_INPUT_1 << RULE_3_INPUT_2 << RULE_3_INPUT_3
                                          << RULE_3_INPUT_4,
                            QStringList() << RULE_3_OUTPUT_1);

    rules << Lvk::Nlp::Rule(nextId++,
                            QStringList() << RULE_4_INPUT_1,
                            QStringList() << RULE_4_OUTPUT_1);

    rules << Lvk::Nlp::Rule(nextId++,
                            QStringList() << RULE_5_INPUT_1,
                            QStringList() << RULE_5_OUTPUT_1);

    m_engine->setRules(rules);
}

//--------------------------------------------------------------------------------------------------

void TestAimlEngine::setRules2()
{

    int nextId = 0;

    Lvk::Nlp::RuleList rules;

    rules << Lvk::Nlp::Rule(nextId++,
                            QStringList() << RULE_1_INPUT_1 << RULE_1_INPUT_2 << RULE_1_INPUT_3,
                            QStringList() << RULE_1_OUTPUT_1 << RULE_1_OUTPUT_2 << RULE_1_OUTPUT_3);

    m_engine->setRules(rules);
}

//--------------------------------------------------------------------------------------------------

void TestAimlEngine::testMatchWithSingleOutput_data()
{
    QTest::addColumn<QString>("userInput");
    QTest::addColumn<QString>("expectedOutput");

    QTest::newRow("AIML 1")  << USER_INPUT_1a << RULE_1_OUTPUT_1;
    QTest::newRow("AIML 2")  << USER_INPUT_1b << RULE_1_OUTPUT_1;
    QTest::newRow("AIML 3")  << USER_INPUT_1c << RULE_1_OUTPUT_1;
    QTest::newRow("AIML 4")  << USER_INPUT_2  << RULE_1_OUTPUT_1;
    QTest::newRow("AIML 5")  << USER_INPUT_4a << RULE_2_OUTPUT_1;
    QTest::newRow("AIML 6")  << USER_INPUT_4b << RULE_2_OUTPUT_1;
    QTest::newRow("AIML 7")  << USER_INPUT_5  << RULE_1_OUTPUT_1;
    QTest::newRow("AIML 8")  << USER_INPUT_6  << RULE_1_OUTPUT_1;
    QTest::newRow("AIML 9")  << USER_INPUT_7a << RULE_3_OUTPUT_1;
    QTest::newRow("AIML 10") << USER_INPUT_7b << RULE_3_OUTPUT_1;
    QTest::newRow("AIML 11") << USER_INPUT_7c << RULE_3_OUTPUT_1;
    QTest::newRow("AIML 12") << USER_INPUT_7d << RULE_3_OUTPUT_1;
    QTest::newRow("AIML 13") << USER_INPUT_8a << RULE_5_OUTPUT_1;
    QTest::newRow("AIML 14") << USER_INPUT_8b << QString("");
}

//--------------------------------------------------------------------------------------------------

void TestAimlEngine::testMatchWithSingleOutput()
{
    QFETCH(QString, userInput);
    QFETCH(QString, expectedOutput);

    setRules1();

    Lvk::Nlp::Engine::MatchList matches;

    QString output = m_engine->getResponse(userInput, matches);

    if (!expectedOutput.isNull()) {
        QCOMPARE(output, expectedOutput);
        QCOMPARE(matches.size(), 1);
    } else {
        QVERIFY(output.isEmpty());
        QCOMPARE(matches.size(), 0);
    }
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
// Test entry point
//--------------------------------------------------------------------------------------------------

QTEST_MAIN(TestAimlEngine)

#include "testaimlengine.moc"
