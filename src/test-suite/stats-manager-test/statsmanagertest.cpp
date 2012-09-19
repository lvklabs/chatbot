#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QFile>
#include <QDir>
#include <QMetaType>

#include "stats/statsmanager.h"
#include "stats/metric.h"
#include "stats/securestatsfile.h"
#include "common/conversationreader.h"

#define CHATBOT_ID_1     "bee2c509-3380-4954-9e2e-a39985ed25b1"
#define CHATBOT_ID_2     "bee2c509-3380-4954-9e2e-a39985ed25b2"

#define STAT_DIR         "./stats/"
#define STAT_EXT         ".stat"

#define STAT_FILENAME_1  STAT_DIR CHATBOT_ID_1 STAT_EXT
#define STAT_FILENAME_2  STAT_DIR CHATBOT_ID_2 STAT_EXT

#define CONV_SHORT_FILENAME     "conv_short.txt"
#define CONV_LONG_FILENAME      "conv_long.txt"
#define CONV_INACT_FILENAME     "conv_inactive.txt"
#define CONV_INTER_FILENAME     "conv_interfered.txt"


Q_DECLARE_METATYPE(Lvk::BE::Rule *)
Q_DECLARE_METATYPE(Lvk::Cmn::Conversation::Entry)
Q_DECLARE_METATYPE(Lvk::Cmn::Conversation)


using namespace Lvk;

//--------------------------------------------------------------------------------------------------
// Helpers
//--------------------------------------------------------------------------------------------------

inline Cmn::Conversation readConversation(const QString &filename)
{
    Cmn::Conversation conv;
    Cmn::ConversationReader reader(filename);

    if (!reader.read(&conv)) {
        conv.clear();
    }

    return conv;
}

//--------------------------------------------------------------------------------------------------

inline BE::Rule * newOrdinaryRule(const QStringList &input, const QStringList &output,
                                  BE::Rule *parent)
{
    return new BE::Rule("", BE::Rule::OrdinaryRule, input, output, parent);
}

//--------------------------------------------------------------------------------------------------
// StatsManagerUnitTest
//--------------------------------------------------------------------------------------------------

class StatsManagerTest : public QObject
{
    Q_OBJECT

public:
    StatsManagerTest() {}

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
    void testSetMetricsAndIntervals();
    void testScoreAlgorithm_data();
    void testScoreAlgorithm();
};

//--------------------------------------------------------------------------------------------------

void StatsManagerTest::initTestCase()
{
    QVERIFY(QDir().mkpath(STAT_DIR));
}

//--------------------------------------------------------------------------------------------------

void StatsManagerTest::init()
{
    resetManager();

    manager()->setChatbotId(CHATBOT_ID_1);
    manager()->clear();
    manager()->setChatbotId(CHATBOT_ID_2);
    manager()->clear();

    if (QFile::exists(STAT_FILENAME_1)) {
        QVERIFY(QFile::remove(STAT_FILENAME_1));
    }
    if (QFile::exists(STAT_FILENAME_2)) {
        QVERIFY(QFile::remove(STAT_FILENAME_2));
    }
}

//--------------------------------------------------------------------------------------------------

void StatsManagerTest::cleanupTestCase()
{
    QFile::remove(STAT_FILENAME_1);
    QFile::remove(STAT_FILENAME_2);
}

//--------------------------------------------------------------------------------------------------

void StatsManagerTest::testFileCreationAndClear()
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

void StatsManagerTest::testSetMetricsAndIntervals()
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

