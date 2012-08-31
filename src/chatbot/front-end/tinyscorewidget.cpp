#include "tinyscorewidget.h"
#include "ui_tinyscorewidget.h"


//--------------------------------------------------------------------------------------------------
// TinyScoreWidget
//--------------------------------------------------------------------------------------------------

Lvk::FE::TinyScoreWidget::TinyScoreWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::TinyScoreWidget)
{
    ui->setupUi(this);

    clear();
}

//--------------------------------------------------------------------------------------------------

Lvk::FE::TinyScoreWidget::~TinyScoreWidget()
{
    delete ui;
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::TinyScoreWidget::clear()
{
    ui->bottomLabel->clear();
    ui->topLabel->clear();
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::TinyScoreWidget::setScore(const BE::Score &current, const BE::Score &best)
{
    QString text = QString(tr("Current points: %1 / Best: %2")).arg(QString::number(current.total),
                                                                    QString::number(best.total));

    ui->topLabel->setText(text);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::TinyScoreWidget::setRemainingTime(const QTime &time)
{
    QString text = QString(tr("Remaining time: %1")).arg(time.toString("hh:mm:ss"));

    ui->bottomLabel->setText(text);
}
