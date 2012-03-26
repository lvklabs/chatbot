#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QFile>
#include <QString>

#include "defaultsanitizer.h"

#define TEST_DATA_FILE       "test_data.txt"
#define IO_SPLIT_TOKEN       "==="

//--------------------------------------------------------------------------------------------------

class testDefaultSanitizer : public QObject
{
    Q_OBJECT

public:
    testDefaultSanitizer() {}

private Q_SLOTS:
    void initTestCase() {}
    void cleanupTestCase() {}
    void testCase1_data();
    void testCase1();
};

//--------------------------------------------------------------------------------------------------

void testDefaultSanitizer::testCase1_data()
{
    QFile dataFile(TEST_DATA_FILE);

    if (!dataFile.open(QFile::ReadOnly)) {
        QFAIL("Cannot open test data file " TEST_DATA_FILE);
    }

    QString fileContent = QString::fromUtf8(dataFile.readAll());
    QStringList ioData = fileContent.split(IO_SPLIT_TOKEN, QString::SkipEmptyParts);

    QVERIFY2(ioData.size() == 2,
             "Invalid file format in test data file " TEST_DATA_FILE ". Error #1");

    QStringList inputList = ioData[0].split("\n", QString::SkipEmptyParts);
    QStringList outputList = ioData[1].split("\n", QString::SkipEmptyParts);

    QVERIFY2(inputList.size() == outputList.size(),
             "Invalid file format in test data file" TEST_DATA_FILE ". Error #2");

    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("expectedOutput");

    for (int i = 0; i < inputList.size(); ++i) {
        QTest::newRow(QString::number(i).toAscii()) << inputList[i] << outputList[i];
    }
}

//--------------------------------------------------------------------------------------------------

void testDefaultSanitizer::testCase1()
{
    QFETCH(QString, input);
    QFETCH(QString, expectedOutput);

    QString output = Lvk::Nlp::DefaultSanitizer().sanitize(input);

    QCOMPARE(output, expectedOutput);
}

//--------------------------------------------------------------------------------------------------

QTEST_APPLESS_MAIN(testDefaultSanitizer)

#include "testdefaultsanitizer.moc"
