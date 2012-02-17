#include "sadoskyuiprototype1.h"
#include "ui_sadoskyuiprototype1.h"

SadoskyUIPrototype1::SadoskyUIPrototype1(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SadoskyUIPrototype1)
{
    ui->setupUi(this);

    ui->tableWidget2->setColumnWidth(0, 250);
    ui->tableWidget2->setColumnWidth(1, 60);
}

SadoskyUIPrototype1::~SadoskyUIPrototype1()
{
    delete ui;
}
