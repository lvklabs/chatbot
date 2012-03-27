#include "ruleoutputwidget.h"

RuleOutputWidget::RuleOutputWidget(QWidget *parent) :
    QPlainTextEdit(parent)
{
}

QStringList RuleOutputWidget::outputList()
{
    return toPlainText().split("\n", QString::SkipEmptyParts);
}

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

void RuleOutputWidget::highlightOuput(int /*number*/)
{
    static const QString HIGHLIGHT_INPUT_CSS = "background-color: rgba(255,128,128,128);";

    setStyleSheet(HIGHLIGHT_INPUT_CSS);
}

void RuleOutputWidget::clearHighlight()
{
    setStyleSheet("");
}
