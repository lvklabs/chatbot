#include "ruleoutputwidget.h"

RuleOutputWidget::RuleOutputWidget(QWidget *parent) :
    QTextEdit(parent)
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
