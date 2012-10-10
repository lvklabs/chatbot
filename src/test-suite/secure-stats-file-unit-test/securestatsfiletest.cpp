#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QFile>
#include <QVariant>

#include "stats/securestatsfile.h"
#include "stats/metric.h"
#include "stats/score.h"

#define FILENAME_1 "teststatfile1.stat"


using namespace Lvk;

//--------------------------------------------------------------------------------------------------
// SecureStatsFileTest
//--------------------------------------------------------------------------------------------------

class SecureStatsFileTest : public QObject
{
    Q_OBJECT

public:
    SecureStatsFileTest() { }

private Q_SLOTS:
    void initTestCase();
    void init();
    void cleanupTestCase();
    void testIsEmptyAndClose();
    void testIsEmptySaveAndClear();
    void testSaveAndLoadScore();
    void testSaveAndLoadMetrics();
    void testNewIntervalAndHistory();

private:

    void verifyAllEmpty(const Stats::SecureStatsFile &file)
    {
        QVERIFY(file.isEmpty());
        QVERIFY(file.currentScore().isNull());
        QVERIFY(file.bestScore().isNull());
        QVERIFY(file.scoreElapsedTime() == 0);
        QVERIFY(file.contacts().isEmpty());

        Cmn::Conversation h;
        file.chatHistory(h);
        QVERIFY(h.isEmpty());
    }
};

//--------------------------------------------------------------------------------------------------

void SecureStatsFileTest::initTestCase()
{
    // Nothing to do
}

//--------------------------------------------------------------------------------------------------

void SecureStatsFileTest::init()
{
    if (QFile::exists(FILENAME_1)) {
        QVERIFY(QFile::remove(FILENAME_1));
    }
}

//--------------------------------------------------------------------------------------------------

void SecureStatsFileTest::cleanupTestCase()
{
    if (QFile::exists(FILENAME_1)) {
        QVERIFY(QFile::remove(FILENAME_1));
    }
}

//--------------------------------------------------------------------------------------------------

void SecureStatsFileTest::testIsEmptyAndClose()
{
    Stats::SecureStatsFile file(FILENAME_1);

    verifyAllEmpty(file);
    file.setMetric(Stats::RuleLexiconSize, QVariant(10));
    QVERIFY(!file.isEmpty());
    file.close();
    verifyAllEmpty(file);
    QVERIFY(!QFile::exists(FILENAME_1));
}

//--------------------------------------------------------------------------------------------------

void SecureStatsFileTest::testIsEmptySaveAndClear()
{
    Stats::SecureStatsFile file(FILENAME_1);

    verifyAllEmpty(file);
    file.setMetric(Stats::RuleLexiconSize, QVariant(10));
    QVERIFY(!file.isEmpty());
    file.save();
    QVERIFY(QFile::exists(FILENAME_1));
    file.clear();
    verifyAllEmpty(file);
    QVERIFY(!QFile::exists(FILENAME_1));
}


//--------------------------------------------------------------------------------------------------