void StatsManagerTest::testScoreAlgorithm_data()
{
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Points

    // Rules
    const int COND_P  = 4; // conditional
    const int VAR_P   = 3; // variable
    const int REGEX_P = 2; // regex
    const int KWOP_P  = 2; // keyword op
    const int SIMPL_P = 1; // simple

    // Conversation
    const int CONV_P = 1000; // Valid conversation points

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Rules

    BE::Rule *root0  = new BE::Rule("", BE::Rule::ContainerRule);
    BE::Rule *root1a = new BE::Rule("", BE::Rule::ContainerRule);
    BE::Rule *root1b = new BE::Rule("", BE::Rule::ContainerRule);
    BE::Rule *root1c = new BE::Rule("", BE::Rule::ContainerRule);
    BE::Rule *root1d = new BE::Rule("", BE::Rule::ContainerRule);
    BE::Rule *root1e = new BE::Rule("", BE::Rule::ContainerRule);
    BE::Rule *root1f = new BE::Rule("", BE::Rule::ContainerRule);
    BE::Rule *root1g = new BE::Rule("", BE::Rule::ContainerRule);
    BE::Rule *root1h = new BE::Rule("", BE::Rule::ContainerRule);
    BE::Rule *root1i = new BE::Rule("", BE::Rule::ContainerRule);
    BE::Rule *root5a = new BE::Rule("", BE::Rule::ContainerRule);
    BE::Rule *root5b = new BE::Rule("", BE::Rule::ContainerRule);

    newOrdinaryRule(QStringList(),                         QStringList(),              root1a);
    newOrdinaryRule(QStringList() << "Hi!",                QStringList(),              root1b);
    newOrdinaryRule(QStringList(),                         QStringList() << "Hello",   root1c);
    newOrdinaryRule(QStringList() << "Hi!",                QStringList() << "Hello",   root1d);
    newOrdinaryRule(QStringList() << "Hi *",               QStringList() << "Hello",   root1e);
    newOrdinaryRule(QStringList() << "Soccer **",          QStringList() << "Hello",   root1f);
    newOrdinaryRule(QStringList() << "[hecho]",            QStringList() << "[hecho]", root1g);
    newOrdinaryRule(QStringList() << "Do you play [sth]?", QStringList() << "{if [sth] = soccer}Yes{else}No", root1h);
    newOrdinaryRule(QStringList() << "Do you play [sth]?", QStringList() << "{if sth = soccer}Yes{else}No", root1i);
    newOrdinaryRule(QStringList() << "Hi!",                QStringList() << "Hello",   root5a);
    newOrdinaryRule(QStringList() << "Hey",                QStringList() << "Hello",   root5a);
    newOrdinaryRule(QStringList() << "Hello",              QStringList() << "Hello",   root5a);
    newOrdinaryRule(QStringList() << "Hi *",               QStringList() << "Hello",   root5a);
    newOrdinaryRule(QStringList() << "Soccer **",          QStringList() << "Hello",   root5a);
    newOrdinaryRule(QStringList() << "[hecho]",            QStringList() << "[hecho]", root5a);
    newOrdinaryRule(QStringList() << "Do you play [sth]?", QStringList() << "{if [sth] = soccer}Yes{else}No", root5a);
    newOrdinaryRule(QStringList() << "Do you like [sth]?", QStringList() << "{if [sth] = ice-cream}Yes{else}No", root5a);
    newOrdinaryRule(QStringList() << "Hi!",                QStringList() << "Hello",   root5b);
    newOrdinaryRule(QStringList() << "Hi!",                QStringList() << "Hello",   root5b);
    newOrdinaryRule(QStringList() << "Hi!",                QStringList() << "Hello",   root5b);
    newOrdinaryRule(QStringList() << "Hi *",               QStringList() << "Hello",   root5b);
    newOrdinaryRule(QStringList() << "Soccer **",          QStringList() << "Hello",   root5b);
    newOrdinaryRule(QStringList() << "[hecho]",            QStringList() << "[hecho]", root5b);
    newOrdinaryRule(QStringList() << "Do you play [sth]?", QStringList() << "{if [sth] = soccer}Yes{else}No", root5b);
    newOrdinaryRule(QStringList() << "Do you play [sth]?", QStringList() << "{if [sth] = soccer}Yes{else}No", root5b);

    int root5a_score = SIMPL_P*3 + KWOP_P + REGEX_P + VAR_P + COND_P*2;
    //int root5b_score = SIMPL_P + KWOP_P + REGEX_P + VAR_P + COND_P;

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Conversations

    Cmn::Conversation c_short = readConversation(CONV_SHORT_FILENAME);
    QVERIFY(!c_short.isEmpty());
    int c_short_conv_score = 4 + 7; // #lines + #lexicon
    int c_short_cont_score = 0;

    Cmn::Conversation c_long  = readConversation(CONV_LONG_FILENAME);
    QVERIFY(!c_long.isEmpty());
    int c_long_conv_score = 27 + 29; // #lines + #lexicon
    int c_long_cont_score = 1*CONV_P;

    Cmn::Conversation c_inact = readConversation(CONV_INACT_FILENAME);
    QVERIFY(!c_inact.isEmpty());
    int c_inact_conv_score = 27 + 29; // #lines + #lexicon
    int c_inact_cont_score = 0;

    Cmn::Conversation c_inter = readConversation(CONV_INTER_FILENAME);
    QVERIFY(!c_inter.isEmpty());
    int c_inter_conv_score = 24 + 25; // #lines + #lexicon
    int c_inter_cont_score = 0;

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Cols & Rows

    QTest::addColumn<BE::Rule *>("root");
    QTest::addColumn<Cmn::Conversation>("conv");
    QTest::addColumn<Stats::Score>("s"); // Expected score for root and conv

    QTest::newRow("null") << reinterpret_cast<BE::Rule *>(0) << Cmn::Conversation() << Stats::Score();

    QTest::newRow("r0")  << root0  << Cmn::Conversation() << Stats::Score();
    QTest::newRow("r1a") << root1a << Cmn::Conversation() << Stats::Score();
    QTest::newRow("r1b") << root1b << Cmn::Conversation() << Stats::Score();
    QTest::newRow("r1d") << root1d << Cmn::Conversation() << Stats::Score(SIMPL_P,0,0);
    QTest::newRow("r1e") << root1e << Cmn::Conversation() << Stats::Score(REGEX_P,0,0);
    QTest::newRow("r1f") << root1f << Cmn::Conversation() << Stats::Score(KWOP_P,0,0);
    QTest::newRow("r1g") << root1g << Cmn::Conversation() << Stats::Score(VAR_P,0,0);
    QTest::newRow("r1h") << root1h << Cmn::Conversation() << Stats::Score(COND_P,0,0);
    QTest::newRow("r1i") << root1i << Cmn::Conversation() << Stats::Score(SIMPL_P,0,0);
    QTest::newRow("r5a") << root5a << Cmn::Conversation() << Stats::Score(root5a_score, 0,0);
    // TODO QTest::newRow("r5b") << root5b << Cmn::Conversation() << Stats::Score(root5b_score, 0,0);

    QTest::newRow("cs")  << reinterpret_cast<BE::Rule *>(0) << c_short << Stats::Score(0, c_short_cont_score, c_short_conv_score);
    QTest::newRow("cl")  << reinterpret_cast<BE::Rule *>(0) << c_long << Stats::Score(0, c_long_cont_score, c_long_conv_score);
    QTest::newRow("cli") << reinterpret_cast<BE::Rule *>(0) << c_inact << Stats::Score(0, c_inact_cont_score, c_inact_conv_score);
    QTest::newRow("clI") << reinterpret_cast<BE::Rule *>(0) << c_inter << Stats::Score(0, c_inter_cont_score, c_inter_conv_score);

    QTest::newRow("m1") << root5a << c_long << Stats::Score(root5a_score, c_long_cont_score, c_long_conv_score);
    QTest::newRow("m2") << root5a << c_inact << Stats::Score(root5a_score, c_inact_cont_score, c_inact_conv_score);
    // TODO QTest::newRow("m4") << root5b << c_inter << Stats::Score(root5b_score, c_inter_cont_score, c_inter_conv_score);
}

//--------------------------------------------------------------------------------------------------

void StatsManagerTest::testScoreAlgorithm()
{
    QFETCH(BE::Rule *, root);
    QFETCH(Cmn::Conversation, conv);
    QFETCH(Stats::Score, s);

    manager()->setChatbotId(CHATBOT_ID_1);
    manager()->clear();

    QVERIFY(manager()->bestScore().isNull());
    QVERIFY(manager()->currentScore().isNull());
    QVERIFY(manager()->m_elapsedTime == 0);

    manager()->updateScoreWith(root);

    foreach (const Cmn::Conversation::Entry &e, conv.entries()) {
        manager()->updateScoreWith(e);
    }

    if (manager()->currentScore() != s) {
        qDebug() << " - Got:";
        qDebug() << manager()->currentScore().rules;
        qDebug() << manager()->currentScore().contacts;
        qDebug() << manager()->currentScore().conversations;
        qDebug() << " - Expected:";
        qDebug() << s.rules;
        qDebug() << s.contacts;
        qDebug() << s.conversations;
    }

    QVERIFY(manager()->currentScore() == s);
}

//--------------------------------------------------------------------------------------------------


QTEST_APPLESS_MAIN(StatsManagerTest)

#include "statsmanagertest.moc"
