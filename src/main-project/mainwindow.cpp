#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->mainTabWidget->setCurrentIndex(0);

    ui->conversationTable->setColumnWidth(0, 60);
    ui->conversationTable->setColumnWidth(1, 300);
}

MainWindow::~MainWindow()
{
    delete ui;
}
