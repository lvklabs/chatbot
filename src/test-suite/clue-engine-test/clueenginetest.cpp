#include <QtCore/QString>
#include <QtTest/QtTest>

#include "common/globalstrings.h"
#include "common/settings.h"
#include "common/settingskeys.h"
#include "nlp-engine/rule.h"
#include "nlp-engine/globaltools.h"
#include "nlp-engine/lemmatizerfactory.h"
#include "da-clue/clueengine.h"
#include "da-clue/script.h"
#include "da-clue/analyzedscript.h"
#include "da-clue/scripterror.h"

#define UTF8    QString::fromUtf8

Q_DECLARE_METATYPE(Lvk::Nlp::RuleId)
Q_DECLARE_METATYPE(Lvk::Nlp::Rule)
Q_DECLARE_METATYPE(Lvk::Nlp::RuleList)
Q_DECLARE_METATYPE(Lvk::Clue::ScriptLine)
Q_DECLARE_METATYPE(Lvk::Clue::Script)
Q_DECLARE_METATYPE(Lvk::Clue::AnalyzedLine)
Q_DECLARE_METATYPE(Lvk::Clue::AnalyzedScript)
Q_DECLARE_METATYPE(Lvk::Clue::ScriptList)
Q_DECLARE_METATYPE(Lvk::Clue::ScriptError)

using namespace Lvk;

//--------------------------------------------------------------------------------------------------
// QTest
//--------------------------------------------------------------------------------------------------

namespace QTest
{

template<>
char * toString(const Lvk::Clue::AnalyzedLine& l)
{
    QByteArray ba = QByteArray("\tAnalyzedLine(")
            + "\n\t\t"  + l.question.toUtf8()
            + ",\n\t\t" + l.expAnswer.toUtf8()
            + ",\n\t\t" + l.forbidAnswer.toUtf8()
            + ",\n\t\t" + l.expHint.toUtf8()
            + ",\n\t\t" + l.forbidHint.toUtf8()
            + ",\n\t\t" + l.answer.toUtf8()
            + ",\n\t\t" + (l.importance == Clue::ScriptLine::Critical ? "CRITICAL" : "STANDARD")
            + ",\n\t\t" + QByteArray::number(l.ruleId)
            + ", "      + QByteArray::number(l.inputIdx)
            + ", "      + QByteArray::number(l.score)
            + ", "      + QByteArray::number(l.outputIdx)
            + ", "      + QByteArray::number(l.status)
            + "\n\t)\n";

    return qstrdup(ba.data());
}

//--------------------------------------------------------------------------------------------------

template<>
char * toString(const Lvk::Clue::AnalyzedScript& s)
{
    QByteArray ba = QByteArray("\nAnalyzedScript(")
            + "\n\tfile: " + s.filename.toUtf8()
            + "\n\tchar: " + s.character.toUtf8()
            + "\n\tnumb: " + QByteArray::number(s.number)
            + "\n\tcov : " + QByteArray::number(s.coverage)
            + "\n";

    foreach (const Lvk::Clue::AnalyzedLine &l, s) {
        char * str = toString(l);
        ba += str;
        ba + ",\n";
        delete[] str;
    }

    ba += ")\n";

    return qstrdup(ba.data());
}

}

//--------------------------------------------------------------------------------------------------
// ClueEngineTest
//--------------------------------------------------------------------------------------------------

class ClueEngineTest : public QObject
{
    Q_OBJECT

public:
    ClueEngineTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase1_data();
};

//--------------------------------------------------------------------------------------------------

ClueEngineTest::ClueEngineTest()
{
}

//--------------------------------------------------------------------------------------------------

void ClueEngineTest::initTestCase()
{
    Cmn::Settings().setValue(SETTING_APP_LANGUAGE, "es_AR");

    Nlp::GlobalTools::instance()->setLemmatizer(Nlp::LemmatizerFactory().createLemmatizer());
}

//--------------------------------------------------------------------------------------------------

void ClueEngineTest::cleanupTestCase()
{
}

//--------------------------------------------------------------------------------------------------

void ClueEngineTest::testCase1()
{
    QFETCH(Nlp::RuleList, rules);
    QFETCH(Clue::Script, script);
    QFETCH(Clue::AnalyzedScript, expAscript);

    Clue::ClueEngine engine;
    Clue::AnalyzedScript ascript;

    engine.setRules(rules);
    engine.analyze(script, ascript);

    QCOMPARE(ascript, expAscript);

    QCOMPARE(ascript.number,    script.number);
    QCOMPARE(ascript.filename,  script.filename);
    QCOMPARE(ascript.character, script.character);
}

//--------------------------------------------------------------------------------------------------

