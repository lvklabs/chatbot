#include "restmock.h"
#include "da-server/updater.h"

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

private Q_SLOTS:
    void testCase1();
    void testCase1_data();
};

//--------------------------------------------------------------------------------------------------

UpdaterUnitTest::UpdaterUnitTest()
{
}

//--------------------------------------------------------------------------------------------------

void UpdaterUnitTest::testCase1_data()
{
    QTest::addColumn<QString>("filename");

    QTest::newRow("w1")  << QString("update1-wrong.xml");
    QTest::newRow("w2")  << QString("update2-wrong.xml");
    QTest::newRow("w3")  << QString("update3-wrong.xml");
    QTest::newRow("ok1") << QString("update1.xml");
    QTest::newRow("ok2") << QString("update2.xml");
    QTest::newRow("ok3") << QString("update3.xml");
}

//--------------------------------------------------------------------------------------------------

void UpdaterUnitTest::testCase1()
{
    QFETCH(QString, filename);

    RestMock *rest = new RestMock();

    QVERIFY(rest->setResponse(filename));

    DAS::Updater * updater = new DAS::Updater(rest);

    updater->checkForUpdate();
}

//--------------------------------------------------------------------------------------------------

QTEST_MAIN(UpdaterUnitTest)

#include "updaterunittest.moc"

