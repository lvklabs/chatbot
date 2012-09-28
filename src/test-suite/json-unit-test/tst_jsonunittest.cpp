#include <QtCore/QString>
#include <QtTest/QtTest>

#include "common/json.h"

using namespace Lvk::Cmn;

//--------------------------------------------------------------------------------------------------
// JsonUnitTest
//--------------------------------------------------------------------------------------------------

class JsonUnitTest : public QObject
{
    Q_OBJECT

public:
    JsonUnitTest();

private Q_SLOTS:
    void testParser();
    void testParser_data();
};

//--------------------------------------------------------------------------------------------------

JsonUnitTest::JsonUnitTest()
{
}

//--------------------------------------------------------------------------------------------------

void JsonUnitTest::testParser_data()
{
    QString jsonNull        = "{}";
    QString jsonAuthOk      = "{\"username\": \"andres.test\", \"email\": \"test@gmail.com\", \"uid\": \"1575963427\"}";
    QString jsonAuthFailed  = "{\"error_message\": \"No pudimos encontrar tu usuario en la base de datos! Estas registrado en DaleAceptar?\", \"error_code\": 1}";
    QString jsonAuthFailed2 = "{\"error_message\": \"No pudimos encontrar tu usuario en la base de datos! Estas \\\"registrado\\\" en DaleAceptar?\", \"error_code\": 1.3}";
    Json::Object objNull;
    Json::Object objOk;
    Json::Object objErr;
    Json::Object objErr2;

    objOk[QString("username")] = QString("andres.test");
    objOk[QString("email")]    = QString("test@gmail.com");
    objOk[QString("uid")]      = QString("1575963427");

    objErr[QString("error_message")] = QString("No pudimos encontrar tu usuario en la base de datos! Estas registrado en DaleAceptar?");
    objErr[QString("error_code")]    = double(1);

    objErr2[QString("error_message")] = QString("No pudimos encontrar tu usuario en la base de datos! Estas \\\"registrado\\\" en DaleAceptar?");
    objErr2[QString("error_code")]    = double(1.3);

    QTest::addColumn<QString>("jsonStr");
    QTest::addColumn<Json::Object>("expObj");
    QTest::newRow("null") << jsonNull        << objNull;
    QTest::newRow("ok")   << jsonAuthOk      << objOk;
    QTest::newRow("err")  << jsonAuthFailed  << objErr;
    QTest::newRow("err2") << jsonAuthFailed2 << objErr2;
}

//--------------------------------------------------------------------------------------------------

void JsonUnitTest::testParser()
{
    QFETCH(QString, jsonStr);
    QFETCH(Json::Object, expObj);

    Json jparser;
    Json::Object obj;

    QVERIFY(jparser.parse(jsonStr, obj));

    QCOMPARE(obj, expObj);
}

//--------------------------------------------------------------------------------------------------

QTEST_APPLESS_MAIN(JsonUnitTest)

#include "tst_jsonunittest.moc"
