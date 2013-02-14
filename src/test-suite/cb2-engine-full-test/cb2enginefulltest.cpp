#include <QtCore/QString>
#include <QtTest/QtTest>

#include "nlp-engine/cb2engine.h"
#include "nlp-engine/lemmatizerfactory.h"

#include "ruledef.h"

using namespace Lvk;

//--------------------------------------------------------------------------------------------------
// Cb2EnginefullTest
//--------------------------------------------------------------------------------------------------

class Cb2EnginefullTest : public QObject
{
    Q_OBJECT

public:
    Cb2EnginefullTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase1_data();

private:
    Nlp::Cb2Engine *m_engine;
};

//--------------------------------------------------------------------------------------------------

Cb2EnginefullTest::Cb2EnginefullTest()
    : m_engine(new Nlp::Cb2Engine())
{
    m_engine->setLemmatizer(Nlp::LemmatizerFactory().createLemmatizer());

    setRules1(m_engine);
}

//--------------------------------------------------------------------------------------------------

void Cb2EnginefullTest::initTestCase()
{
}

//--------------------------------------------------------------------------------------------------

void Cb2EnginefullTest::cleanupTestCase()
{
}

//--------------------------------------------------------------------------------------------------

void Cb2EnginefullTest::testCase1()
{
    QFETCH(QString, input);
    QFETCH(QString, expOutput);
    QFETCH(int, ruleId);
    QFETCH(int, inputIdx);

    Nlp::Engine::MatchList matches;
    QString output = m_engine->getResponse(input, matches);

    if (!expOutput.isNull()) {
        QCOMPARE(output, expOutput);
        QVERIFY(matches.size() > 0);
        QCOMPARE(ruleId, static_cast<int>(matches[0].first));
        QCOMPARE(inputIdx, matches[0].second);
    } else {
        QVERIFY(output.isEmpty());
        QVERIFY(matches.isEmpty());
    }
}

//--------------------------------------------------------------------------------------------------

void Cb2EnginefullTest::testCase1_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("expOutput");
    QTest::addColumn<int>("ruleId");
    QTest::addColumn<int>("inputIdx");

    QTest::newRow(" 0") << "Hola"                << RULE_1_OUTPUT_1 << RULE_1_ID << 0;
    QTest::newRow(" 1") << "HOLAAA!"             << RULE_1_OUTPUT_1 << RULE_1_ID << 0;
    QTest::newRow(" 2") << "hooola!!!"           << RULE_1_OUTPUT_1 << RULE_1_ID << 0;
    QTest::newRow(" 3") << "Hola como andas?"    << RULE_1_OUTPUT_1 << RULE_1_ID << 1;
    QTest::newRow(" 3") << "Hola, que hacessss?" << RULE_1_OUTPUT_1 << RULE_1_ID << 1;
    QTest::newRow(" 4") << "chau"                << RULE_2_OUTPUT_1 << RULE_2_ID << 0;
    QTest::newRow(" 5") << "CHAUUU CHE..."       << RULE_2_OUTPUT_1 << RULE_2_ID << 0;
    QTest::newRow(" 6") << "bueno chau"          << RULE_2_OUTPUT_1 << RULE_2_ID << 1;
    QTest::newRow(" 7") << "hey hola"            << QString()       << 0         << 1;
    QTest::newRow(" 8") << "Bye"                 << QString()       << 0         << 1;
    QTest::newRow(" 9") << "jugar futbol"        << RULE_3_OUTPUT_1 << RULE_3_ID << 0;
    QTest::newRow("10") << "yo jugaba futbol"    << RULE_3_OUTPUT_1 << RULE_3_ID << 0;
    QTest::newRow("11") << QString::fromUtf8("Yo jugu\xc3\xa9 al futbol mucho tiempo")  << RULE_3_OUTPUT_1 << RULE_3_ID << 0;
}

//--------------------------------------------------------------------------------------------------

QTEST_APPLESS_MAIN(Cb2EnginefullTest)

#include "cb2enginefulltest.moc"
