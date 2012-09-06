#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QFile>
#include <QVariant>

#include "stats/securestatsfile.h"
#include "stats/id.h"

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
    void testSetSaveAndLoad();
    void testNewIntervalAndHistory();
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
    // Nothing to do
}

//--------------------------------------------------------------------------------------------------

void SecureStatsFileTest::testIsEmptyAndClose()
{
    Stats::SecureStatsFile file(FILENAME_1);

    QVERIFY(file.isEmpty());
    file.setStat(Stats::LexiconSize, QVariant(10));
    QVERIFY(!file.isEmpty());
    file.close();
    QVERIFY(file.isEmpty());
    QVERIFY(!QFile::exists(FILENAME_1));
}

//--------------------------------------------------------------------------------------------------

void SecureStatsFileTest::testIsEmptySaveAndClear()
{
    Stats::SecureStatsFile file(FILENAME_1);

    QVERIFY(file.isEmpty());
    file.setStat(Stats::LexiconSize, QVariant(10));
    QVERIFY(!file.isEmpty());
    file.save();
    QVERIFY(QFile::exists(FILENAME_1));
    file.clear();
    QVERIFY(file.isEmpty());
    QVERIFY(!QFile::exists(FILENAME_1));
}


//--------------------------------------------------------------------------------------------------

void SecureStatsFileTest::testSetSaveAndLoad()
{
    const Stats::Id id1 = Stats::LexiconSize;
    const Stats::Id id2 = Stats::HistoryChatbotDiffLines;
    const Stats::Id id3 = Stats::HistoryImportantContacts;
    const Stats::Id id4 = Stats::ConnectionTime; // cumulative

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

        file.setStat(id1, QVariant(value1));
        file.setStat(id2, QVariant(value2));
        file.setStat(id3, QVariant(value2));
        file.setStat(id3, QVariant(value3));
        file.setStat(id4, QVariant(value4a));
        file.setStat(id4, QVariant(value4b));

        QVERIFY(!file.isEmpty());

        file.stat(id1, v);
        QVERIFY(v.toUInt() == value1);
        file.stat(id2, v);
        QVERIFY(v.toUInt() == value2);
        file.stat(id3, v);
        QVERIFY(v.toUInt() == value3);
        file.stat(id4, v);
        QVERIFY(v.toUInt() == (value4a + value4b)*(i+1));

        file.save();
        QVERIFY(QFile::exists(FILENAME_1));
    }

    {
        Stats::SecureStatsFile file;

        file.load(FILENAME_1);

        QVERIFY(!file.isEmpty());

        file.stat(id1, v);
        QVERIFY(v.toUInt() == value1);
        file.stat(id2, v);
        QVERIFY(v.toUInt() == value2);
        file.stat(id3, v);
        QVERIFY(v.toUInt() == value3);
        file.stat(id4, v);
        QVERIFY(v.toUInt() == (value4a + value4b)*ITERATIONS);
    }
}

//--------------------------------------------------------------------------------------------------

void SecureStatsFileTest::testNewIntervalAndHistory()
{
    const Stats::Id id1 = Stats::LexiconSize;
    const Stats::Id id4 = Stats::ConnectionTime; // cumulative
    const Stats::Id id5 = Stats::HistoryTotalLines;

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

        file.history(id1, h);
        QVERIFY(h.size() == 0);
        file.history(id4, h);
        QVERIFY(h.size() == 0);

        // interval 1 ////////////////////////////////

        file.setStat(id1, QVariant(value1b));
        file.setStat(id1, QVariant(value1a));
        file.setStat(id4, QVariant(value4a));
        file.setStat(id4, QVariant(value4b));

        file.stat(id1, v);
        QVERIFY(v.toUInt() == value1a);
        file.stat(id4, v);
        QVERIFY(v.toUInt() == value4a + value4b);

        // interval 2 ////////////////////////////////
        file.newInterval();

        file.stat(id1, v);
        QVERIFY(v.toUInt() == 0);
        file.stat(id4, v);
        QVERIFY(v.toUInt() == 0);

        file.setStat(id1, QVariant(value1b));
        file.setStat(id4, QVariant(value4b));

        file.stat(id1, v);
        QVERIFY(v.toUInt() == value1b);
        file.stat(id4, v);
        QVERIFY(v.toUInt() == value4b);

        // interval 3 ////////////////////////////////
        file.newInterval();

        file.stat(id1, v);
        QVERIFY(v.toUInt() == 0);
        file.stat(id4, v);
        QVERIFY(v.toUInt() == 0);

        file.setStat(id1, QVariant(value1c));
        file.setStat(id4, QVariant(value4c));

        file.stat(id1, v);
        QVERIFY(v.toUInt() == value1c);
        file.stat(id4, v);
        QVERIFY(v.toUInt() == value4c);

        file.save();
    }

    {
        Stats::SecureStatsFile file;

        file.load(FILENAME_1);

        file.stat(id1, v);
        QVERIFY(v.toUInt() == value1c);
        file.stat(id4, v);
        QVERIFY(v.toUInt() == value4c);
        file.stat(id5, v);
        QVERIFY(v.toUInt() == 0);

        file.history(id1, h);
        QVERIFY(h.size() == 3);
        QVERIFY(h[0].second.toUInt() == value1a);
        QVERIFY(h[1].second.toUInt() == value1b);
        QVERIFY(h[2].second.toUInt() == value1c);

        file.history(id4, h);
        QVERIFY(h.size() == 3);
        QVERIFY(h[0].second.toUInt() == value4a + value4b);
        QVERIFY(h[1].second.toUInt() == value4b);
        QVERIFY(h[2].second.toUInt() == value4c);

        file.combinedHistory(id1, id4, h);
        QVERIFY(h.size() == 3);
        QVERIFY(h[0].second.toUInt() == value1a + value4a + value4b);
        QVERIFY(h[1].second.toUInt() == value1b + value4b);
        QVERIFY(h[2].second.toUInt() == value1c + value4c);

        file.combinedHistory(id1, id5, h);
        QVERIFY(h.size() == 3);
        QVERIFY(h[0].second.toUInt() == value1a);
        QVERIFY(h[1].second.toUInt() == value1b);
        QVERIFY(h[2].second.toUInt() == value1c);
    }
}

//--------------------------------------------------------------------------------------------------

QTEST_APPLESS_MAIN(SecureStatsFileTest)

#include "tst_securestatsfiletest.moc"
