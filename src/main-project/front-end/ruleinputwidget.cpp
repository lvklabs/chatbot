#include "ruleinputwidget.h"

#include <QTextEdit>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QLabel>

RuleInputWidget::RuleInputWidget(QWidget *parent) :
    QWidget(parent),
    m_layout(new QVBoxLayout(this)),
    m_inputLabel(new QLabel(tr("If user writes:"))),
    m_input(new QLineEdit()),
    m_inputVariantsLabel(new QLabel(tr("Or any of these variants:"))),
    m_inputVariants(new QTextEdit())
{
    m_layout->setMargin(0);

    m_layout->addWidget(m_inputLabel);
    m_layout->addWidget(m_input);
    m_layout->addWidget(m_inputVariantsLabel);
    m_layout->addWidget(m_inputVariants);

    setLayout(m_layout);

    connect (m_input, SIGNAL(textEdited(QString)), SIGNAL(inputTextEdited(QString)));
}

void RuleInputWidget::clear()
{
    m_input->clear();
    m_inputVariants->clear();
}

QStringList RuleInputWidget::inputList()
{
    QStringList inputList = m_inputVariants->toPlainText().split("\n", QString::SkipEmptyParts);
    inputList.prepend(m_input->text());

    return inputList;
}

void RuleInputWidget::setInputList(const QStringList &inputList)
{
    QString input, inputVariants;

    for (int i = 0; i < inputList.size(); ++i) {
        QString trimmed = inputList[i].trimmed();
        if (!trimmed.isEmpty()) {
            if (i == 0) {
                input = trimmed;
            } else {
                inputVariants += trimmed + "\n";
            }
        }
    }

    m_input->setText(input);
    m_inputVariants->setText(inputVariants);
}

void RuleInputWidget::setFocusOnInput()
{
    m_input->setFocus();
}

void RuleInputWidget::setFocusOnInputVariants()
{
    m_inputVariants->setFocus();
}
