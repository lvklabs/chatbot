#include <QtTest/QtTest>
#include <QApplication>
#include <QThread>
#include "testmainwindow.h"
//#include "testmainwindow.moc"

//--------------------------------------------------------------------------------------------------
// TestThread class
//--------------------------------------------------------------------------------------------------

// Run TestMainWindow on new thread
class TestThread : public QThread
{
public:
    void run()
    {
        QTest::qSleep(1000);
        TestMainWindow tmw;
        int rc = QTest::qExec(&tmw, 0, 0);
        qApp->exit(rc);
    }
};

//--------------------------------------------------------------------------------------------------
// Test entry point
//--------------------------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // NOTE: The test needs an event loop, this tricks works but there must be a better way
    //TestMainWindow tmw;
    //return QTest::qExec(&tmw, argc, argv);

    TestThread tt;
    tt.start();

    return app.exec();
}


