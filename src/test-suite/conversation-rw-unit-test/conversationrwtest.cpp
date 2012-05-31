#include <QtCore/QString>
#include <QtTest/QtTest>

#include <QFile>
#include <QString>
#include <QList>

#include "conversation.h"
#include "conversationreader.h"
#include "conversationwriter.h"

typedef QList<Lvk::BE::Conversation::Entry> EntryList;

Q_DECLARE_METATYPE(Lvk::BE::Conversation)
Q_DECLARE_METATYPE(Lvk::BE::Conversation::Entry)
Q_DECLARE_METATYPE(EntryList)

//--------------------------------------------------------------------------------------------------
// ConversationRwTest
//--------------------------------------------------------------------------------------------------

class ConversationRwTest : public QObject
{
    Q_OBJECT

public:
    ConversationRwTest();

private Q_SLOTS:
    void testReadWriteConversation_data();
    void testReadWriteConversation();
    void testReadWriteConversationEntry_data();
    void testReadWriteConversationEntry();
};

//--------------------------------------------------------------------------------------------------

ConversationRwTest::ConversationRwTest()
{
}

//--------------------------------------------------------------------------------------------------

void ConversationRwTest::testReadWriteConversation_data()
{
    QTest::addColumn<Lvk::BE::Conversation>("conv");
    QTest::addColumn<bool>("ioDevCtor");    // Use QIODevice constructor

    Lvk::BE::Conversation conv1;
    Lvk::BE::Conversation conv2;

    Lvk::BE::Conversation::Entry entry1;
    entry1.dateTime = QDateTime::currentDateTime();
    entry1.from     = "user A";
    entry1.to       = "user B";
    entry1.msg      = "Hey there!";
    entry1.response = "Do I know you?";
    entry1.match    = true;

    Lvk::BE::Conversation::Entry entry2;
    entry2.dateTime = QDateTime::currentDateTime();
    entry2.from     = "user A";
    entry2.to       = "user B";
    entry2.msg      = "Of course!";
    entry2.response = "Really? From where?";
    entry2.match    = true;

    conv2.append(entry1);
    conv2.append(entry2);

    QTest::newRow("conv 1 - io") << conv1 << true;
    QTest::newRow("conv 1 - fn") << conv1 << false;
    QTest::newRow("conv 2 - io") << conv2 << true;
    QTest::newRow("conv 2 - fn") << conv2 << false;
}

//--------------------------------------------------------------------------------------------------


void ConversationRwTest::testReadWriteConversation()
{
    QFETCH(Lvk::BE::Conversation, conv);
    QFETCH(bool, ioDevCtor);

    const QString CONV_FILENAME = "chat_conv_test1.txt";

    if (QFile::exists(CONV_FILENAME)) {
        if (!QFile::remove(CONV_FILENAME)) {
            QFAIL("Cannot remove file with chat converation ");
        }
    }

    ////////////////////////// WRITE ////////////////////////////

    Lvk::BE::ConversationWriter *writer = 0;

    if (ioDevCtor) {
        writer = new Lvk::BE::ConversationWriter(new QFile(CONV_FILENAME));
    } else {
        writer = new Lvk::BE::ConversationWriter(CONV_FILENAME);
    }

    QVERIFY(writer->write(conv));

    delete writer;

    if (!QFile::exists(CONV_FILENAME)) {
        QFAIL("File not created");
    }

    ////////////////////////// READ ////////////////////////////

    Lvk::BE::Conversation convRead;
    Lvk::BE::ConversationReader *reader = 0;

    if (ioDevCtor) {
        reader = new Lvk::BE::ConversationReader(new QFile(CONV_FILENAME));
    } else {
        reader = new Lvk::BE::ConversationReader(CONV_FILENAME);
    }

    QVERIFY(reader->read(&convRead));

    QVERIFY(reader->atEnd());

    QCOMPARE(conv, convRead);

    delete reader;

    ////////////////////////// CLEAN ////////////////////////////

    QFile::remove(CONV_FILENAME);
}

//--------------------------------------------------------------------------------------------------


void ConversationRwTest::testReadWriteConversationEntry_data()
{
    QTest::addColumn<EntryList>("entries");
    QTest::addColumn<bool>("ioDevCtor");    // Use QIODevice constructor

    Lvk::BE::Conversation::Entry entry1;
    entry1.dateTime = QDateTime::currentDateTime();
    entry1.from     = "user A";
    entry1.to       = "user B";
    entry1.msg      = "Hey there!";
    entry1.response = "Do I know you?";
    entry1.match    = true;

    Lvk::BE::Conversation::Entry entry2;
    entry2.dateTime = QDateTime::currentDateTime();
    entry2.from     = "user A";
    entry2.to       = "user B";
    entry2.msg      = "Of course!";
    entry2.response = "Really? From where?";
    entry2.match    = false;

    QTest::newRow("entries 1 - io") << ( EntryList() << entry1 )           << true;
    QTest::newRow("entries 1 - fn") << ( EntryList() << entry1 )           << false;
    QTest::newRow("entries 2 - io") << ( EntryList() << entry1 << entry2 ) << true;
    QTest::newRow("entries 2 - fn") << ( EntryList() << entry1 << entry2 ) << false;
}

void ConversationRwTest::testReadWriteConversationEntry()
{
    QFETCH(EntryList, entries);
    QFETCH(bool, ioDevCtor);

    const QString CONV_FILENAME = "chat_conv_test12.txt";

    if (QFile::exists(CONV_FILENAME)) {
        if (!QFile::remove(CONV_FILENAME)) {
            QFAIL("Cannot remove file with chat converation ");
        }
    }

    ////////////////////////// WRITE ////////////////////////////

    Lvk::BE::ConversationWriter *writer = 0;

    if (ioDevCtor) {
        writer = new Lvk::BE::ConversationWriter(new QFile(CONV_FILENAME));
    } else {
        writer = new Lvk::BE::ConversationWriter(CONV_FILENAME);
    }

    foreach (const Lvk::BE::Conversation::Entry &entry, entries) {
        QVERIFY(writer->write(entry));
    }

    delete writer;

    if (!QFile::exists(CONV_FILENAME)) {
        QFAIL("File not created");
    }

    ////////////////////////// READ ////////////////////////////

    Lvk::BE::Conversation::Entry entryRead;
    Lvk::BE::ConversationReader *reader = 0;

    if (ioDevCtor) {
        reader = new Lvk::BE::ConversationReader(new QFile(CONV_FILENAME));
    } else {
        reader = new Lvk::BE::ConversationReader(CONV_FILENAME);
    }

    foreach (const Lvk::BE::Conversation::Entry &entry, entries) {
        QVERIFY(!reader->atEnd());
        QVERIFY(reader->read(&entryRead));
        QCOMPARE(entry, entryRead);
    }

    QVERIFY(reader->atEnd());

    delete reader;

    ////////////////////////// CLEAN ////////////////////////////

    QFile::remove(CONV_FILENAME);
}

//--------------------------------------------------------------------------------------------------

QTEST_APPLESS_MAIN(ConversationRwTest)

#include "conversationrwtest.moc"
