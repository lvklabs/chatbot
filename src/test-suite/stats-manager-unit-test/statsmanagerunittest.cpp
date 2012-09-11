#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QFile>
#include <QDir>

#include "stats/statsmanager.h"
#include "stats/id.h"

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
    manager()->setStat(Stats::HistoryChatbotDiffLines, 1);
    manager()->setChatbotId(CHATBOT_ID_2);
    manager()->setStat(Stats::HistoryChatbotDiffLines, 2);

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
    const Stats::Id id1 = Stats::LexiconSize;
    const Stats::Id id2 = Stats::HistoryChatbotDiffLines;
    const Stats::Id id3 = Stats::HistoryScoreContacts;
    const Stats::Id id4 = Stats::ConnectionTime; // cumulative

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
            manager()->stat(id1, v);
            QVERIFY(v.toUInt() == 0);
            manager()->stat(id2, v);
            QVERIFY(v.toUInt() == 0);
            manager()->stat(id3, v);
            QVERIFY(v.toUInt() == 0);
            manager()->stat(id4, v);
            QVERIFY(v.toUInt() == 0);
        } else {
            manager()->stat(id1, v);
            QVERIFY(v.toUInt() == value1 + i - 2);
            manager()->stat(id2, v);
            QVERIFY(v.toUInt() == value2 + i - 2);
            manager()->stat(id3, v);
            QVERIFY(v.toUInt() == value3 + i - 2);
            manager()->stat(id4, v);
            QVERIFY(v.toUInt() == (value4a + value4b)*(i/2));
        }

        manager()->setStat(id1, QVariant(value1 + i));
        manager()->setStat(id2, QVariant(value2 + i));
        manager()->setStat(id3, QVariant(value2 + i));
        manager()->setStat(id3, QVariant(value3 + i));
        manager()->setStat(id4, QVariant(value4a));
        manager()->setStat(id4, QVariant(value4b));

        manager()->stat(id1, v);
        QVERIFY(v.toUInt() == value1 + i);
        manager()->stat(id2, v);
        QVERIFY(v.toUInt() == value2 + i);
        manager()->stat(id3, v);
        QVERIFY(v.toUInt() == value3 + i);
        manager()->stat(id4, v);
        QVERIFY(v.toUInt() == (value4a + value4b)*((i+2)/2));
    }

    // Intervals ///////////////////////////////////////////////////////////////////////////////////

    const unsigned id1_fvalue1  = value1 + ITERATIONS - 2;
    const unsigned id1_fvalue2  = value2 + ITERATIONS - 2;
    const unsigned id1_fvalue3  = value3 + ITERATIONS - 2;
    const unsigned id1_fvalue4  = (value4a + value4b)*(ITERATIONS/2);

    const unsigned id2_fvalue1  = value1 + ITERATIONS - 1;
    const unsigned id2_fvalue2  = value2 + ITERATIONS - 1;
    const unsigned id2_fvalue3  = value3 + ITERATIONS - 1;
    const unsigned id2_fvalue4  = (value4a + value4b)*(ITERATIONS/2);

    resetManager();

    manager()->setChatbotId(CHATBOT_ID_1);

    QVERIFY(manager()->stat(id1).toUInt() == id1_fvalue1);
    QVERIFY(manager()->stat(id2).toUInt() == id1_fvalue2);
    QVERIFY(manager()->stat(id3).toUInt() == id1_fvalue3);
    QVERIFY(manager()->stat(id4).toUInt() == id1_fvalue4);

    manager()->setChatbotId(CHATBOT_ID_2);

    QVERIFY(manager()->stat(id1).toUInt() == id2_fvalue1);
    QVERIFY(manager()->stat(id2).toUInt() == id2_fvalue2);
    QVERIFY(manager()->stat(id3).toUInt() == id2_fvalue3);
    QVERIFY(manager()->stat(id4).toUInt() == id2_fvalue4);

    manager()->newInterval();

    QVERIFY(manager()->stat(id1).toUInt() == 0);
    QVERIFY(manager()->stat(id2).toUInt() == 0);
    QVERIFY(manager()->stat(id3).toUInt() == 0);
    QVERIFY(manager()->stat(id4).toUInt() == 0);

    manager()->setChatbotId(CHATBOT_ID_1);

    QVERIFY(manager()->stat(id1).toUInt() == id1_fvalue1);
    QVERIFY(manager()->stat(id2).toUInt() == id1_fvalue2);
    QVERIFY(manager()->stat(id3).toUInt() == id1_fvalue3);
    QVERIFY(manager()->stat(id4).toUInt() == id1_fvalue4);

    manager()->newInterval();

    QVERIFY(manager()->stat(id1).toUInt() == 0);
    QVERIFY(manager()->stat(id2).toUInt() == 0);
    QVERIFY(manager()->stat(id3).toUInt() == 0);
    QVERIFY(manager()->stat(id4).toUInt() == 0);

    const unsigned id1_fvalue1_i2 = 1000000;
    const unsigned id1_fvalue2_i2 = 2000000;
    const unsigned id1_fvalue3_i2 = 3000000;
    const unsigned id1_fvalue4_i2 = 4000000;

    manager()->setStat(id1, QVariant(id1_fvalue1_i2));
    manager()->setStat(id2, QVariant(id1_fvalue2_i2));
    manager()->setStat(id3, QVariant(id1_fvalue3_i2));
    manager()->setStat(id4, QVariant(id1_fvalue4_i2));

    manager()->newInterval();

    const unsigned id1_fvalue1_i3 = 1000001;
    const unsigned id1_fvalue2_i3 = 2000001;
    const unsigned id1_fvalue3_i3 = 3000001;
    const unsigned id1_fvalue4_i3 = 4000001;

    manager()->setStat(id1, QVariant(id1_fvalue1_i3));
    manager()->setStat(id2, QVariant(id1_fvalue2_i3));
    manager()->setStat(id3, QVariant(id1_fvalue3_i3));
    manager()->setStat(id4, QVariant(id1_fvalue4_i3));

    // History /////////////////////////////////////////////////////////////////////////////////////

    Stats::History h;

    resetManager();

    manager()->setChatbotId(CHATBOT_ID_1);

    manager()->history(id1, h);

    QVERIFY(h.size() == 3);
    QVERIFY(h[0].second.toUInt() == id1_fvalue1);
    QVERIFY(h[1].second.toUInt() == id1_fvalue1_i2);
    QVERIFY(h[2].second.toUInt() == id1_fvalue1_i3);

    manager()->history(id2, h);

    QVERIFY(h.size() == 3);
    QVERIFY(h[0].second.toUInt() == id1_fvalue2);
    QVERIFY(h[1].second.toUInt() == id1_fvalue2_i2);
    QVERIFY(h[2].second.toUInt() == id1_fvalue2_i3);

    manager()->history(id4, h);

    QVERIFY(h.size() == 3);
    QVERIFY(h[0].second.toUInt() == id1_fvalue4);
    QVERIFY(h[1].second.toUInt() == id1_fvalue4_i2);
    QVERIFY(h[2].second.toUInt() == id1_fvalue4_i3);

    manager()->setChatbotId(CHATBOT_ID_2);

    manager()->history(id1, h);

    QVERIFY(h.size() == 1);
    QVERIFY(h[0].second.toUInt() == id2_fvalue1);
}

//--------------------------------------------------------------------------------------------------

QTEST_APPLESS_MAIN(StatsManagerUnitTest)

#include "statsmanagerunittest.moc"