void SecureStatsFileTest::testSaveAndLoadScore()
{

    Stats::Score cs(10, 20, 30);
    Stats::Score bs(100, 200, 300);
    int et = 32134;

    QString c1 = "John";
    QString c2 = "Jane G";
    QString c3 = "John";

    Cmn::Conversation::Entry e1(QDateTime::currentDateTime(), c1, c3, "Hi!!!", "Hi!", true, 2);
    Cmn::Conversation::Entry e2(QDateTime::currentDateTime(), c1, c3, "What's up?", "", false, 0);

    {
        Stats::SecureStatsFile file(FILENAME_1);

        verifyAllEmpty(file);

        file.setCurrentScore(cs);
        file.setBestScore(bs);
        file.setScoreElapsedTime(et);

        QVERIFY(!file.isEmpty());

        file.addContact(c1);
        file.addContact(c2);
        file.addContact(c3);

        file.appendChatEntry(e1);
        file.appendChatEntry(e2);
        file.appendChatEntry(e2);

        QVERIFY(file.currentScore() == cs);
        QVERIFY(file.bestScore() == bs);
        QVERIFY(file.scoreElapsedTime() == et);
        QVERIFY(file.contacts().size() == 2);
        QVERIFY(file.contacts().contains(c1));
        QVERIFY(file.contacts().contains(c2));

        Cmn::Conversation h;
        file.chatHistory(h);
        QVERIFY(h.size() == 3);
        QVERIFY(h.entries().at(0) == e1);
        QVERIFY(h.entries().at(1) == e2);
        QVERIFY(h.entries().at(2) == e2);

        file.save();
        QVERIFY(QFile::exists(FILENAME_1));
    }

    {
        Stats::SecureStatsFile file;

        file.load(FILENAME_1);

        QVERIFY(!file.isEmpty());

        QVERIFY(file.currentScore() == cs);
        QVERIFY(file.bestScore() == bs);
        QVERIFY(file.scoreElapsedTime() == et);
        QVERIFY(file.contacts().size() == 2);
        QVERIFY(file.contacts().contains(c1));
        QVERIFY(file.contacts().contains(c2));

        Cmn::Conversation h;
        file.chatHistory(h);
        QVERIFY(h.size() == 3);
        QVERIFY(h.entries().at(0) == e1);
        QVERIFY(h.entries().at(1) == e2);
        QVERIFY(h.entries().at(2) == e2);

        // interval 2 ////////////////////////////////

        file.newInterval();

        QVERIFY(file.currentScore() == cs);
        QVERIFY(file.bestScore() == bs);
        QVERIFY(file.scoreElapsedTime() == 0);
        QVERIFY(file.contacts().size() == 2);
        QVERIFY(file.contacts().contains(c1));
        QVERIFY(file.contacts().contains(c2));

        h.clear();
        file.chatHistory(h);
        QVERIFY(h.isEmpty());

        // interval 3 ////////////////////////////////

        file.newInterval();

        QVERIFY(file.currentScore() == cs);
        QVERIFY(file.bestScore() == bs);
        QVERIFY(file.scoreElapsedTime() == 0);
        QVERIFY(file.contacts().size() == 2);
        QVERIFY(file.contacts().contains(c1));
        QVERIFY(file.contacts().contains(c2));

        h.clear();
        file.chatHistory(h);
        QVERIFY(h.isEmpty());

        file.clear();
    }

    {
        Stats::SecureStatsFile file(FILENAME_1);

        verifyAllEmpty(file);
    }
}

//--------------------------------------------------------------------------------------------------

void SecureStatsFileTest::testSaveAndLoadMetrics()
{
    const Stats::Metric m1 = Stats::RuleLexiconSize;
    const Stats::Metric m2 = Stats::RuleWordCount;
    const Stats::Metric m3 = Stats::RulePoints;
    const Stats::Metric m4 = Stats::ConnectionTime; // cumulative

    const unsigned value1  = 10;
    const unsigned value2  = 20;
    const unsigned value3  = 3000;
    const unsigned value4a = 60;
    const unsigned value4b = 70;

    const int ITERATIONS = 10;

    QVariant v;

    for (int i = 0; i < ITERATIONS; ++i) {
        Stats::SecureStatsFile file(FILENAME_1);

        if (i == 0) {
            QVERIFY(file.isEmpty());
        } else {
            QVERIFY(!file.isEmpty());
        }

        file.setMetric(m1, QVariant(value1));
        file.setMetric(m2, QVariant(value2));
        file.setMetric(m3, QVariant(value2));
        file.setMetric(m3, QVariant(value3));
        file.setMetric(m4, QVariant(value4a));
        file.setMetric(m4, QVariant(value4b));

        QVERIFY(!file.isEmpty());

        file.metric(m1, v);
        QVERIFY(v.toUInt() == value1);
        file.metric(m2, v);
        QVERIFY(v.toUInt() == value2);
        file.metric(m3, v);
        QVERIFY(v.toUInt() == value3);
        file.metric(m4, v);
        QVERIFY(v.toUInt() == (value4a + value4b)*(i+1));

        file.save();
        QVERIFY(QFile::exists(FILENAME_1));
    }

    {
        Stats::SecureStatsFile file;

        file.load(FILENAME_1);

        QVERIFY(!file.isEmpty());

        file.metric(m1, v);
        QVERIFY(v.toUInt() == value1);
        file.metric(m2, v);
        QVERIFY(v.toUInt() == value2);
        file.metric(m3, v);
        QVERIFY(v.toUInt() == value3);
        file.metric(m4, v);
        QVERIFY(v.toUInt() == (value4a + value4b)*ITERATIONS);

        file.clear();
    }

    {
        Stats::SecureStatsFile file;

        file.load(FILENAME_1);

        verifyAllEmpty(file);
    }
}

