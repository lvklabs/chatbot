#include <QtCore/QString>
#include <QtTest/QtTest>

#include "csvrow.h"
#include "csvdocument.h"

Q_DECLARE_METATYPE(Lvk::Cmn::CsvRow)
Q_DECLARE_METATYPE(Lvk::Cmn::CsvDocument)

//--------------------------------------------------------------------------------------------------
// CsvDocumentTest
//--------------------------------------------------------------------------------------------------

class CsvDocumentTest : public QObject
{
    Q_OBJECT

public:
    CsvDocumentTest();

private Q_SLOTS:

    void testToFromStrings_data();
    void testToFromStrings();
};

//--------------------------------------------------------------------------------------------------

CsvDocumentTest::CsvDocumentTest()
{
}

//--------------------------------------------------------------------------------------------------

#define COMMA   ","
#define EOL     "\n"
#define CELL_1  "apple"
#define CELL_2  "orange"
#define CELL_3  "pineapple"
#define CELL_4  "  shiny green apple  "
#define CELL_5  "  apple  "
#define CELL_6  ""
#define CELL_7  "\"apple,orange\""
#define CELL_8  "\"strawberry,blackberry,plumb\""
#define CELL_9  "\"long sentence,\"\"with\"\",\"\"lot\"\" of commas and quotes,,,\""

// unescaped
#define CELL_7u "apple,orange"
#define CELL_8u "strawberry,blackberry,plumb"
#define CELL_9u "long sentence,\"with\",\"lot\" of commas and quotes,,,"

void CsvDocumentTest::testToFromStrings_data()
{
    QTest::addColumn<QString>("csvString");
    QTest::addColumn<Lvk::Cmn::CsvDocument>("expectedCsvDoc");

    Lvk::Cmn::CsvRow doc1row1;
    Lvk::Cmn::CsvRow doc2row1;
    Lvk::Cmn::CsvRow doc2row2;
    Lvk::Cmn::CsvRow doc3row1;
    Lvk::Cmn::CsvRow doc3row2;
    Lvk::Cmn::CsvRow doc3row3;

    Lvk::Cmn::CsvDocument emptyDoc;
    Lvk::Cmn::CsvDocument doc1;
    Lvk::Cmn::CsvDocument doc2;
    Lvk::Cmn::CsvDocument doc3;

    doc1row1.append(CELL_1);

    doc1.append(doc1row1);

    doc2row1.append(CELL_1);
    doc2row1.append(CELL_2);
    doc2row1.append(CELL_3);
    doc2row2 = Lvk::Cmn::CsvRow(QStringList() << CELL_4  << CELL_5  << CELL_6);

    doc2.append(doc2row1);
    doc2.append(doc2row2);

    doc3row1 = Lvk::Cmn::CsvRow(QStringList() << CELL_1  << CELL_2  << CELL_3);
    doc3row2 = Lvk::Cmn::CsvRow(QStringList() << CELL_4  << CELL_5  << CELL_6);
    doc3row3 = Lvk::Cmn::CsvRow(QStringList() << CELL_7u << CELL_8u << CELL_9u);

    doc3.append(doc3row1);
    doc3.append(doc3row2);
    doc3.append(doc3row3);

    QTest::newRow("empty doc") << ""                                            << emptyDoc;

    QTest::newRow("doc 1")     << CELL_1                                        << doc1;

    QTest::newRow("doc 2")     << CELL_1 COMMA CELL_2 COMMA CELL_3 EOL\
                                  CELL_4 COMMA CELL_5 COMMA CELL_6              << doc2;

    QTest::newRow("doc 3")     << CELL_1 COMMA CELL_2 COMMA CELL_3 EOL\
                                  CELL_4 COMMA CELL_5 COMMA CELL_6 EOL\
                                  CELL_7 COMMA CELL_8 COMMA CELL_9              << doc3;
}

//--------------------------------------------------------------------------------------------------

void CsvDocumentTest::testToFromStrings()
{
    QFETCH(QString, csvString);
    QFETCH(Lvk::Cmn::CsvDocument, expectedCsvDoc);

    Lvk::Cmn::CsvDocument csvDoc(csvString);

    QCOMPARE(csvDoc, expectedCsvDoc);
    QCOMPARE(csvDoc.toString(), csvString);
}

//--------------------------------------------------------------------------------------------------

QTEST_APPLESS_MAIN(CsvDocumentTest)

#include "csvdocumenttest.moc"
