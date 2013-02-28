#include "front-end/cluewidget.h"
#include "da-clue/scriptmanager.h"
#include "da-clue/clueengine.h"
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
