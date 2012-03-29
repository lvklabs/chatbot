#include "ruleoutputwidget.h"

//--------------------------------------------------------------------------------------------------
// RuleOutputWidget
//--------------------------------------------------------------------------------------------------

RuleOutputWidget::RuleOutputWidget(QWidget *parent) :
    QPlainTextEdit(parent)
{
    connectTextChangedSignal();
}

//--------------------------------------------------------------------------------------------------

QStringList RuleOutputWidget::outputList()
{
    return toPlainText().split("\n", QString::SkipEmptyParts);
}

//--------------------------------------------------------------------------------------------------

void RuleOutputWidget::setOutputList(const QStringList &outputList)
{
    QString output;

    for (int i = 0; i < outputList.size(); ++i) {
        QString trimmed = outputList[i].trimmed();
        if (!trimmed.isEmpty()) {
            output += trimmed + "\n";
        }
    }

    setPlainText(output);
}

//--------------------------------------------------------------------------------------------------

void RuleOutputWidget::highlightOuput(int /*number*/)
{
    static const QString HIGHLIGHT_INPUT_CSS = "background-color: rgba(255,128,128,128);";

    setStyleSheet(HIGHLIGHT_INPUT_CSS);
}

//--------------------------------------------------------------------------------------------------

void RuleOutputWidget::clearHighlight()
{
    setStyleSheet("");
}

//--------------------------------------------------------------------------------------------------

void RuleOutputWidget::connectTextChangedSignal()
{
    connect(this, SIGNAL(textChanged()), SIGNAL(outputTextEdited()));
}

//--------------------------------------------------------------------------------------------------

void RuleOutputWidget::disconnectTextChangedSignal()
{
    disconnect(this, SIGNAL(textChanged()), this, SIGNAL(outputTextEdited()));
}

//--------------------------------------------------------------------------------------------------

void RuleOutputWidget::setPlainText(const QString &text)
{
    // QTBUG-8449: Signal textEdited() is missing in QTextEdit and QPlainTextEdit
    disconnectTextChangedSignal();
    QPlainTextEdit::setPlainText(text);
    connectTextChangedSignal();
}

//--------------------------------------------------------------------------------------------------

void RuleOutputWidget::clear()
{
    // QTBUG-8449: Signal textEdited() is missing in QTextEdit and QPlainTextEdit
    disconnectTextChangedSignal();
    QPlainTextEdit::clear();
    connectTextChangedSignal();
}
