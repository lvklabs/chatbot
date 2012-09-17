#include <QtCore/QByteArray>
#include <QtTest/QtTest>

#include "common/cipher.h"

using namespace Lvk;

//--------------------------------------------------------------------------------------------------
// CipherUnitTest
//--------------------------------------------------------------------------------------------------

class CipherUnitTest : public QObject
{
    Q_OBJECT

public:
    CipherUnitTest() { }

private Q_SLOTS:

#ifdef OPENSSL_SUPPORT
    void testCipherSimmetry();
    void testCipherSimmetry_data();
#endif
};

//--------------------------------------------------------------------------------------------------

void CipherUnitTest::testCipherSimmetry()
{
    QFETCH(QByteArray, data);
    QFETCH(QByteArray, key);

    QByteArray cdata = data;

    QByteArray invalidkey = QByteArray(key);
    invalidkey[3] = '-';

    Cmn::Cipher cipher;

    QVERIFY(cipher.encrypt(cdata, key));
    QVERIFY(data != cdata);
    QVERIFY(!cipher.decrypt(cdata, invalidkey));
    QVERIFY(cipher.decrypt(cdata, key));
    QVERIFY(data == cdata);
}

//--------------------------------------------------------------------------------------------------

void CipherUnitTest::testCipherSimmetry_data()
{
    QTest::addColumn<QByteArray>("data");
    QTest::addColumn<QByteArray>("key");

    QTest::newRow("00") << QByteArray()
                        << QByteArray("1234567890123456");

    QTest::newRow("0") << QByteArray("")
                       << QByteArray("1234567890123456");

    QTest::newRow("1") << QByteArray("mydata")
                       << QByteArray("asdfaalj23243raf");

    QTest::newRow("2") << QByteArray(20*1024*1024, 'c') // big data
                       << QByteArray("!&$/?^*\t\bsser\003\012\014");

    QTest::newRow("3") << QByteArray(new char[100], 100)  // random
                       << QByteArray(new char[16], 16);   // random
}

//--------------------------------------------------------------------------------------------------

QTEST_APPLESS_MAIN(CipherUnitTest)

#include "cipherunittest.moc"
