#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QFile>
#include <QDir>
#include <QMetaType>

#include "stats/statsmanager.h"
#include "stats/metric.h"
#include "stats/securestatsfile.h"
#include "common/conversationreader.h"

Q_DECLARE_METATYPE(Lvk::BE::Rule *)
Q_DECLARE_METATYPE(Lvk::Cmn::Conversation::Entry)
Q_DECLARE_METATYPE(Lvk::Cmn::Conversation)


#define CHATBOT_ID_1     "bee2c509-3380-4954-9e2e-a39985ed25b1"
#define CHATBOT_ID_2     "bee2c509-3380-4954-9e2e-a39985ed25b2"

#define STAT_DIR         "./stats/"
#define STAT_EXT         ".stat"

#define STAT_FILENAME_1  STAT_DIR CHATBOT_ID_1 STAT_EXT
#define STAT_FILENAME_2  STAT_DIR CHATBOT_ID_2 STAT_EXT

// Rules Points
const int COND_P  = 4; // conditional
const int VAR_P   = 3; // variable
const int REGEX_P = 2; // regex
const int KWOP_P  = 2; // keyword op
const int SIMPL_P = 1; // simple

// Conversation Points
const int CONV_P = 1000; // Valid conversation points

#define CONV_SHORT_FILENAME     "conv_short.txt"
#define CONV_LONG_FILENAME      "conv_long.txt"
#define CONV_LONG2_FILENAME     "conv_long2.txt"
#define CONV_INACT_FILENAME     "conv_inactive.txt"
#define CONV_INTER_FILENAME     "conv_interfered.txt"

const int CONV_SHORT_CONV_SCORE = 4 + 7;   // #lines + #lexicon
const int CONV_SHORT_CONT_SCORE = 0;
const int CONV_LONG_CONV_SCORE = 27 + 29;  // #lines + #lexicon
const int CONV_LONG_CONT_SCORE = 1*CONV_P;
const int CONV_LONG2_CONV_SCORE = 24 + 26; // #lines + #lexicon
const int CONV_LONG2_CONT_SCORE = 1*CONV_P;
const int CONV_INACT_CONV_SCORE = 27 + 29; // #lines + #lexicon
const int CONV_INACT_CONT_SCORE = 0;
const int CONV_INTER_CONV_SCORE = 13 + 14; // #lines + #lexicon
const int CONV_INTER_CONT_SCORE = 0;

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
    BE::Rule *rule = new BE::Rule("", BE::Rule::OrdinaryRule, input, output);
    parent->appendChild(rule);

    return rule;
}

//--------------------------------------------------------------------------------------------------

inline BE::Rule * newRuleTree1()
{
    BE::Rule *root = new BE::Rule("", BE::Rule::ContainerRule);
    newOrdinaryRule(QStringList() << "Hi *",QStringList() << "Hello",   root);

    return root;
}

//--------------------------------------------------------------------------------------------------

inline int ruleTree1Score()
{
    return REGEX_P;
}

//--------------------------------------------------------------------------------------------------

inline BE::Rule * newRuleTree2()
{
    BE::Rule *root = new BE::Rule("", BE::Rule::ContainerRule);
    newOrdinaryRule(QStringList() << "Hi *",               QStringList() << "Hello",   root);
    newOrdinaryRule(QStringList() << "Soccer **",          QStringList() << "Hello",   root);
    newOrdinaryRule(QStringList() << "[hecho]",            QStringList() << "[hecho]", root);
    newOrdinaryRule(QStringList() << "Do you play [sth]?",
                    QStringList() << "{if [sth] = soccer}Yes{else}No", root);

    return root;
}

//--------------------------------------------------------------------------------------------------

inline int ruleTree2Score()
{
    return REGEX_P + KWOP_P + VAR_P + COND_P;
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
            Stats::StatsManager::manager()->setFilename("");
        }
        delete Stats::StatsManager::m_manager;
        Stats::StatsManager::m_manager = new Stats::StatsManager();
    }

    Stats::StatsManager* manager()
    {
        return Stats::StatsManager::manager();
    }

    void simulateScoreTimeout()
    {
        manager()->m_elapsedTime = manager()->scoreRemainingTime();
        manager()->onScoreTick();
    }