//--------------------------------------------------------------------------------------------------

void SecureStatsFileTest::testNewIntervalAndHistory()
{
    const Stats::Metric m1 = Stats::RuleLexiconSize;
    const Stats::Metric m4 = Stats::ConnectionTime; // cumulative
    const Stats::Metric id5 = Stats::CondRuleCount;

    const unsigned value1a = 10;
    const unsigned value1b = 20;
    const unsigned value1c = 30;
    const unsigned value4a = 60;
    const unsigned value4b = 70;
    const unsigned value4c = 80;

    Stats::History h;
    QVariant v;

    {
        Stats::SecureStatsFile file;

        file.load(FILENAME_1);

        file.metricHistory(m4, h);
        QVERIFY(h.size() == 0);

        // interval 1 ////////////////////////////////

        file.setMetric(m1, QVariant(value1b));
        file.setMetric(m1, QVariant(value1a));
        file.setMetric(m4, QVariant(value4a));
        file.setMetric(m4, QVariant(value4b));

        file.metric(m1, v);
        QVERIFY(v.toUInt() == value1a);
        file.metric(m4, v);
        QVERIFY(v.toUInt() == value4a + value4b);

        // interval 2 ////////////////////////////////
        file.newInterval();

        file.metric(m1, v);
        QVERIFY(v.toUInt() == 0);
        file.metric(m4, v);
        QVERIFY(v.toUInt() == 0);

        file.setMetric(m1, QVariant(value1b));
        file.setMetric(m4, QVariant(value4b));

        file.metric(m1, v);
        QVERIFY(v.toUInt() == value1b);
        file.metric(m4, v);
        QVERIFY(v.toUInt() == value4b);

        // interval 3 ////////////////////////////////
        file.newInterval();

        file.metric(m1, v);
        QVERIFY(v.toUInt() == 0);
        file.metric(m4, v);
        QVERIFY(v.toUInt() == 0);

        file.setMetric(m1, QVariant(value1c));
        file.setMetric(m4, QVariant(value4c));

        file.metric(m1, v);
        QVERIFY(v.toUInt() == value1c);
        file.metric(m4, v);
        QVERIFY(v.toUInt() == value4c);

        file.save();
    }

    {
        Stats::SecureStatsFile file;

        file.load(FILENAME_1);

        file.metric(m1, v);
        QVERIFY(v.toUInt() == value1c);
        file.metric(m4, v);
        QVERIFY(v.toUInt() == value4c);
        file.metric(id5, v);
        QVERIFY(v.toUInt() == 0);

        file.metricHistory(m1, h);
        QVERIFY(h.size() == 3);
        QVERIFY(h[0].second.toUInt() == value1a);
        QVERIFY(h[1].second.toUInt() == value1b);
        QVERIFY(h[2].second.toUInt() == value1c);

        file.metricHistory(m4, h);
        QVERIFY(h.size() == 3);
        QVERIFY(h[0].second.toUInt() == value4a + value4b);
        QVERIFY(h[1].second.toUInt() == value4b);
        QVERIFY(h[2].second.toUInt() == value4c);
    }
}

//--------------------------------------------------------------------------------------------------

QTEST_APPLESS_MAIN(SecureStatsFileTest)

#include "securestatsfiletest.moc"
