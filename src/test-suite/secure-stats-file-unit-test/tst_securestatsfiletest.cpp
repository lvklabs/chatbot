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
    SecureStatsFileTest();

private Q_SLOTS:
    void initTestCase();
    void init();
    void cleanupTestCase();
    void testIsEmptyAndClose();
    void testIsEmptySaveAndClear();
    void testSetSaveAndLoad();
    void testCase1();
    void testCase1_data();
};

//--------------------------------------------------------------------------------------------------

SecureStatsFileTest::SecureStatsFileTest()
{
}

//--------------------------------------------------------------------------------------------------

void SecureStatsFileTest::initTestCase()
{
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

void SecureStatsFileTest::testCase1()
{
    QFETCH(QString, data);
    QVERIFY2(true, "Failure");
}

//--------------------------------------------------------------------------------------------------

void SecureStatsFileTest::testCase1_data()
{
    QTest::addColumn<QString>("data");
    QTest::newRow("0") << QString();
}

//--------------------------------------------------------------------------------------------------

QTEST_APPLESS_MAIN(SecureStatsFileTest)


#include "tst_securestatsfiletest.moc"
