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
#define RULE_1_INPUT_1                      "Do you like [VarName]"
#define RULE_1_OUTPUT_1                     "{if [VarName] = football} Yes, I like [VarName] {else} No, I don't"
#define USER_INPUT_1a                       "Do you like football?"
#define USER_INPUT_1b                       "Do you like tennis?"
#define USER_OUTPUT_1a                      "Yes, I Like football"
#define USER_OUTPUT_1b                      "No, I don't"

TestSimpleAimlEngine::TestSimpleAimlEngine()
    : m_engine(new Lvk::Nlp::SimpleAimlEngine())
{
    Lvk::Nlp::RuleList rules;

    rules << Lvk::Nlp::Rule(RULE_1_ID,
                            QStringList() << RULE_1_INPUT_1,
                            QStringList() << RULE_1_OUTPUT_1);

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

    QTest::newRow("SimpleAIML Cond 1")  << USER_INPUT_1a << USER_OUTPUT_1a << RULE_1_ID << 0;
    QTest::newRow("SimpleAIML Cond 2")  << USER_INPUT_1b << USER_OUTPUT_1b << RULE_1_ID << 0;
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