private Q_SLOTS:
    void initTestCase();
    void init();
    void cleanupTestCase();
    void testFileCreationAndClear();
    void testSetMetricsAndIntervals();
    void testScoreAlgorithm_data();
    void testScoreAlgorithm();
    void testBestScoreAndIntervals();
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

    manager()->setFilename(STAT_FILENAME_1);
    manager()->clear();
    manager()->setFilename(STAT_FILENAME_2);
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

    manager()->setFilename(STAT_FILENAME_1);
    manager()->m_statsFile->setMetric(Stats::HistoryChatbotDiffLines, 1);
    manager()->setFilename(STAT_FILENAME_2);
    manager()->m_statsFile->setMetric(Stats::HistoryChatbotDiffLines, 2);

    resetManager();

    QVERIFY(QFile::exists(STAT_FILENAME_1));
    QVERIFY(QFile::exists(STAT_FILENAME_2));

    manager()->setFilename(STAT_FILENAME_1);
    manager()->clear();
    QVERIFY(!QFile::exists(STAT_FILENAME_1));

    manager()->setFilename(STAT_FILENAME_2);
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
        manager()->setFilename((i % 2 == 0) ? STAT_FILENAME_1 : STAT_FILENAME_2);

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

    manager()->setFilename(STAT_FILENAME_1);

    QVERIFY(manager()->metric(m1).toUInt() == m1_fvalue1);
    QVERIFY(manager()->metric(m2).toUInt() == m1_fvalue2);
    QVERIFY(manager()->metric(m3).toUInt() == m1_fvalue3);
    QVERIFY(manager()->metric(m4).toUInt() == m1_fvalue4);

    manager()->setFilename(STAT_FILENAME_2);

    QVERIFY(manager()->metric(m1).toUInt() == m2_fvalue1);
    QVERIFY(manager()->metric(m2).toUInt() == m2_fvalue2);
    QVERIFY(manager()->metric(m3).toUInt() == m2_fvalue3);
    QVERIFY(manager()->metric(m4).toUInt() == m2_fvalue4);

    manager()->m_statsFile->newInterval();

    QVERIFY(manager()->metric(m1).toUInt() == 0);
    QVERIFY(manager()->metric(m2).toUInt() == 0);
    QVERIFY(manager()->metric(m3).toUInt() == 0);
    QVERIFY(manager()->metric(m4).toUInt() == 0);

    manager()->setFilename(STAT_FILENAME_1);

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
    int root5b_score = SIMPL_P + KWOP_P + REGEX_P + VAR_P + COND_P;

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Conversations

    Cmn::Conversation c_short = readConversation(CONV_SHORT_FILENAME);
    QVERIFY(!c_short.isEmpty());

    Cmn::Conversation c_long  = readConversation(CONV_LONG_FILENAME);
    QVERIFY(!c_long.isEmpty());

    Cmn::Conversation c_inact = readConversation(CONV_INACT_FILENAME);
    QVERIFY(!c_inact.isEmpty());

    Cmn::Conversation c_inter = readConversation(CONV_INTER_FILENAME);
    QVERIFY(!c_inter.isEmpty());

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
    QTest::newRow("r5b") << root5b << Cmn::Conversation() << Stats::Score(root5b_score, 0,0);

    QTest::newRow("cs")  << reinterpret_cast<BE::Rule *>(0) << c_short << Stats::Score(0, CONV_SHORT_CONT_SCORE, CONV_SHORT_CONV_SCORE);
    QTest::newRow("cl")  << reinterpret_cast<BE::Rule *>(0) << c_long  << Stats::Score(0, CONV_LONG_CONT_SCORE, CONV_LONG_CONV_SCORE);
    QTest::newRow("cli") << reinterpret_cast<BE::Rule *>(0) << c_inact << Stats::Score(0, CONV_INACT_CONT_SCORE, CONV_INACT_CONV_SCORE);
    QTest::newRow("clI") << reinterpret_cast<BE::Rule *>(0) << c_inter << Stats::Score(0, CONV_INTER_CONT_SCORE, CONV_INTER_CONV_SCORE);

    QTest::newRow("m1") << root5a << c_long << Stats::Score(root5a_score, CONV_LONG_CONT_SCORE, CONV_LONG_CONV_SCORE);
    QTest::newRow("m2") << root5a << c_inact << Stats::Score(root5a_score, CONV_INACT_CONT_SCORE, CONV_INACT_CONV_SCORE);
    QTest::newRow("m3") << root5b << c_inter << Stats::Score(root5b_score, CONV_INTER_CONT_SCORE, CONV_INTER_CONV_SCORE);
}

//--------------------------------------------------------------------------------------------------

