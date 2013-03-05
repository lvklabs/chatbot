#include "front-end/cluewidget.h"
#include "da-clue/scriptmanager.h"
#include "da-clue/clueengine.h"
#include "ui_cluewidget.h"

//--------------------------------------------------------------------------------------------------
// ClueWidget
//--------------------------------------------------------------------------------------------------

Lvk::FE::ClueWidget::ClueWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::ClueWidget), m_appFacade(0)
{
    ui->setupUi(this);

    connect(ui->refreshButton, SIGNAL(clicked()), SLOT(refresh()));

    loadCharacter();
}

//--------------------------------------------------------------------------------------------------

Lvk::FE::ClueWidget::~ClueWidget()
{
    delete ui;
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ClueWidget::setAppFacade(BE::AppFacade *appFacade)
{
    m_appFacade = appFacade;

    loadCharacter();
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ClueWidget::loadCharacter()
{
    if (m_appFacade && !m_appFacade->currentCharacter().isEmpty()) {
        ui->curCharacterLabel->setText(tr("Character: ") + m_appFacade->currentCharacter());
    } else {
        ui->curCharacterLabel->setText(tr("Character: (none)"));
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ClueWidget::refresh()
{
    loadCharacter();
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ClueWidget::clear()
{
    // TODO
}
