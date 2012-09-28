#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QtCore/QCoreApplication>

#include "da-server/userauth.h"

using namespace Lvk;

//--------------------------------------------------------------------------------------------------
// UserAuthTest
//--------------------------------------------------------------------------------------------------

class UserAuthTest : public QObject
{
    Q_OBJECT

public:
    UserAuthTest();

private Q_SLOTS:

    void testAuthOk();
    void testAuthOk_data();

    void testAuthErr();
    void testAuthErr_data();

    void testWrongEndpoint();

private slots:

    void onAuthOk(const DAS::UserAuth::AuthInfo &info)
    {
        m_authInfo = info;
    }

    void onAuthError(int code, const QString &msg)
    {
        m_errCode = code;
        m_errMsg = msg;
    }

    void clear()
    {
        m_authInfo.email.clear();
        m_authInfo.uid.clear();
        m_authInfo.username.clear();
        m_errCode = 0;
        m_errMsg.clear();
    }

private:
    DAS::UserAuth::AuthInfo m_authInfo;
    int m_errCode;
    QString m_errMsg;

    void sleep(int secs)
    {
        for (int i = 0; i < secs*100; ++i) {
            QTest::qSleep(1000/100);
            qApp->processEvents();
        }
    }
};

//--------------------------------------------------------------------------------------------------

UserAuthTest::UserAuthTest()
{
}

//--------------------------------------------------------------------------------------------------

void UserAuthTest::testAuthOk_data()
{
    QTest::addColumn<QString>("email");
    QTest::addColumn<QString>("expUsername");
    QTest::addColumn<QString>("expEmail");
    QTest::addColumn<QString>("expUid");

    QTest::newRow("0") << QString("paglian@gmail.com") << QString("andres.pagliano")
                       << QString("paglian@gmail.com") << QString("1575963427");
}

//--------------------------------------------------------------------------------------------------

void UserAuthTest::testAuthOk()
{
#ifndef DA_CONTEST
    QSKIP("DA Contest tests disabled", SkipSingle);
#endif //DA_CONTEST

    QFETCH(QString, email);
    QFETCH(QString, expUsername);
    QFETCH(QString, expEmail);
    QFETCH(QString, expUid);

    clear();

    DAS::UserAuth auth;

    connect(&auth, SIGNAL(success(DAS::UserAuth::AuthInfo)), SLOT(onAuthOk(DAS::UserAuth::AuthInfo)));
    connect(&auth, SIGNAL(error(int,QString)),               SLOT(onAuthError(int,QString)));

    auth.authenticate(email);

    this->sleep(2);

    QCOMPARE(m_authInfo.username, expUsername);
    QCOMPARE(m_authInfo.email, expEmail);
    QCOMPARE(m_authInfo.uid, expUid);
 }

//--------------------------------------------------------------------------------------------------

void UserAuthTest::testAuthErr_data()
{
    QTest::addColumn<QString>("email");
    QTest::addColumn<QString>("expMsg");
    QTest::addColumn<int>("expCode");

    QTest::newRow("0") << QString("paglia@gmail.com") << QString("FIXME")
                       << (int)DAS::UserAuth::UserNotRegistered;
}

//--------------------------------------------------------------------------------------------------

void UserAuthTest::testAuthErr()
{
#ifndef DA_CONTEST
    QSKIP("DA Contest tests disabled", SkipSingle);
#endif //DA_CONTEST

    QFETCH(QString, email);
    QFETCH(QString, expMsg);
    QFETCH(int,     expCode);

    clear();

    DAS::UserAuth auth;

    connect(&auth, SIGNAL(success(DAS::UserAuth::AuthInfo)), SLOT(onAuthOk(DAS::UserAuth::AuthInfo)));
    connect(&auth, SIGNAL(error(int,QString)),               SLOT(onAuthError(int,QString)));

    auth.authenticate(email);

    this->sleep(2);

    QCOMPARE(m_errCode, expCode);
    //QCOMPARE(m_errMsg, expMsg);
 }

//--------------------------------------------------------------------------------------------------

void UserAuthTest::testWrongEndpoint()
{
    clear();

    DAS::UserAuth auth;

    connect(&auth, SIGNAL(success(DAS::UserAuth::AuthInfo)), SLOT(onAuthOk(DAS::UserAuth::AuthInfo)));
    connect(&auth, SIGNAL(error(int,QString)),               SLOT(onAuthError(int,QString)));

    auth.m_rest.request("http://pruebas.daleaceptar.gob.ar/facebook_user_translatio/?email=dasd");

    this->sleep(2);

    QVERIFY(m_errCode != 0);
}

//--------------------------------------------------------------------------------------------------

QTEST_MAIN(UserAuthTest)

#include "userauthtest.moc"