void ClueEngineTest::testCase1_data()
{
    //************************************************************************************
    // files
    QString f1 = "pedro1." SCRIPT_FILE_EXT;
    QString f2 = "pedro4." SCRIPT_FILE_EXT;

    // names
    QString n1 = "pedro";

    // questions
    QString q1 = "Que estaba haciendo la noche del 31 de marzo?";
    QString q2 = UTF8("Por qu\xc3\xa9 no respondi\xc3\xb3 el llamado?");
    QString q3 = "Y la noche del 1 de abril?";

    // expected answers
    QString ea1 = "*durmiendo* | *descanzando*";
    QString ea2 = "* sarasa *";
    QString ea3 = "* sarasa *";

    // forbidden answers
    QString fa1 = "";
    QString fa2 = "*odiaba* | *detestaba*";
    QString fa3 = "";

    // expected hints
    QString eh1 = "Recordar que a Jorge le gusta dormir.";
    QString eh2 = "No puede admitir que la odiaba!";
    QString eh3 = "";

    // forbidden hints
    QString fh1 = "";
    QString fh2 = "No mencionar el arma!";
    QString fh3 = "No mencionar su pareja!";

    // lines
    Clue::ScriptLine l1(q1, ea1, fa1, eh1, fh1, Clue::ScriptLine::Standard);
    Clue::ScriptLine l2(q2, ea2, fa2, eh2, fh2, Clue::ScriptLine::Critical);
    Clue::ScriptLine l3(q3, ea3, fa3, eh3, fh3, Clue::ScriptLine::Standard);

    // parsed scripts
    Clue::Script s1(f1, n1, 1);
    s1.append(l1);

    Clue::Script s2(f2, n1, 2);
    s2.append(l1);
    s2.append(l2);
    s2.append(l3);

    // answers
    QString answ1a = "Estaba durmiendo!";
    QString answ1b = "Estaba descanzando";
    QString answ2a = "sarasa";
    QString answ2b = "no se";
    QString answ2c = "Odiaba sarasa";

    // rules
    Nlp::RuleList rl1;
    rl1.append(Nlp::Rule(1, QStringList() << "Hola *",              QStringList() << "Hola!"));
    Nlp::RuleList rl2;
    rl2.append(Nlp::Rule(1, QStringList() << q1,                    QStringList() << answ1a));
    rl2.append(Nlp::Rule(2, QStringList() << q2,                    QStringList() << answ2b));
    rl2.append(Nlp::Rule(3, QStringList() << "Y la noche de abril", QStringList() << answ2a));
    Nlp::RuleList rl3;
    rl3.append(Nlp::Rule(1, QStringList() << q1,                    QStringList() << answ1b));
    rl3.append(Nlp::Rule(2, QStringList() << q2,                    QStringList() << answ2a));
    rl3.append(Nlp::Rule(3, QStringList() << q3,                    QStringList() << answ2a));
    Nlp::RuleList rl4;
    rl4.append(Nlp::Rule(1, QStringList() << q1,                    QStringList() << answ1b));
    rl4.append(Nlp::Rule(2, QStringList() << q2,                    QStringList() << answ2c));

    // analyzed scripts
    Clue::AnalyzedScript as1(f1, n1, 1);
    as1.append(Clue::AnalyzedLine(l1, 0, -1, 0, "",     -1, Clue::NoAnswerFound));
    as1.coverage = 0.0/as1.size()*100;
    Clue::AnalyzedScript as2(f2, n1, 2);
    as2.append(Clue::AnalyzedLine(l1, 1,  0, 0, answ1a,  0, Clue::AnswerOk));
    as2.append(Clue::AnalyzedLine(l2, 2,  0, 0, answ2b, -1, Clue::MismatchExpectedAnswer));
    as2.append(Clue::AnalyzedLine(l3, 0, -1, 0, "",     -1, Clue::NoAnswerFound));
    as2.coverage = 1.0/as2.size()*100;
    Clue::AnalyzedScript as3(f2, n1, 2);
    as3.append(Clue::AnalyzedLine(l1, 1,  0, 0, answ1b,  0, Clue::AnswerOk));
    as3.append(Clue::AnalyzedLine(l2, 2,  0, 0, answ2a,  0, Clue::AnswerOk));
    as3.append(Clue::AnalyzedLine(l3, 3,  0, 0, answ2a,  0, Clue::AnswerOk));
    as3.coverage = 3.0/as3.size()*100;
    Clue::AnalyzedScript as4(f2, n1, 2);
    as4.append(Clue::AnalyzedLine(l1, 1,  0, 0, answ1b,  0, Clue::AnswerOk));
    as4.append(Clue::AnalyzedLine(l2, 2,  0, 0, answ2c, -1, Clue::MatchForbiddenAnswer));
    as4.append(Clue::AnalyzedLine(l3, 0, -1, 0, "",     -1, Clue::NoAnswerFound));
    as4.coverage = 1.0/as4.size()*100;

    //************************************************************************************

    QTest::addColumn<Nlp::RuleList>("rules");
    QTest::addColumn<Clue::Script>("script");
    QTest::addColumn<Clue::AnalyzedScript>("expAscript");

    QTest::newRow("0") << rl1 << s1 << as1;
    QTest::newRow("1") << rl2 << s2 << as2;
    QTest::newRow("2") << rl3 << s2 << as3;
    QTest::newRow("3") << rl4 << s2 << as4;
}

//--------------------------------------------------------------------------------------------------

QTEST_APPLESS_MAIN(ClueEngineTest);

#include "clueenginetest.moc"