void StatsManagerTest::testScoreAlgorithm()
{
    QFETCH(BE::Rule *, root);
    QFETCH(Cmn::Conversation, conv);
    QFETCH(Stats::Score, s);

    manager()->setFilename(STAT_FILENAME_1);
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

void StatsManagerTest::testBestScoreAndIntervals()
{
    manager()->setFilename(STAT_FILENAME_1);
    manager()->clear();

    BE::Rule *root1 = newRuleTree1();
    BE::Rule *root2 = newRuleTree2();

    Cmn::Conversation conv1 = readConversation(CONV_SHORT_FILENAME);
    Cmn::Conversation conv2 = readConversation(CONV_LONG_FILENAME);
    Cmn::Conversation conv3 = readConversation(CONV_LONG2_FILENAME);


    QVERIFY(manager()->bestScore().isNull());
    QVERIFY(manager()->currentScore().isNull());
    QVERIFY(manager()->m_elapsedTime == 0);

    // Score 1 -----------------------

    manager()->updateScoreWith(root1);

    foreach (const Cmn::Conversation::Entry &e, conv1.entries()) {
        manager()->updateScoreWith(e);
    }

    Stats::Score s1(ruleTree1Score(), CONV_SHORT_CONT_SCORE, CONV_SHORT_CONV_SCORE);

    QVERIFY(manager()->currentScore() == s1);
    QVERIFY(manager()->bestScore() == s1);

    // Interval 2 -----------------------

    simulateScoreTimeout();

    QVERIFY(manager()->currentScore().rules == s1.rules);
    QVERIFY(manager()->currentScore().contacts == s1.contacts);
    QVERIFY(manager()->currentScore().conversations == 0);
    QVERIFY(manager()->bestScore() == s1);

    manager()->updateScoreWith(root2);

    foreach (const Cmn::Conversation::Entry &e, conv2.entries()) {
        manager()->updateScoreWith(e);
    }

    Stats::Score s2(ruleTree2Score(), CONV_LONG_CONT_SCORE, CONV_LONG_CONV_SCORE);

    QVERIFY(manager()->currentScore() == s2);
    QVERIFY(manager()->bestScore() == s2);

    // Interval 3 -----------------------

    simulateScoreTimeout();

    QVERIFY(manager()->currentScore().rules == s2.rules);
    QVERIFY(manager()->currentScore().contacts == s2.contacts);
    QVERIFY(manager()->currentScore().conversations == 0);
    QVERIFY(manager()->bestScore() == s2);

    manager()->updateScoreWith(root1);

    foreach (const Cmn::Conversation::Entry &e, conv2.entries()) {
        manager()->updateScoreWith(e);
    }

    Stats::Score s3(ruleTree1Score(), CONV_LONG_CONT_SCORE, CONV_LONG_CONV_SCORE);

    QVERIFY(manager()->currentScore() == s3);
    QVERIFY(manager()->bestScore() == s2);

    // Interval 4 -----------------------

    simulateScoreTimeout();

    QVERIFY(manager()->currentScore().rules == s3.rules);
    QVERIFY(manager()->currentScore().contacts == s3.contacts);
    QVERIFY(manager()->currentScore().conversations == 0);
    QVERIFY(manager()->bestScore() == s2);

    resetManager();
    manager()->setFilename(STAT_FILENAME_1);

    manager()->updateScoreWith(root2);

    foreach (const Cmn::Conversation::Entry &e, conv1.entries()) {
        manager()->updateScoreWith(e);
    }

    Stats::Score s4(ruleTree2Score(), CONV_LONG_CONT_SCORE, CONV_SHORT_CONV_SCORE);

    QVERIFY(manager()->currentScore() == s4);
    QVERIFY(manager()->bestScore() == s2);

    // Interval 5 -----------------------

    simulateScoreTimeout();

    QVERIFY(manager()->currentScore().rules == s4.rules);
    QVERIFY(manager()->currentScore().contacts == s4.contacts);
    QVERIFY(manager()->currentScore().conversations == 0);
    QVERIFY(manager()->bestScore() == s2);

    manager()->updateScoreWith(root2);

    foreach (const Cmn::Conversation::Entry &e, conv3.entries()) {
        manager()->updateScoreWith(e);
    }

    Stats::Score s5(ruleTree2Score(), CONV_LONG_CONT_SCORE + CONV_LONG2_CONT_SCORE, CONV_LONG2_CONV_SCORE);

    QVERIFY(manager()->currentScore() == s5);
    QVERIFY(manager()->bestScore() == s5);
}

//--------------------------------------------------------------------------------------------------

QTEST_APPLESS_MAIN(StatsManagerTest)

#include "statsmanagertest.moc"
