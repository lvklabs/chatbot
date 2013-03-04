#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QFile>
#include <QDir>

#include "da-clue/scriptmanager.h"
#include "common/settingskeys.h"
#include "common/settings.h"

#define UTF8    QString::fromUtf8

Q_DECLARE_METATYPE(Lvk::Clue::ScriptLine)
Q_DECLARE_METATYPE(Lvk::Clue::Script)
Q_DECLARE_METATYPE(Lvk::Clue::ScriptList)

using namespace Lvk;


//--------------------------------------------------------------------------------------------------
// Script file format
//--------------------------------------------------------------------------------------------------

const QString XML_SCRIPT =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<SCRIPT>\n"
          "<HEADER>\n%1\n</HEADER>\n"
          "<BODY>\n%2\n</BODY>\n"
        "</SCRIPT>\n";

const QString XML_HEADER =
        "<CHARACTER>%1</CHARACTER>"
        "<SCRIPTNUMBER>%2</SCRIPTNUMBER>";

const QString XML_QUESTION =
        "<QUESTION>"
          "<PHRASE>%1</PHRASE>"
          "<EXPECTEDANSWERS>%2</EXPECTEDANSWERS>"
          "<FORBIDDENANSWERS>%3</FORBIDDENANSWERS>"
          "<IMPORTANCE>%4</IMPORTANCE>"
          "<HINT>%5</HINT>"
        "</QUESTION>\n";

const QString XML_STANDARD = "STANDARD";
const QString XML_CRITICAL = "CRITICAL";

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

    void testLoadScripts();
    void testLoadScripts_data();

private:
    QString m_clueDir;

    void rmAllScripts();
    void mkFile(const QString &filename, const QString &content);
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

    mkFile(filename, content);

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
                       << "   \r\nPedro\r\nFlorencia  \r\n\r\n\r\nDomecq*\r\n\r\n\r\n"
                       << (QStringList() << "Pedro" << "Florencia" << "Domecq")
                       << 2;
}

//--------------------------------------------------------------------------------------------------

void ScriptManagerUnitTest::testLoadScripts()
{
    QFETCH(QStringList, filenames);
    QFETCH(QStringList, contents);
    QFETCH(QString, character);
    QFETCH(Clue::ScriptList, expScripts);

    QCOMPARE(filenames.size(), contents.size());

    rmAllScripts();

    for (int i = 0; i < filenames.size(); ++i) {
        mkFile(filenames[i], contents[i]);
    }

    Clue::ScriptManager mgr;

    QVERIFY(mgr.loadScriptsForCharacter(character));
    QCOMPARE(mgr.error(), Clue::NoError);

    Clue::ScriptList scripts = mgr.scripts();

    QCOMPARE(scripts.size(), expScripts.size());
    for (int i = 0; i < scripts.size(); ++i) {
        QCOMPARE(scripts[i].size(), expScripts[i].size());
        for (int j = 0; j < scripts[i].size(); ++j) {
            QCOMPARE(scripts[i][j], expScripts[i][j]);
        }
    }

    mgr.clear();

    QVERIFY(mgr.scripts().isEmpty());
}

//--------------------------------------------------------------------------------------------------

void ScriptManagerUnitTest::testLoadScripts_data()
{
    //************************************************************************************
    // files
    QString f1 = "pedro1.txt";
    QString f2 = "script_pedro2.txt";

    // names
    QString n1 = "pedro";

    // questions
    QString q1 = "Que estaba haciendo la noche del 31 de marzo?";
    QString q2 = UTF8("Por qu\xc3\xa9 no respondi\xc3\xb3 el llamado?");

    // expected answers
    QString ea1 = "*durmiendo* | *descanzando*";
    QString ea2 = "*";

    // forbidden answers
    QString fa1 = "";
    QString fa2 = "*odiaba* | *detestaba*";

    // hints
    QString h1 = "Recordar que a Jorge le gusta dormir.";
    QString h2 = "No puede admitir que la odiaba!";

    // file contents
    QString c1 = XML_SCRIPT.arg(XML_HEADER.arg(n1, "1"),
                                XML_QUESTION.arg(q1, ea1, fa1, XML_STANDARD, h1));
    QString c2 = XML_SCRIPT.arg(XML_HEADER.arg(n1, "2"),
                                XML_QUESTION.arg(q1, ea1, fa1, XML_STANDARD, h1) +
                                XML_QUESTION.arg(q2, ea2, fa2, XML_CRITICAL, h2));

    // parsed scripts
    Clue::Script s1(f1, n1, 1);
    s1.append(Clue::ScriptLine(q1, ea1, fa1, h1, Clue::ScriptLine::Standard));
    Clue::Script s2(f2, n1, 2);
    s2.append(Clue::ScriptLine(q1, ea1, fa1, h1, Clue::ScriptLine::Standard));
    s2.append(Clue::ScriptLine(q2, ea2, fa2, h2, Clue::ScriptLine::Critical));
    //************************************************************************************

    QTest::addColumn<QStringList>("filenames");
    QTest::addColumn<QStringList>("contents");
    QTest::addColumn<QString>("character");
    QTest::addColumn<Clue::ScriptList>("expScripts");

    QTest::newRow("0") << QStringList()
                       << QStringList()
                       << n1
                       << Clue::ScriptList();

    QTest::newRow("1") << (QStringList() << f1)
                       << (QStringList() << c1)
                       << n1
                       << (Clue::ScriptList() << s1);

    QTest::newRow("2") << (QStringList() << f1 << f2)
                       << (QStringList() << c1 << c2)
                       << n1
                       << (Clue::ScriptList() << s1 << s2);
}

//--------------------------------------------------------------------------------------------------

void ScriptManagerUnitTest::mkFile(const QString &filename, const QString &content)
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

void ScriptManagerUnitTest::rmAllScripts()
{
    QDir dir(m_clueDir);
    QStringList nameFilters;
    nameFilters.append("*pedro*");
    nameFilters.append("*florencia*");
    QStringList files = dir.entryList(nameFilters, QDir::Files, QDir::Name);

    foreach (const QString &file, files) {
        QVERIFY(QFile::remove(m_clueDir + "/" + file));
    }
}

//--------------------------------------------------------------------------------------------------

QTEST_APPLESS_MAIN(ScriptManagerUnitTest);

#include "scriptmanagerunittest.moc"
