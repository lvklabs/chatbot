#ifndef TESTMAINWINDOW_H
#define TESTMAINWINDOW_H

#include <QObject>
#include <QtGlobal>

namespace Lvk
{

namespace FE
{

class MainWindow;

} // namespace FE

} // namespace Lvk


class TestMainWindow : public QObject
{
    Q_OBJECT

public:
    TestMainWindow();

    enum { Facebook, Gmail };

private Q_SLOTS:

    void initTestCase();
    void init();

    void testTestTabConversationSingleOutput_data();
    void testTestTabConversationSingleOutput();

    void testTestTabConversationRandomOuput_data();
    void testTestTabConversationRandomOuput();

    void testChatConnection_data();
    void testChatConnection();

    void testChatbotResponse_data();
    void testChatbotResponse();

    void cleanupTestCase();
    void cleanup();

private:
    Lvk::FE::MainWindow *m_window;

    void buildTestRuleHierarchy1();
    void buildTestRuleHierarchy2();

    static void (* m_defaultMsgHandler)(QtMsgType, const char *);
    static void noWarningsMsgHandler(QtMsgType type, const char *msg);
};

#endif // TESTMAINWINDOW_H
