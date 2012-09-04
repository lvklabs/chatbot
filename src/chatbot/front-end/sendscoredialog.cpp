#include "sendscoredialog.h"
#include "ui_sendscoredialog.h"

#include <QPixmap>
#include <QImage>
#include <QIcon>

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
    setMinimumWidth(width()*1.5);
    setMinimumHeight(height()*3);
    ui->detailsLabel->setVisible(true);
    ui->detailsText->setVisible(true);
    ui->label->setText(tr("Send score? This will also send your rule definitions."));
}

