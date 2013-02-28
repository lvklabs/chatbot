#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QFile>
#include <QDir>

#include "da-clue/scriptmanager.h"
#include "common/settingskeys.h"
#include "common/settings.h"


using namespace Lvk;

//--------------------------------------------------------------------------------------------------
// ScriptManagerUnitTest
//--------------------------------------------------------------------------------------------------

class ScriptManagerUnitTest : public QObject
{
    Q_OBJECT

public:
    ScriptManagerUnitTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCharacters();
    void testCharacters_data();

private:
    QString m_clueDir;

    void createCharsFile(const QString &filename, const QString &content);
};

//--------------------------------------------------------------------------------------------------

ScriptManagerUnitTest::ScriptManagerUnitTest()
    : m_clueDir("./clue_test_data")
{
}

//--------------------------------------------------------------------------------------------------

void ScriptManagerUnitTest::initTestCase()
{
    if (!QDir(m_clueDir).exists()) {
        QVERIFY(QDir().mkpath(m_clueDir));
    }

    Cmn::Settings().setValue(SETTING_CLUE_PATH, m_clueDir);
}

//--------------------------------------------------------------------------------------------------

void ScriptManagerUnitTest::cleanupTestCase()
{
}

//--------------------------------------------------------------------------------------------------

void ScriptManagerUnitTest::testCharacters()
{
    QFETCH(QString, filename);
    QFETCH(QString, content);
    QFETCH(QStringList, expChars);
    QFETCH(int , detectiveIdx);

    if (filename.isEmpty()) {
        filename = Cmn::Settings().value(SETTING_CLUE_CHARS_FILE).toString();
    } else {
        Cmn::Settings().setValue(SETTING_CLUE_CHARS_FILE, filename);
    }

    createCharsFile(filename, content);

    QList<Clue::Character> chars = Clue::ScriptManager().characters();

    QCOMPARE(chars.size(), expChars.size());

    for (int i = 0; i < chars.size(); ++i) {
        QCOMPARE(chars[i].name, expChars[i]);

        if (detectiveIdx == i) {
            QVERIFY(chars[i].detective);
        } else {
            QVERIFY(!chars[i].detective);
        }
    }
}

//--------------------------------------------------------------------------------------------------

void ScriptManagerUnitTest::createCharsFile(const QString &filename, const QString &content)
{
    qDebug() << "ScripManagerUnitTest: Creating file" << filename;

    QFile f(m_clueDir + "/" + filename);

    if (f.exists()) {
        QVERIFY(f.remove());
    }

    QVERIFY(f.open(QFile::WriteOnly));
    QVERIFY(f.write(content.toUtf8()) != -1);
}

//--------------------------------------------------------------------------------------------------

void ScriptManagerUnitTest::testCharacters_data()
{
    QTest::addColumn<QString>("filename");
    QTest::addColumn<QString>("content");
    QTest::addColumn<QStringList>("expChars");
    QTest::addColumn<int>("detectiveIdx");

    QTest::newRow("0") << "characters0.txt"
                       << ""
                       << QStringList()
                       << -1; // No dectective

    QTest::newRow("1") << "characters1.txt"
                       << "Pedro\nFlorencia"
                       << (QStringList() << "Pedro" << "Florencia")
                       << -1;  // No dectective

    QTest::newRow("2") << "characters2.txt"
                       << QString::fromUtf8("Pedro\nAndr\xc3\xa9s\n\n")
                       << (QStringList() << "Pedro" << QString::fromUtf8("Andr\xc3\xa9s"))
                       << -1;  // No dectective

    QTest::newRow("3") << "characters3.txt"
                       << "   \nPedro\nFlorencia  \n\n\nDomecq*\n\n\n"
                       << (QStringList() << "Pedro" << "Florencia" << "Domecq")
                       << 2;

    QTest::newRow("4") << "" // Default filename
                       << "   \nPedro\nFlorencia  \n\n\nDomecq*\n\n\n"
                       << (QStringList() << "Pedro" << "Florencia" << "Domecq")
                       << 2;
}

//--------------------------------------------------------------------------------------------------


QTEST_APPLESS_MAIN(ScriptManagerUnitTest);


#include "scriptmanagerunittest.moc"
