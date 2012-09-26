#include "optionswindow.h"
#include "ui_optionswindow.h"


//--------------------------------------------------------------------------------------------------
// OptionsWindow
//--------------------------------------------------------------------------------------------------

Lvk::FE::OptionsWindow::OptionsWindow(QWidget *parent)
    : QDialog(parent), ui(new Ui::OptionsWindow)
{
    ui->setupUi(this);
    connectSignals();
    setOptions(Options());

#ifndef FREELING_SUPPORT
    ui->lemmatizeCheckBox->setChecked(false);
    ui->lemmatizeCheckBox->setEnabled(false);
#endif
}

//--------------------------------------------------------------------------------------------------

Lvk::FE::OptionsWindow::OptionsWindow(const Options &opt, QWidget *parent)
    : QDialog(parent), ui(new Ui::OptionsWindow)
{
    ui->setupUi(this);
    connectSignals();
    setOptions(opt);

#ifndef FREELING_SUPPORT
    ui->lemmatizeCheckBox->setChecked(false);
    ui->lemmatizeCheckBox->setEnabled(false);
#endif
}

//--------------------------------------------------------------------------------------------------

Lvk::FE::OptionsWindow::~OptionsWindow()
{
    delete ui;
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::OptionsWindow::connectSignals()
{
    connect(ui->acceptButton, SIGNAL(clicked()), SLOT(accept()));
    connect(ui->cancelButton, SIGNAL(clicked()), SLOT(reject()));
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::OptionsWindow::setOptions(const Lvk::FE::OptionsWindow::Options &opt)
{
    ui->rmDupCharCheckBox->setChecked(opt.ignoreDupChars);
    ui->lemmatizeCheckBox->setChecked(opt.lemmatizeSentence);
    ui->exactMatchCheckbox->setChecked(opt.exactMatchSupport);
}

//--------------------------------------------------------------------------------------------------

Lvk::FE::OptionsWindow::Options Lvk::FE::OptionsWindow::options() const
{
    Options opt;

    opt.ignoreDupChars = ui->rmDupCharCheckBox->isChecked();
    opt.lemmatizeSentence = ui->lemmatizeCheckBox->isChecked();
    opt.exactMatchSupport = ui->exactMatchCheckbox->isChecked();

    return opt;
}
