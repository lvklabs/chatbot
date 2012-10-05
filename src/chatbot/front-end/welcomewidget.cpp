#include "welcomewidget.h"
#include "ui_welcomewidget.h"
#include "front-end/filedialog.h"
#include "front-end/mainwindow.h"
#include "common/settings.h"
#include "common/settingskeys.h"
#include "common/globalstrings.h"

#include <QtDebug>
#include <QFileDialog>
#include <QStyle>
#include <QFile>
#include <QDesktopWidget>
#include <QMessageBox>

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

    m_lastFilename = Lvk::Cmn::Settings().value(SETTING_LAST_FILE).toString();

    ui->openLastChatbotButton->setVisible(!m_lastFilename.isEmpty());

#ifdef WIN32
    ui->spacerTop->changeSize(20, 214);
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
    QString filename = FileDialog::openChatbot(this);

    if (!filename.isEmpty()) {
        m_mw->openFile(filename);
        m_mw->show();
        close();
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::WelcomeWidget::onCreateChatbot()
{
    QString filename = FileDialog::newChatbot(this);

    if (!filename.isEmpty()) {
        m_mw->newFile(filename);
        m_mw->show();
        close();
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::WelcomeWidget::onOpenLastChatbot()
{
    if (QFile::exists(m_lastFilename)) {
        m_mw->openFile(m_lastFilename);
        m_mw->show();
        close();
    } else {
        QString msg = tr("Last file opened does not exist anymore: '%1'");
        QMessageBox::critical(this, tr("Open last file"), msg.arg(m_lastFilename));
        ui->openLastChatbotButton->setVisible(false);
    }
}

