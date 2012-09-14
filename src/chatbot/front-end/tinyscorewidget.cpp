#include "tinyscorewidget.h"
#include "ui_tinyscorewidget.h"

#include <QtDebug>


//--------------------------------------------------------------------------------------------------
// TinyScoreWidget
//--------------------------------------------------------------------------------------------------

Lvk::FE::TinyScoreWidget::TinyScoreWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::TinyScoreWidget)
{
    ui->setupUi(this);

#ifdef WIN32
    layout()->setContentsMargins(0,0,0,0);
#else
    layout()->setContentsMargins(0,8,0,0);
#endif

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
    ui->scoreLabel->clear();
    ui->timeLabel->clear();
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::TinyScoreWidget::setScore(const Stats::Score &current, const Stats::Score &best)
{
    m_strCur = QString::number(current.total);
    m_strBest = QString::number(best.total);

    QString text = QString(tr("Score: %1 / Record: %2")).arg(m_strCur, m_strBest);

    ui->scoreLabel->setText(text);

    updateTooltip();
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::TinyScoreWidget::setRemainingTime(const QTime &time)
{
    m_strTime = time.toString("hh:mm:ss");

    QString text = QString(tr("%1")).arg(m_strTime);

    ui->timeLabel->setText(text);

    updateTooltip();
}

//--------------------------------------------------------------------------------------------------

bool Lvk::FE::TinyScoreWidget::event(QEvent *event)
{
    // TODO remove this debug info
    if (event->type() == QEvent::MouseButtonPress) qDebug() << "TinyScoreWidget click event";

    return false;
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::TinyScoreWidget::updateTooltip()
{
    QString tooltip = QString(tr("Actual score: %1\nRecord score: %2\nRemaining time: %3"))
            .arg(m_strCur, m_strBest, m_strTime);

    ui->scoreLabel->setToolTip(tooltip);
    ui->timeLabel->setToolTip(tooltip);
}
