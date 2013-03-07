#include "detailsdialog.h"
#include "ui_detailsdialog.h"

#include <QPixmap>
#include <QImage>
#include <QIcon>
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
    ui->setupUi(this);
    ui->detailsText->setPlainText(details);
    ui->detailsLabel->setVisible(false);
    ui->detailsText->setVisible(false);

    setPixmap(QStyle::SP_MessageBoxQuestion);

    QString labelText = msg;

    if (!linkMsg.isEmpty()) {
        labelText.append(QString("<br/><a href=\"#\">%1</a>").arg(linkMsg));
    }

    ui->label->setTextFormat(Qt::RichText);
    ui->label->setText(labelText);

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

void Lvk::FE::DetailsDialog::setCancelButtonVisible(bool visible)
{
    ui->rejectButton->setVisible(visible);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::DetailsDialog::setPixmap(QStyle::StandardPixmap pixmap, int w, int h)
{
    ui->icon->setPixmap(style()->standardIcon(pixmap).pixmap(w, h));
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
