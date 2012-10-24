#include "restmock.h"
#include "da-server/updater.h"
#include "da-server/updateinfo.h"

#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QtCore/QCoreApplication>

using namespace Lvk;

//--------------------------------------------------------------------------------------------------
// UpdaterUnitTest
//--------------------------------------------------------------------------------------------------

class UpdaterUnitTest : public QObject
{
    Q_OBJECT

public:
    UpdaterUnitTest();

    void clear()
    {
        m_noUpdate = false;
        m_update = false;
        m_info.clear();
    }

private Q_SLOTS:

    void testCase1();
    void testCase1_data();


    void onNoUpdate()
    {
        m_noUpdate = true;
        m_update = false;
        m_info.clear();
    }

    void onUpdate(const DAS::UpdateInfo &info)
    {
        m_noUpdate = false;
        m_update = true;
        m_info = info;
    }

private:
    DAS::UpdateInfo m_info;
    bool m_update;
    bool m_noUpdate;
};

//--------------------------------------------------------------------------------------------------

UpdaterUnitTest::UpdaterUnitTest()
{
}

//--------------------------------------------------------------------------------------------------

void UpdaterUnitTest::testCase1_data()
{
    const QString URL_1 = "http://lvklabs.com/builds/chatbot/chatbot-linux32-0.23.zip";

    const QDate DATE_1 = QDate::fromString("28/10/2012", "dd/MM/yyyy");
    const QDate DATE_2 = QDate::fromString("18/01/2011", "dd/MM/yyyy");

    QTest::addColumn<QString>("filename");
    QTest::addColumn<QString>("curVer");
    QTest::addColumn<bool>("newer");
    QTest::addColumn<int>("severity");
    QTest::addColumn<int>("verMajor");
    QTest::addColumn<int>("verMinor");
    QTest::addColumn<QString>("url");
    QTest::addColumn<QDate>("date");
    QTest::addColumn<QStringList>("wn");
    QTest::addColumn<QString>("hash");

    QTest::newRow("w1")  << QString("update1-wrong.xml") << "1.5"  << false
                         << -1 << -1 << -1 << QString() << QDate() << QStringList() << QString();

    QTest::newRow("w2")  << QString("update2-wrong.xml") << "1.0"  << false
                         << -1 << -1 << -1 << QString() << QDate() << QStringList() << QString();

    QTest::newRow("w3")  << QString("update3-wrong.xml") << "0.0"  << false
                         << -1 << -1 << -1 << QString() << QDate() << QStringList() << QString();

    QTest::newRow("ok1") << QString("update1.xml")         << "0.98" << true
                         << (int)DAS::UpdateInfo::Low      << 0 << 99 << URL_1 << DATE_2
                         << QStringList() << QString();

    QTest::newRow("ok2") << QString("update2.xml")         << "1.1"  << false
                         << (int)DAS::UpdateInfo::Critical << 1 << 1 << URL_1 << DATE_2
                         << QStringList() << QString();

    QTest::newRow("ok3") << QString("update3.xml")         << "1.2"  << true
                         << (int)DAS::UpdateInfo::Critical << 1 << 3 << URL_1 << DATE_1
                         << (QStringList() << "Hi!")
                         << QString("ce286df1e54f176ca76a8020a1ae3b464223fc0f");

    QTest::newRow("ok4") << QString("update4.xml")         << "1.9"  << true
                         << (int)DAS::UpdateInfo::Low      << 2 << 0 << URL_1 << DATE_1
                         << (QStringList() << QString::fromWCharArray(L"áéíóúñ") << "Other string")
                         << QString();

    QTest::newRow("ok5") << QString("update5.xml")         << "1.2"  << true
                         << (int)DAS::UpdateInfo::Critical << 1 << 3 << URL_1 << DATE_1
                         << (QStringList() << "Some stuff 2" << "Some stuff 3")
                         << QString("ce286df1e54f176ca76a8020a1ae3b464223fc0f");

}

//--------------------------------------------------------------------------------------------------

void UpdaterUnitTest::testCase1()
{
    QFETCH(QString, filename);
    QFETCH(QString, curVer);
    QFETCH(bool, newer);
    QFETCH(int, severity);
    QFETCH(int, verMajor);
    QFETCH(int, verMinor);
    QFETCH(QString, url);
    QFETCH(QDate, date);
    QFETCH(QStringList, wn);
    QFETCH(QString, hash);

    RestMock *rest = new RestMock();

    QVERIFY(rest->setResponse(filename));

    DAS::Updater * updater = new DAS::Updater(rest);
    updater->setIgnoreSslErrors(true);
    updater->m_curVersion = DAS::UpdateVersion(curVer);
    connect(updater, SIGNAL(noUpdate()),              SLOT(onNoUpdate()));
    connect(updater, SIGNAL(update(DAS::UpdateInfo)), SLOT(onUpdate(DAS::UpdateInfo)));

    clear();

    updater->checkForUpdate();

    // wrong or invalid xml files must produce a noUpdate signal
    if (!newer || filename.contains("wrong")) {
        QVERIFY(m_noUpdate);
        QVERIFY(!m_update);
    } else {
        QVERIFY(!m_noUpdate);
        QVERIFY(m_update);
        QCOMPARE(severity, (int)m_info.severity());
        QCOMPARE(verMajor, m_info.version()._major);
        QCOMPARE(verMinor, m_info.version()._minor);
        QCOMPARE(url, m_info.url());
        QCOMPARE(date, m_info.date());
        QCOMPARE(wn.size(), m_info.whatsNew().size());
        QCOMPARE(wn, m_info.whatsNew());
        QCOMPARE(hash, m_info.hash());
    }
}

//--------------------------------------------------------------------------------------------------

QTEST_MAIN(UpdaterUnitTest)

#include "updaterunittest.moc"

