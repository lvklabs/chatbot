#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QFile>
#include <QDir>

#include "stats/statsmanager.h"
#include "stats/metric.h"
#include "stats/securestatsfile.h"

#define CHATBOT_ID_1     "bee2c509-3380-4954-9e2e-a39985ed25b1"
#define CHATBOT_ID_2     "bee2c509-3380-4954-9e2e-a39985ed25b2"

#define STAT_DIR         "./stats/"
#define STAT_EXT         ".stat"

#define STAT_FILENAME_1  STAT_DIR CHATBOT_ID_1 STAT_EXT
#define STAT_FILENAME_2  STAT_DIR CHATBOT_ID_2 STAT_EXT

using namespace Lvk;

//--------------------------------------------------------------------------------------------------
// StatsManagerUnitTest
//--------------------------------------------------------------------------------------------------

class StatsManagerUnitTest : public QObject
{
    Q_OBJECT

public:
    StatsManagerUnitTest() {}

private:

    void resetManager()
    {
        if (Stats::StatsManager::m_manager) {
            Stats::StatsManager::manager()->setChatbotId("");
        }
        delete Stats::StatsManager::m_manager;
        Stats::StatsManager::m_manager = new Stats::StatsManager();
    }

    Stats::StatsManager* manager()
    {
        return Stats::StatsManager::manager();
    }


private Q_SLOTS:
    void initTestCase();
    void init();
    void cleanupTestCase();
    void testFileCreationAndClear();
    void testSetStatAndIntervals();
};

//--------------------------------------------------------------------------------------------------

void StatsManagerUnitTest::initTestCase()
{
    QVERIFY(QDir().mkpath(STAT_DIR));
}

//--------------------------------------------------------------------------------------------------

void StatsManagerUnitTest::init()
{
    resetManager();

    if (QFile::exists(STAT_FILENAME_1)) {
        QVERIFY(QFile::remove(STAT_FILENAME_1));
    }
    if (QFile::exists(STAT_FILENAME_2)) {
        QVERIFY(QFile::remove(STAT_FILENAME_2));
    }
}

//--------------------------------------------------------------------------------------------------

void StatsManagerUnitTest::cleanupTestCase()
{
    QFile::remove(STAT_FILENAME_1);
    QFile::remove(STAT_FILENAME_2);
}

//--------------------------------------------------------------------------------------------------

void StatsManagerUnitTest::testFileCreationAndClear()
{
    if (QFile::exists(STAT_FILENAME_1)) {
        QVERIFY(QFile::remove(STAT_FILENAME_1));
    }

    manager()->setChatbotId(CHATBOT_ID_1);
    manager()->m_statsFile->setMetric(Stats::HistoryChatbotDiffLines, 1);
    manager()->setChatbotId(CHATBOT_ID_2);
    manager()->m_statsFile->setMetric(Stats::HistoryChatbotDiffLines, 2);

    resetManager();

    QVERIFY(QFile::exists(STAT_FILENAME_1));
    QVERIFY(QFile::exists(STAT_FILENAME_2));

    manager()->setChatbotId(CHATBOT_ID_1);
    manager()->clear();
    QVERIFY(!QFile::exists(STAT_FILENAME_1));

    manager()->setChatbotId(CHATBOT_ID_2);
    manager()->clear();
    QVERIFY(!QFile::exists(STAT_FILENAME_2));
}

//--------------------------------------------------------------------------------------------------

