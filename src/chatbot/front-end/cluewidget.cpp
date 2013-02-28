#include "front-end/cluewidget.h"
#include "ui_cluewidget.h"

//--------------------------------------------------------------------------------------------------
// ClueWidget
//--------------------------------------------------------------------------------------------------

Lvk::FE::ClueWidget::ClueWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::ClueWidget)
{
    ui->setupUi(this);
}

//--------------------------------------------------------------------------------------------------

Lvk::FE::ClueWidget::~ClueWidget()
{
    delete ui;
}
