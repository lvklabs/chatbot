#include "sendscoredialog.h"
#include "ui_sendscoredialog.h"

#include <QPixmap>
#include <QImage>
#include <QIcon>
#include <QStyle>
#include <QDesktopWidget>

#ifdef WIN32
# define DIALOG_INIT_W  500
# define DIALOG_INIT_H  100
# define DIALOG_DET_W   (500*1.2)
# define DIALOG_DET_H   (100*3)
#else
# define DIALOG_INIT_W  500
# define DIALOG_INIT_H  120
# define DIALOG_DET_W   (500*1.2)
# define DIALOG_DET_H   (100*3)
#endif

//--------------------------------------------------------------------------------------------------
// TinyScoreWidget
//--------------------------------------------------------------------------------------------------

Lvk::FE::SendScoreDialog::SendScoreDialog(QString details, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SendScoreDialog)
{
    QPixmap questionPix = style()->standardIcon(QStyle::SP_MessageBoxQuestion).pixmap(48,48);

    ui->setupUi(this);
    ui->detailsText->setPlainText(details);
    ui->detailsLabel->setVisible(false);
    ui->detailsText->setVisible(false);
    ui->icon->setPixmap(questionPix);
    ui->label->setText(tr("Send score? This will also send your rule definitions."
                          "<br/><a href=\"#\">See what I'm sending</a>"));

    alignCenter(DIALOG_INIT_W, DIALOG_INIT_H);

    connect(ui->label,        SIGNAL(linkActivated(QString)), SLOT(onLinkActivated(QString)));
    connect(ui->rejectButton, SIGNAL(clicked()),              SLOT(reject()));
    connect(ui->acceptButton, SIGNAL(clicked()),              SLOT(accept()));
}

//--------------------------------------------------------------------------------------------------

Lvk::FE::SendScoreDialog::~SendScoreDialog()
{
    delete ui;
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::SendScoreDialog::onLinkActivated(const QString &)
{
    alignCenter(DIALOG_DET_W, DIALOG_DET_H);
    ui->detailsLabel->setVisible(true);
    ui->detailsText->setVisible(true);
    ui->label->setText(tr("Send score? This will also send your rule definitions."));
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::SendScoreDialog::alignCenter(int w, int h)
{
    QRect g = parentWidget() ? parentWidget()->geometry() : qApp->desktop()->availableGeometry();
    setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, QSize(w, h), g));
}
