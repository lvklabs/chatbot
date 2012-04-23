/*
 * Copyright (C) 2012 Andres Pagliano, Gabriel Miretti, Gonzalo Buteler,
 * Nestor Bustamante, Pablo Perez de Angelis
 *
 * This file is part of LVK Botmaster.
 *
 * LVK Botmaster is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * LVK Botmaster is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LVK Botmaster.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "testmainwindow.h"
#include "mainwindow.h"
#include "rule.h"
#include "ruletreemodel.h"
#include "ui_mainwindow.h"
#include "testxmppclient.h"

#include <QString>
#include <QtTest/QtTest>
#include <QHash>
#include <QRegExp>

#include <iostream>

//--------------------------------------------------------------------------------------------------
// Test data
//--------------------------------------------------------------------------------------------------

// Rules -------------------------------------------------------------------------------------------

#define CATEGORY_1_NAME                     "Greetings"
#define CATEGORY_2_NAME                     "Likes/Dislikes"

#define RULE_1_INPUT_1                      "Hello"
#define RULE_1_INPUT_2                      "Hi"
#define RULE_1_INPUT_3                      "Hello *"
#define RULE_1_OUTPUT_1                     "Hi!"
#define RULE_1_OUTPUT_2                     "Hello!"
#define RULE_1_OUTPUT_3                     "Hey!"

#define RULE_2_INPUT_1                      "What is your name?"
#define RULE_2_OUTPUT_1                     "R2D2"

#define RULE_3_INPUT_1                      "cars"
#define RULE_3_INPUT_2                      "cars _"
#define RULE_3_INPUT_3                      "_ cars"
#define RULE_3_INPUT_4                      "_ cars *"
#define RULE_3_OUTPUT_1                     "I love cars"

#define RULE_4_INPUT_1                      "Do you like *"
#define RULE_4_OUTPUT_1                     "<srai><star/></srai>"

#define RULE_5_INPUT_1                      "cats"
#define RULE_5_OUTPUT_1                     "I hate cats!"

#define RULE_EVASIVE_1                      "I don't understand"
#define RULE_EVASIVE_2                      "Could you say that in another way?"

// User inputs -------------------------------------------------------------------------------------

#define USER_INPUT_1a                       "Hello"
#define USER_INPUT_1b                       "hello"
#define USER_INPUT_1c                       "HELLO"
#define USER_INPUT_2                        "Hi"
#define USER_INPUT_3                        "Hey there!"
#define USER_INPUT_4a                       "What is your name?"
#define USER_INPUT_4b                       "What is your name"
#define USER_INPUT_5                        "Hello there!"
#define USER_INPUT_6                        "Hello how are you!!"
#define USER_INPUT_7a                       "cars"
#define USER_INPUT_7b                       "Do you like cars?"
#define USER_INPUT_7c                       "cars are the best!"
#define USER_INPUT_7d                       "Have you seen the latest cars that BMW have launched?"
#define USER_INPUT_8a                       "Do you like cats?"
#define USER_INPUT_8b                       "Do you like robots?"

// Chat accounts -----------------------------------------------------------------------------------

#define GMAIL_DOMAIN                        "gmail.com"
#define FB_CHAT_DOMAIN                      "chat.facebook.com"

#define GMAIL_USER_1                        "andres.xmpp"
#define GMAIL_USER_1_PASSWD_OK              "xmpp1234"
#define GMAIL_USER_1_PASSWD_WRONG           "xxx"

#define GMAIL_USER_2                        "andres.test2"
#define GMAIL_USER_2_PASSWD_OK              "andresTEST"
#define GMAIL_USER_2_PASSWD_WRONG           "xxx"

#define FB_USER_1                           "andres.pagliano" // FIXME create new account
#define FB_USER_1_PASSWD_OK                 "FIXME"
#define FB_USER_1_PASSWD_WRONG              "xxx"

// Misc --------------------------------------------------------------------------------------------

#define CONVERSATION_ENTRY                  "You: %1\nChatbot: %2\n"
#define CONVERSATION_NOT_OUTPUT_REGEX       "You: .*\nChatbot:"

#define CHAT_CONNECTION_OK_TOKEN            "sucessful"
#define CHAT_CONNECTION_FAILED_TOKEN        "error"
#define CHAT_CONNECTING_TOKEN               "connecting"
#define CHAT_DISCONNECTION_TOKEN            "disconnected"




//--------------------------------------------------------------------------------------------------
// TestMainWindow definition
//--------------------------------------------------------------------------------------------------

void (* TestMainWindow::m_defaultMsgHandler)(QtMsgType, const char *) = 0;

//--------------------------------------------------------------------------------------------------

TestMainWindow::TestMainWindow()
    : m_window(0)
{
}

//--------------------------------------------------------------------------------------------------

void TestMainWindow::noWarningsMsgHandler(QtMsgType type, const char *msg)
{
    switch (type) {
    case QtWarningMsg:
        // Do not show warnings messages
        break;
    default:
        if (m_defaultMsgHandler) {
            m_defaultMsgHandler(type, msg);
        }
        break;
    }
}

//--------------------------------------------------------------------------------------------------

void TestMainWindow::initTestCase()
{
    m_defaultMsgHandler = qInstallMsgHandler(TestMainWindow::noWarningsMsgHandler);
}

//--------------------------------------------------------------------------------------------------

void TestMainWindow::cleanupTestCase()
{
    qInstallMsgHandler(0);
}

//--------------------------------------------------------------------------------------------------

void TestMainWindow::init()
{
    m_window = new Lvk::FE::MainWindow();
}

//--------------------------------------------------------------------------------------------------

void TestMainWindow::cleanup()
{
    delete m_window;
    m_window = 0;
}

//--------------------------------------------------------------------------------------------------

void TestMainWindow::UiSetRuleHierarchy1()
{
    m_window->m_ruleTreeModel->clear();

    Lvk::BE::Rule *rootRule = m_window->m_coreApp->rootRule();

    Lvk::BE::Rule *cat1 = new Lvk::BE::Rule(CATEGORY_1_NAME, Lvk::BE::Rule::ContainerRule,rootRule);
    Lvk::BE::Rule *cat2 = new Lvk::BE::Rule(CATEGORY_2_NAME, Lvk::BE::Rule::ContainerRule,rootRule);

    Lvk::BE::Rule *rule1 = new Lvk::BE::Rule("",
                             QStringList() << RULE_1_INPUT_1 << RULE_1_INPUT_2 << RULE_1_INPUT_3,
                             QStringList() << RULE_1_OUTPUT_1,
                             cat1);

    Lvk::BE::Rule *rule2 = new Lvk::BE::Rule("",
                             QStringList() << RULE_2_INPUT_1,
                             QStringList() << RULE_2_OUTPUT_1,
                             cat1);

    Lvk::BE::Rule *rule3 = new Lvk::BE::Rule("",
                             QStringList() << RULE_3_INPUT_1 << RULE_3_INPUT_2 << RULE_3_INPUT_3
                                           << RULE_3_INPUT_4,
                             QStringList() << RULE_3_OUTPUT_1,
                             cat2);

    Lvk::BE::Rule *rule4 = new Lvk::BE::Rule("",
                             QStringList() << RULE_4_INPUT_1,
                             QStringList() << RULE_4_OUTPUT_1,
                             cat2);

    Lvk::BE::Rule *rule5 = new Lvk::BE::Rule("",
                             QStringList() << RULE_5_INPUT_1,
                             QStringList() << RULE_5_OUTPUT_1,
                             cat2);

    Lvk::BE::Rule *evasives = new Lvk::BE::Rule("", Lvk::BE::Rule::EvasiveRule, rootRule);

    evasives->setOutput(QStringList() << RULE_EVASIVE_1);

    m_window->m_ruleTreeModel->appendItem(cat1);
    m_window->m_ruleTreeModel->appendItem(cat2);

    m_window->m_ruleTreeModel->appendItem(rule1);
    m_window->m_ruleTreeModel->appendItem(rule2);
    m_window->m_ruleTreeModel->appendItem(rule3);
    m_window->m_ruleTreeModel->appendItem(rule4);
    m_window->m_ruleTreeModel->appendItem(rule5);

    m_window->m_ruleTreeModel->appendItem(evasives);

    m_window->m_coreApp->refreshNlpEngine();
}

//--------------------------------------------------------------------------------------------------

void TestMainWindow::UiSetRuleHierarchy2()
{
    m_window->m_ruleTreeModel->clear();

    Lvk::BE::Rule *rootRule = m_window->m_coreApp->rootRule();

    Lvk::BE::Rule *cat1 = new Lvk::BE::Rule(CATEGORY_1_NAME, Lvk::BE::Rule::ContainerRule, rootRule);

    Lvk::BE::Rule *rule1 = new Lvk::BE::Rule("",
                             QStringList() << RULE_1_INPUT_1 << RULE_1_INPUT_2 << RULE_1_INPUT_3,
                             QStringList() << RULE_1_OUTPUT_1 << RULE_1_OUTPUT_2 << RULE_1_OUTPUT_3,
                             cat1);

    Lvk::BE::Rule *evasives = new Lvk::BE::Rule("", Lvk::BE::Rule::EvasiveRule, rootRule);

    evasives->setOutput(QStringList() << RULE_EVASIVE_1 << RULE_EVASIVE_2);

    m_window->m_ruleTreeModel->appendItem(cat1);
    m_window->m_ruleTreeModel->appendItem(rule1);
    m_window->m_ruleTreeModel->appendItem(evasives);

    m_window->m_coreApp->refreshNlpEngine();
}

//--------------------------------------------------------------------------------------------------

void TestMainWindow::UiConnect(const QString &username, const QString &password, int chatType)
{
    QTest::keyClicks(m_window->ui->usernameText, username);
    QTest::keyClicks(m_window->ui->passwordText, password);

    if (chatType == Facebook) {
        m_window->ui->fbChatRadio->setChecked(true);
    } else if (chatType == Gmail){
        m_window->ui->gtalkChatRadio->setChecked(true);
    }

    m_window->onConnectButtonPressed();
}

//--------------------------------------------------------------------------------------------------

void TestMainWindow::UiWaitForConnection()
{
    QLabel *statusLabel = m_window->ui->connectionStatusLabel;

    int t = 0;
    const int CONNECTION_TIMEOUT = 20*1000;

    while (statusLabel->text().contains(CHAT_CONNECTING_TOKEN, false) && t < CONNECTION_TIMEOUT) {
        QTest::qSleep(500);
        t += 500;

        //http://stackoverflow.com/questions/6433933/qtcpclient-successfully-connects-but-not-to-my-
        //server-where-is-it-connecting
        qApp->processEvents();
    }
}

//--------------------------------------------------------------------------------------------------

void TestMainWindow::testTestTabConversationSingleOutput_data()
{
    QTest::addColumn<QString>("userInput");
    QTest::addColumn<QString>("expectedOutput");

    // test case insensitive exact match
    QTest::newRow("exact match 1") << USER_INPUT_1a << RULE_1_OUTPUT_1;
    QTest::newRow("exact match 2") << USER_INPUT_1b << RULE_1_OUTPUT_1;
    QTest::newRow("exact match 3") << USER_INPUT_1c << RULE_1_OUTPUT_1;
    QTest::newRow("exact match 4") << USER_INPUT_2  << RULE_1_OUTPUT_1;
    QTest::newRow("exact match 5") << USER_INPUT_3  << RULE_EVASIVE_1;
    QTest::newRow("exact match 6") << USER_INPUT_4a << RULE_2_OUTPUT_1;

    // test AIML match
    QTest::newRow("AIML 1")        << USER_INPUT_4b << RULE_2_OUTPUT_1;
    QTest::newRow("AIML 2")        << USER_INPUT_5  << RULE_1_OUTPUT_1;
    QTest::newRow("AIML 3")        << USER_INPUT_6  << RULE_1_OUTPUT_1;
    QTest::newRow("AIML 4")        << USER_INPUT_7a << RULE_3_OUTPUT_1;
    QTest::newRow("AIML 5")        << USER_INPUT_7b << RULE_3_OUTPUT_1;
    QTest::newRow("AIML 6")        << USER_INPUT_7c << RULE_3_OUTPUT_1;
    QTest::newRow("AIML 7")        << USER_INPUT_7d << RULE_3_OUTPUT_1;
    QTest::newRow("AIML 8")        << USER_INPUT_8a << RULE_5_OUTPUT_1;
    QTest::newRow("AIML 9")        << USER_INPUT_8b << RULE_EVASIVE_1;
}

//--------------------------------------------------------------------------------------------------

void TestMainWindow::testTestTabConversationSingleOutput()
{
    QFETCH(QString, userInput);
    QFETCH(QString, expectedOutput);

    UiSetRuleHierarchy1();

    m_window->ui->testInputText->clear();
    m_window->ui->testConversationText->clear();

    QTest::keyClicks(m_window->ui->testInputText, userInput);

    m_window->onTestInputTextEntered();

    QString conversation = QString(CONVERSATION_ENTRY).arg(userInput, expectedOutput);

    QCOMPARE(m_window->ui->testConversationText->toPlainText(), conversation);
}

//--------------------------------------------------------------------------------------------------

void TestMainWindow::testTestTabConversationRandomOuput_data()
{
    QTest::addColumn<QString>("userInput");
    QTest::addColumn<QStringList>("expectedOutput");

    QStringList RULE_1_OUTPUT_LIST(QStringList() << RULE_1_OUTPUT_1 << RULE_1_OUTPUT_2
                                                 << RULE_1_OUTPUT_3);

    QStringList EVASIVES_OUTPUT_LIST(QStringList() << RULE_EVASIVE_1 << RULE_EVASIVE_2);

    // Case insensitive exact match
    QTest::newRow("exact match 1") << USER_INPUT_1a << RULE_1_OUTPUT_LIST;
    QTest::newRow("exact match 2") << USER_INPUT_1b << RULE_1_OUTPUT_LIST;
    QTest::newRow("exact match 3") << USER_INPUT_1c << RULE_1_OUTPUT_LIST;
    QTest::newRow("exact match 4") << USER_INPUT_2  << RULE_1_OUTPUT_LIST;
    QTest::newRow("exact match 5") << USER_INPUT_3  << EVASIVES_OUTPUT_LIST;

    // AIML
    QTest::newRow("AIML 1")        << USER_INPUT_5  << RULE_1_OUTPUT_LIST;
    QTest::newRow("AIML 2")        << USER_INPUT_6  << RULE_1_OUTPUT_LIST;

}

//--------------------------------------------------------------------------------------------------

void TestMainWindow::testTestTabConversationRandomOuput()
{
    QFETCH(QString, userInput);
    QFETCH(QStringList, expectedOutput);

    QHash<QString, int> outputUseCount;

    UiSetRuleHierarchy2();

    for (int i = 0; i < 50; ++i) {
        m_window->ui->testInputText->clear();
        m_window->ui->testConversationText->clear();

        QTest::keyClicks(m_window->ui->testInputText, userInput);

        m_window->onTestInputTextEntered();

        QString conversation = m_window->ui->testConversationText->toPlainText();
        QString output = conversation.remove(QRegExp(CONVERSATION_NOT_OUTPUT_REGEX)).trimmed();

        QVERIFY(expectedOutput.contains(output));

        outputUseCount[output] = outputUseCount[output] + 1;
    }

    QCOMPARE(outputUseCount.size(), expectedOutput.size());
}

//--------------------------------------------------------------------------------------------------

void TestMainWindow::testChatConnection_data()
{
    QTest::addColumn<QString>("username");
    QTest::addColumn<QString>("password");
    QTest::addColumn<int>("chatType");
    QTest::addColumn<bool>("valid");

    QTest::newRow("gmail fail 1") << GMAIL_USER_1 << GMAIL_USER_1_PASSWD_WRONG << (int)Gmail    << false;
    QTest::newRow("gmail ok 1")   << GMAIL_USER_1 << GMAIL_USER_1_PASSWD_OK    << (int)Gmail    << true;
    QTest::newRow("fb fail 1")    << FB_USER_1    << FB_USER_1_PASSWD_WRONG    << (int)Facebook << false;
    QTest::newRow("fb ok 1")      << FB_USER_1    << FB_USER_1_PASSWD_OK       << (int)Facebook << true;
}

//--------------------------------------------------------------------------------------------------

void TestMainWindow::testChatConnection()
{
    QFETCH(QString, username);
    QFETCH(QString, password);
    QFETCH(int, chatType);
    QFETCH(bool, valid);

    if (chatType == Facebook && valid) {
        QSKIP("", SkipSingle);
    }

    UiConnect(username, password, chatType);

    UiWaitForConnection();

    QLabel *chatStatusLabel = m_window->ui->connectionStatusLabel;

    //qDebug() << chatStatusLabel->text();

    if (valid) {
        QVERIFY(chatStatusLabel->text().contains(CHAT_CONNECTION_OK_TOKEN, false));

        QTest::qSleep(200);
        m_window->onConnectButtonPressed();  //disconnect

        QVERIFY(chatStatusLabel->text().contains(CHAT_DISCONNECTION_TOKEN, false));

    } else {
        QVERIFY(chatStatusLabel->text().contains(CHAT_CONNECTION_FAILED_TOKEN, false));
    }
}

//--------------------------------------------------------------------------------------------------

void TestMainWindow::testChatbotResponse_data()
{
    QTest::addColumn<QString>("user1");
    QTest::addColumn<QString>("passwd1");
    QTest::addColumn<QString>("user2");
    QTest::addColumn<QString>("passwd2");
    QTest::addColumn<int>("usersType");
    QTest::addColumn<QString>("domain");
    QTest::addColumn<QString>("msg");
    QTest::addColumn<QString>("expectedResponse");

    QTest::newRow("gmail 1")
            << GMAIL_USER_1
            << GMAIL_USER_1_PASSWD_OK
            << GMAIL_USER_2
            << GMAIL_USER_2_PASSWD_OK
            << (int)Gmail
            << GMAIL_DOMAIN
            << USER_INPUT_1a
            << RULE_1_OUTPUT_1;

    // TODO add test case for facebook
}

//--------------------------------------------------------------------------------------------------

void TestMainWindow::testChatbotResponse()
{
    std::cout << "Running testChatbotResponse()..." << std::endl;

    QFETCH(QString, user1);
    QFETCH(QString, passwd1);
    QFETCH(QString, user2);
    QFETCH(QString, passwd2);
    QFETCH(int, usersType);
    QFETCH(QString, domain);
    QFETCH(QString, msg);
    QFETCH(QString, expectedResponse);

    QString jid1 = user1 + "@" + domain;
    QString jid2 = user2 + "@" + domain;

    // Init rules

    UiSetRuleHierarchy1();

    // Connect user1 as chatbot
    std::cout << " - Connecting " << jid1.toStdString() << " as chatbot..." << std::endl;

    UiConnect(user1, passwd1, usersType);
    UiWaitForConnection();

    QVERIFY(m_window->ui->connectionStatusLabel->text().contains(CHAT_CONNECTION_OK_TOKEN, false));

    // Connect user2 as test xmpp client
    std::cout << " - Connecting " << jid2.toStdString() << " as client..." << std::endl;

    TestXmppClient xmmpClient;

    QVERIFY(xmmpClient.connectClient(jid2, passwd2));

    // Send message to chatbot and wait for response
    std::cout << " - Sending message from client to chatbot..." << std::endl;

    xmmpClient.sendMessage(jid1, msg);

    // Waiting for response
    std::cout << " - Waiting for response..." << std::endl;

    QString response;
    QVERIFY(xmmpClient.waitForResponse(response, 15*1000));

    QCOMPARE(response, expectedResponse);
}


