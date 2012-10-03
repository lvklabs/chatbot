#include "detailsdialog.h"
#include "ui_detailsdialog.h"

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
// DetailsDialog
//--------------------------------------------------------------------------------------------------

Lvk::FE::DetailsDialog::DetailsDialog(const QString &msg, const QString &linkMsg,
                                      const QString &details, QWidget *parent)
    : QDialog(parent), ui(new Ui::DetailsDialog), m_msg(msg)
{
    QPixmap questionPix = style()->standardIcon(QStyle::SP_MessageBoxQuestion).pixmap(48,48);

    ui->setupUi(this);
    ui->detailsText->setPlainText(details);
    ui->detailsLabel->setVisible(false);
    ui->detailsText->setVisible(false);
    ui->icon->setPixmap(questionPix);
    ui->label->setText(QString("%1<br/><a href=\"#\">%2</a>").arg(msg, linkMsg));

    alignCenter(DIALOG_INIT_W, DIALOG_INIT_H);

    connect(ui->label,        SIGNAL(linkActivated(QString)), SLOT(onLinkActivated(QString)));
    connect(ui->rejectButton, SIGNAL(clicked()),              SLOT(reject()));
    connect(ui->acceptButton, SIGNAL(clicked()),              SLOT(accept()));
}

//--------------------------------------------------------------------------------------------------

Lvk::FE::DetailsDialog::~DetailsDialog()
{
    delete ui;
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::DetailsDialog::onLinkActivated(const QString &)
{
    alignCenter(DIALOG_DET_W, DIALOG_DET_H);
    ui->detailsLabel->setVisible(true);
    ui->detailsText->setVisible(true);
    ui->label->setText(m_msg);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::DetailsDialog::alignCenter(int w, int h)
{
    QRect g = parentWidget() ? parentWidget()->geometry() : qApp->desktop()->availableGeometry();
    setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, QSize(w, h), g));
}
