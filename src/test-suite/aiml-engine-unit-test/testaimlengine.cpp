#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QHash>
#include <QRegExp>
#include <QIODevice>

#include <iostream>

#include "aimlengine.h"
#include "ProgramQ/aimlparser.h"
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

    void testAimlParserRegression();

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
#define RULE_3_INPUT_2                      "cars _"
#define RULE_3_INPUT_3                      "_ cars"
#define RULE_3_INPUT_4                      "_ cars *"
#define RULE_3_OUTPUT_1                     "I love cars"

#define RULE_4_ID                           4
#define RULE_4_INPUT_1                      "Do you like *"
#define RULE_4_OUTPUT_1                     "<srai><star/></srai>"

#define RULE_5_ID                           5
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

void TestAimlEngine::testMatchWithSingleOutput_data()
{
    QTest::addColumn<QString>("userInput");
    QTest::addColumn<QString>("expectedOutput");
    QTest::addColumn<int>("ruleId");
    QTest::addColumn<int>("ruleInputNumber");

    QTest::newRow("AIML 1")  << USER_INPUT_1a << RULE_1_OUTPUT_1 << 1 << 0;
    QTest::newRow("AIML 2")  << USER_INPUT_1b << RULE_1_OUTPUT_1 << 1 << 0;
    QTest::newRow("AIML 3")  << USER_INPUT_1c << RULE_1_OUTPUT_1 << 1 << 0;
    QTest::newRow("AIML 4")  << USER_INPUT_2  << RULE_1_OUTPUT_1 << 1 << 1;
    QTest::newRow("AIML 5")  << USER_INPUT_4a << RULE_2_OUTPUT_1 << 2 << 0;
    QTest::newRow("AIML 6")  << USER_INPUT_4b << RULE_2_OUTPUT_1 << 2 << 0;
    QTest::newRow("AIML 7")  << USER_INPUT_5  << RULE_1_OUTPUT_1 << 1 << 2;
    QTest::newRow("AIML 8")  << USER_INPUT_6  << RULE_1_OUTPUT_1 << 1 << 2;
    QTest::newRow("AIML 9")  << USER_INPUT_7a << RULE_3_OUTPUT_1 << 3 << 0;
    QTest::newRow("AIML 10") << USER_INPUT_7b << RULE_3_OUTPUT_1 << 3 << 1;
    QTest::newRow("AIML 11") << USER_INPUT_7c << RULE_3_OUTPUT_1 << 3 << 2;
    QTest::newRow("AIML 12") << USER_INPUT_7d << RULE_3_OUTPUT_1 << 3 << 3;
    QTest::newRow("AIML 13") << USER_INPUT_8a << RULE_5_OUTPUT_1 << 5 << 0;
    QTest::newRow("AIML 14") << USER_INPUT_8b << QString()       << 0 << 0;
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
//        QCOMPARE(matches[0].first, static_cast<Lvk::Nlp::RuleId>(ruleId));
//        QCOMPARE(matches[0].second, ruleInputNumber);
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
