#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    clear();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::clear()
{
    // reset active tabs
    ui->mainTabWidget->setCurrentIndex(0);
    ui->rightSideTabWidget->setCurrentIndex(0);

    // train tab widgets
    ui->categoriesTree->clear();
    ui->ruleInputText->clear();
    ui->ruleOutputText->clear();

    // chat tab widgets
    //ui->fbChatRadio->
    ui->usernameText->clear();
    ui->passwordText->clear();

    // conversation tab widgets
    ui->conversationContactsTable->clear();
    ui->conversationContactsTable->setRowCount(0);
    ui->conversationTable->clear();
    ui->conversationTable->setRowCount(0);
    ui->conversationTable->setColumnWidth(0, 60);
    ui->conversationTable->setColumnWidth(1, 300);

    // help tab widgets
    ui->helpLabel->clear();

    // test tab widgets
    ui->testConversationText->clear();
    ui->testInputText->clear();
}