void StatsManagerUnitTest::testSetStatAndIntervals()
{
    const Stats::Metric m1 = Stats::LexiconSize;
    const Stats::Metric m2 = Stats::HistoryChatbotDiffLines;
    const Stats::Metric m3 = Stats::HistoryScoreContacts;
    const Stats::Metric m4 = Stats::ConnectionTime; // cumulative

    const unsigned value1  = 10;
    const unsigned value2  = 20;
    const unsigned value3  = 3000;
    const unsigned value4a = 60;
    const unsigned value4b = 70;

    const int ITERATIONS = 10;

    QVariant v;

    for (int i = 0; i < ITERATIONS; ++i) {
        manager()->setChatbotId((i % 2 == 0) ? CHATBOT_ID_1 : CHATBOT_ID_2);

        if (i == 0 || i == 1) {
            manager()->metric(m1, v);
            QVERIFY(v.toUInt() == 0);
            manager()->metric(m2, v);
            QVERIFY(v.toUInt() == 0);
            manager()->metric(m3, v);
            QVERIFY(v.toUInt() == 0);
            manager()->metric(m4, v);
            QVERIFY(v.toUInt() == 0);
        } else {
            manager()->metric(m1, v);
            QVERIFY(v.toUInt() == value1 + i - 2);
            manager()->metric(m2, v);
            QVERIFY(v.toUInt() == value2 + i - 2);
            manager()->metric(m3, v);
            QVERIFY(v.toUInt() == value3 + i - 2);
            manager()->metric(m4, v);
            QVERIFY(v.toUInt() == (value4a + value4b)*(i/2));
        }

        manager()->m_statsFile->setMetric(m1, QVariant(value1 + i));
        manager()->m_statsFile->setMetric(m2, QVariant(value2 + i));
        manager()->m_statsFile->setMetric(m3, QVariant(value2 + i));
        manager()->m_statsFile->setMetric(m3, QVariant(value3 + i));
        manager()->m_statsFile->setMetric(m4, QVariant(value4a));
        manager()->m_statsFile->setMetric(m4, QVariant(value4b));

        manager()->metric(m1, v);
        QVERIFY(v.toUInt() == value1 + i);
        manager()->metric(m2, v);
        QVERIFY(v.toUInt() == value2 + i);
        manager()->metric(m3, v);
        QVERIFY(v.toUInt() == value3 + i);
        manager()->metric(m4, v);
        QVERIFY(v.toUInt() == (value4a + value4b)*((i+2)/2));
    }

    // Intervals ///////////////////////////////////////////////////////////////////////////////////

    const unsigned m1_fvalue1  = value1 + ITERATIONS - 2;
    const unsigned m1_fvalue2  = value2 + ITERATIONS - 2;
    const unsigned m1_fvalue3  = value3 + ITERATIONS - 2;
    const unsigned m1_fvalue4  = (value4a + value4b)*(ITERATIONS/2);

    const unsigned m2_fvalue1  = value1 + ITERATIONS - 1;
    const unsigned m2_fvalue2  = value2 + ITERATIONS - 1;
    const unsigned m2_fvalue3  = value3 + ITERATIONS - 1;
    const unsigned m2_fvalue4  = (value4a + value4b)*(ITERATIONS/2);

    resetManager();

    manager()->setChatbotId(CHATBOT_ID_1);

    QVERIFY(manager()->metric(m1).toUInt() == m1_fvalue1);
    QVERIFY(manager()->metric(m2).toUInt() == m1_fvalue2);
    QVERIFY(manager()->metric(m3).toUInt() == m1_fvalue3);
    QVERIFY(manager()->metric(m4).toUInt() == m1_fvalue4);

    manager()->setChatbotId(CHATBOT_ID_2);

    QVERIFY(manager()->metric(m1).toUInt() == m2_fvalue1);
    QVERIFY(manager()->metric(m2).toUInt() == m2_fvalue2);
    QVERIFY(manager()->metric(m3).toUInt() == m2_fvalue3);
    QVERIFY(manager()->metric(m4).toUInt() == m2_fvalue4);

    manager()->m_statsFile->newInterval();

    QVERIFY(manager()->metric(m1).toUInt() == 0);
    QVERIFY(manager()->metric(m2).toUInt() == 0);
    QVERIFY(manager()->metric(m3).toUInt() == 0);
    QVERIFY(manager()->metric(m4).toUInt() == 0);

    manager()->setChatbotId(CHATBOT_ID_1);

    QVERIFY(manager()->metric(m1).toUInt() == m1_fvalue1);
    QVERIFY(manager()->metric(m2).toUInt() == m1_fvalue2);
    QVERIFY(manager()->metric(m3).toUInt() == m1_fvalue3);
    QVERIFY(manager()->metric(m4).toUInt() == m1_fvalue4);

    manager()->m_statsFile->newInterval();

    QVERIFY(manager()->metric(m1).toUInt() == 0);
    QVERIFY(manager()->metric(m2).toUInt() == 0);
    QVERIFY(manager()->metric(m3).toUInt() == 0);
    QVERIFY(manager()->metric(m4).toUInt() == 0);

    const unsigned m1_fvalue1_i2 = 1000000;
    const unsigned m1_fvalue2_i2 = 2000000;
    const unsigned m1_fvalue3_i2 = 3000000;
    const unsigned m1_fvalue4_i2 = 4000000;

    manager()->m_statsFile->setMetric(m1, QVariant(m1_fvalue1_i2));
    manager()->m_statsFile->setMetric(m2, QVariant(m1_fvalue2_i2));
    manager()->m_statsFile->setMetric(m3, QVariant(m1_fvalue3_i2));
    manager()->m_statsFile->setMetric(m4, QVariant(m1_fvalue4_i2));

    manager()->m_statsFile->newInterval();

    const unsigned m1_fvalue1_i3 = 1000001;
    const unsigned m1_fvalue2_i3 = 2000001;
    const unsigned m1_fvalue3_i3 = 3000001;
    const unsigned m1_fvalue4_i3 = 4000001;

    manager()->m_statsFile->setMetric(m1, QVariant(m1_fvalue1_i3));
    manager()->m_statsFile->setMetric(m2, QVariant(m1_fvalue2_i3));
    manager()->m_statsFile->setMetric(m3, QVariant(m1_fvalue3_i3));
    manager()->m_statsFile->setMetric(m4, QVariant(m1_fvalue4_i3));
}

//--------------------------------------------------------------------------------------------------

QTEST_APPLESS_MAIN(StatsManagerUnitTest)

#include "statsmanagerunittest.moc"
