#include <QtCore/QString>
#include <QtTest/QtTest>

#include "simpleaimlengine.h"

//--------------------------------------------------------------------------------------------------
// TestSimpleAimlEngine declaration
//--------------------------------------------------------------------------------------------------

class TestSimpleAimlEngine : public QObject
{
    Q_OBJECT

public:
    TestSimpleAimlEngine();
    ~TestSimpleAimlEngine();

private Q_SLOTS:
    void testConditionals_data();
    void testConditionals();

private:
    Lvk::Nlp::SimpleAimlEngine *m_engine;
};

//--------------------------------------------------------------------------------------------------
// TestSimpleAimlEngine definition
//--------------------------------------------------------------------------------------------------

#define RULE_1_ID                           1
#define RULE_1_INPUT_1                      "I was in [place]"
#define RULE_1_OUTPUT_1                     "Got It! You were in [place]"

#define RULE_2_ID                           2
#define RULE_2_INPUT_1                      "Do you like [VarName]"
#define RULE_2_OUTPUT_1                     "{if [VarName] = football} Yes, I like [VarName] {else} No, I don't"

#define USER_INPUT_1                        "I was in cordoba"
#define USER_OUTPUT_1                       "Got It! You were in cordoba"

#define USER_INPUT_2a                       "Do you like football?"
#define USER_INPUT_2b                       "Do you like tennis?"
#define USER_OUTPUT_2a                      "Yes, I like football"
#define USER_OUTPUT_2b                      "No, I don't"

#define RULE_3_ID                           3
#define RULE_3_INPUT_1                      "Do you remember where I was?"
#define RULE_3_OUTPUT_1                     "Sure, you were in [place]"

#define USER_INPUT_3                        "Do you remember where I was"
#define USER_OUTPUT_3                       "Sure, you were in cordoba"


TestSimpleAimlEngine::TestSimpleAimlEngine()
    : m_engine(new Lvk::Nlp::SimpleAimlEngine())
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

    m_engine->setRules(rules);
}

//--------------------------------------------------------------------------------------------------

TestSimpleAimlEngine::~TestSimpleAimlEngine()
{
    delete m_engine;
}

//--------------------------------------------------------------------------------------------------

void TestSimpleAimlEngine::testConditionals_data()
{
    QTest::addColumn<QString>("userInput");
    QTest::addColumn<QString>("expectedOutput");
    QTest::addColumn<int>("ruleId");
    QTest::addColumn<int>("ruleInputNumber");

    QTest::newRow("SimpleAIML Var  1")  << USER_INPUT_1  << USER_OUTPUT_1  << RULE_1_ID << 0;
    QTest::newRow("SimpleAIML Cond 1")  << USER_INPUT_2a << USER_OUTPUT_2a << RULE_2_ID << 0;
    QTest::newRow("SimpleAIML Cond 2")  << USER_INPUT_2b << USER_OUTPUT_2b << RULE_2_ID << 0;
    QTest::newRow("SimpleAIML Var  2")  << USER_INPUT_3  << USER_OUTPUT_3  << RULE_3_ID << 0;

}

//--------------------------------------------------------------------------------------------------

void TestSimpleAimlEngine::testConditionals()
{
    QFETCH(QString, userInput);
    QFETCH(QString, expectedOutput);
    QFETCH(int, ruleId);
    QFETCH(int, ruleInputNumber);

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

QTEST_APPLESS_MAIN(TestSimpleAimlEngine)

#include "simpleaimlengineunittest.moc"
