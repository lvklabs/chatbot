#include "welcomewidget.h"
#include "ui_welcomewidget.h"
#include "front-end/filefilters.h"
#include "front-end/mainwindow.h"
#include "common/settings.h"
#include "common/settingskeys.h"
#include "common/globalstrings.h"

#include <QtDebug>
#include <QFileDialog>
#include <QStyle>
#include <QDesktopWidget>

//--------------------------------------------------------------------------------------------------
// WelcomeWidget
//--------------------------------------------------------------------------------------------------

Lvk::FE::WelcomeWidget::WelcomeWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::WelcomeWidget), m_mw(new MainWindow())
{
    ui->setupUi(this);

    connect(ui->createChatbotButton,   SIGNAL(clicked()), SLOT(onCreateChatbot()));
    connect(ui->openChatbotButton,     SIGNAL(clicked()), SLOT(onOpenChatbot()));
    connect(ui->openLastChatbotButton, SIGNAL(clicked()), SLOT(onOpenLastChatbot()));

    ui->openLastChatbotButton->setVisible(Lvk::Cmn::Settings().contains(SETTING_LAST_FILE));

#ifdef WIN32
    ui->spacerTop->changeSize(20, 214);
    ui->linkSpacer->changeSize(20, 64);
#endif

    setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, size(),
                                    qApp->desktop()->availableGeometry()));

    setWindowIcon(QIcon(APP_ICON_FILE));
}

//--------------------------------------------------------------------------------------------------

Lvk::FE::WelcomeWidget::~WelcomeWidget()
{
    delete m_mw;
    delete ui;
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::WelcomeWidget::onOpenChatbot()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), "",
                                                    FileFilters::chatbotFilter());
    if (!filename.isEmpty()) {
        m_mw->openFile(filename);
        m_mw->show();
        close();
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::WelcomeWidget::onCreateChatbot()
{
    m_mw->newFile();
    m_mw->show();
    close();
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::WelcomeWidget::onOpenLastChatbot()
{
    m_mw->openLastFile();
    m_mw->show();
    close();
}

