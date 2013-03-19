#include "front-end/ruleeditwidget.h"
#include "ui_ruleeditwidget.h"


//--------------------------------------------------------------------------------------------------
// RuleEditWidget
//--------------------------------------------------------------------------------------------------

Lvk::FE::RuleEditWidget::RuleEditWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::RuleEditWidget), m_type(-1), m_nextCatVisible(false)
{
    ui->setupUi(this);
    clear();
    connectSignals();
}

//--------------------------------------------------------------------------------------------------

Lvk::FE::RuleEditWidget::~RuleEditWidget()
{
    delete ui;
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::RuleEditWidget::connectSignals()
{
    connect(ui->teachRuleButton,       SIGNAL(clicked()),   SLOT(onTeachButtonPressed()));
    connect(ui->undoRuleButton,        SIGNAL(clicked()),   SLOT(onUndoButtonPressed()));

    connect(ui->ruleInputWidget,       SIGNAL(inputVariantsEdited()),
            SLOT(onRuleEdited()));
    connect(ui->ruleOutputWidget,      SIGNAL(outputTextEdited()),
            SLOT(onRuleEdited()));
    connect(ui->categoryNameTextEdit,  SIGNAL(textEdited(QString)),
            SLOT(onRuleInputEdited(QString)));
    connect(ui->ruleInputWidget,       SIGNAL(inputTextEdited(QString)),
            SLOT(onRuleInputEdited(QString)));
    connect(ui->ruleInputWidget,       SIGNAL(targetTextEdited(QString)),
            SLOT(onRuleTargetEdited(QString)));
    connect(ui->nextCategoryList,      SIGNAL(activated(int)),
            SLOT(onNextCategoryEdited(int)));
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::RuleEditWidget::setRule(const BE::Rule *rule)
{
    clear();

    if (rule) {
        if (rule->type() == BE::Rule::OrdinaryRule) {
            initCategoriesList(rule);
        } else {
            ui->nextCategoryList->clear();
        }

        ui->categoryNameTextEdit->setText(rule->name());
        ui->ruleInputWidget->setTargets(rule->target());
        ui->ruleInputWidget->setInput(rule->input());
        ui->ruleOutputWidget->setOutput(rule->output());
        setCurrentNextCategory(rule->nextCategory());

        backupRule();

        setUiMode(rule->type());
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::RuleEditWidget::clear()
{
    m_type = -1;

    ui->categoryNameTextEdit->clear();
    ui->ruleInputWidget->clear();
    ui->ruleOutputWidget->clear();
    ui->nextCategoryList->clear();

    m_ruleBackup.clear();

    ui->categoryNameLabel->setVisible(false);
    ui->categoryNameTextEdit->setVisible(false);
    ui->ruleInputWidget->setVisible(false);
    ui->ruleOutputWidget->setVisible(false);
    ui->chatbotRepliesLabel->setVisible(false);
    ui->nextCategoryLabel->setVisible(false);
    ui->nextCategoryList->setVisible(false);
    ui->teachRuleButton->setVisible(false);
    ui->undoRuleButton->setVisible(false);
    ui->teachRuleButton->setEnabled(false);
    ui->undoRuleButton->setEnabled(false);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::RuleEditWidget::setRoster(const BE::Roster &roster)
{
    ui->ruleInputWidget->setRoster(roster);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::RuleEditWidget::clearRoster()
{
    ui->ruleInputWidget->clearRoster();
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::RuleEditWidget::clearAll()
{
    clear();
    clearRoster();
}

//--------------------------------------------------------------------------------------------------

QString Lvk::FE::RuleEditWidget::name() const
{
    return ui->categoryNameTextEdit->text();
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::TargetList Lvk::FE::RuleEditWidget::targets() const
{
    return ui->ruleInputWidget->targets();
}

//--------------------------------------------------------------------------------------------------

QStringList Lvk::FE::RuleEditWidget::input() const
{
    return ui->ruleInputWidget->input();
}

//--------------------------------------------------------------------------------------------------

QStringList Lvk::FE::RuleEditWidget::output() const

{
    return ui->ruleOutputWidget->output();
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::RuleEditWidget::setFocusOnInput()
{
    ui->ruleInputWidget->setFocusOnInput();
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::RuleEditWidget::setFocusOnOutput()
{
    ui->ruleOutputWidget->setFocus();
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::RuleEditWidget::setButtonsEnabled(bool enabled)
{
    ui->teachRuleButton->setEnabled(enabled);
    ui->undoRuleButton->setEnabled(enabled);
}

//--------------------------------------------------------------------------------------------------

quint64 Lvk::FE::RuleEditWidget::nextCategory() const
{
    return currentCategoryId();
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::RuleEditWidget::setNextCategoryVisible(bool visible)
{
    m_nextCatVisible = visible;

    if (m_type == BE::Rule::OrdinaryRule) {
        ui->nextCategoryLabel->setVisible(visible);
        ui->nextCategoryList->setVisible(visible);
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::RuleEditWidget::onTeachButtonPressed()
{
    setButtonsEnabled(false);

    backupRule();

    emit teachRule();
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::RuleEditWidget::onUndoButtonPressed()
{
    setButtonsEnabled(false);

    ui->categoryNameTextEdit->setText(m_ruleBackup.name());
    ui->ruleInputWidget->setTargets(m_ruleBackup.target());
    ui->ruleInputWidget->setInput(m_ruleBackup.input());
    ui->ruleOutputWidget->setOutput(m_ruleBackup.output());
    setCurrentNextCategory(m_ruleBackup.nextCategory());

    emit undoRule();
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::RuleEditWidget::onRuleEdited()
{
    setButtonsEnabled(true);

    emit ruleEdited();
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::RuleEditWidget::onRuleInputEdited(const QString &input)
{
    onRuleEdited();

    emit ruleInputEdited(input);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::RuleEditWidget::onRuleTargetEdited(const QString &/*ruleInput*/)
{
    onRuleEdited();
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::RuleEditWidget::onNextCategoryEdited(int /*index*/)
{
    if (currentCategoryId() != m_ruleBackup.nextCategory()) {
        onRuleEdited();
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::RuleEditWidget::backupRule()
{
    m_ruleBackup.setName(ui->categoryNameTextEdit->text());
    m_ruleBackup.setTarget(ui->ruleInputWidget->targets());
    m_ruleBackup.setInput(ui->ruleInputWidget->input());
    m_ruleBackup.setOutput(ui->ruleOutputWidget->output());
    m_ruleBackup.setNextCategory(currentCategoryId());
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::RuleEditWidget::initCategoriesList(const BE::Rule *rule)
{
    // Find root rule
    const BE::Rule *root = rule;
    while (root->parent()) {
        root = root->parent();
    }

    ui->nextCategoryList->clear();
    ui->nextCategoryList->addItem(tr("(Keep current category)"), 0);

    foreach (const BE::Rule *child, root->children()) {
        if (child->type() == BE::Rule::ContainerRule && child->id() != rule->parent()->id()) {
            ui->nextCategoryList->addItem(child->name(), child->id());
        }
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::RuleEditWidget::setCurrentNextCategory(quint64 catId)
{
    for (int i = 0; i < ui->nextCategoryList->count(); ++i) {
        if (categoryId(i) == catId) {
            ui->nextCategoryList->setCurrentIndex(i);
            return;
        }
    }

    if (ui->nextCategoryList->count() > 0) {
        ui->nextCategoryList->setCurrentIndex(0);
    }
}

//--------------------------------------------------------------------------------------------------

quint64 Lvk::FE::RuleEditWidget::currentCategoryId() const
{
    return categoryId(ui->nextCategoryList->currentIndex());
}

//--------------------------------------------------------------------------------------------------

quint64 Lvk::FE::RuleEditWidget::categoryId(int index) const
{
    return ui->nextCategoryList->itemData(index).toULongLong();
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::RuleEditWidget::setUiMode(BE::Rule::Type type)
{
    switch (type) {

    case BE::Rule::ContainerRule:
        ui->ruleOutputWidget->setMaximumHeight(16777215);
        ui->categoryNameLabel->setVisible(true);
        ui->categoryNameTextEdit->setVisible(true);
        ui->ruleInputWidget->setVisible(false);
        ui->ruleOutputWidget->setVisible(false);
        ui->chatbotRepliesLabel->setVisible(false);
        ui->nextCategoryLabel->setVisible(false);
        ui->nextCategoryList->setVisible(false);
        ui->teachRuleButton->setVisible(true);
        ui->undoRuleButton->setVisible(true);
        ui->teachRuleButton->setEnabled(false);
        ui->undoRuleButton->setEnabled(false);
        ui->teachRuleButton->setText(QObject::tr("Change name"));
        break;

    case BE::Rule::OrdinaryRule:
        ui->ruleOutputWidget->setMaximumHeight(16777215);
        ui->categoryNameLabel->setVisible(false);
        ui->categoryNameTextEdit->setVisible(false);
        ui->ruleInputWidget->setVisible(true);
        ui->ruleOutputWidget->setVisible(true);
        ui->chatbotRepliesLabel->setVisible(true);
        ui->nextCategoryLabel->setVisible(m_nextCatVisible);
        ui->nextCategoryList->setVisible(m_nextCatVisible);
        ui->teachRuleButton->setVisible(true);
        ui->undoRuleButton->setVisible(true);
        ui->teachRuleButton->setEnabled(false);
        ui->undoRuleButton->setEnabled(false);
        ui->chatbotRepliesLabel->setText(QObject::tr("Chatbot replies:"));
        ui->teachRuleButton->setText(QObject::tr("Teach rule to the chatbot"));
        break;

    case BE::Rule::EvasiveRule:
        ui->ruleOutputWidget->setMaximumHeight(150);
        ui->categoryNameLabel->setVisible(false);
        ui->categoryNameTextEdit->setVisible(false);
        ui->ruleInputWidget->setVisible(false);
        ui->ruleOutputWidget->setVisible(true);
        ui->chatbotRepliesLabel->setVisible(true);
        ui->nextCategoryLabel->setVisible(false);
        ui->nextCategoryList->setVisible(false);
        ui->teachRuleButton->setVisible(true);
        ui->undoRuleButton->setVisible(true);
        ui->teachRuleButton->setEnabled(false);
        ui->undoRuleButton->setEnabled(false);
        ui->chatbotRepliesLabel->setText(QObject::tr("If chatbot does not understand,"
                                                     " it replies:"));
        ui->teachRuleButton->setText(QObject::tr("Teach rule to the chatbot"));
        break;
    }

    m_type = type;
}

