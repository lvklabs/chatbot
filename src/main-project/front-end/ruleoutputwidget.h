#ifndef RULEOUTPUTWIDGET_H
#define RULEOUTPUTWIDGET_H

#include <QPlainTextEdit>
#include <QStringList>

/**
 * \brief Custom widget to edit or display NLP rule ouputs
 */

class RuleOutputWidget : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit RuleOutputWidget(QWidget *parent = 0);

    QStringList outputList();

    void setOutputList(const QStringList &outputList);

public slots:

    void clearHighlight();

    void highlightOuput(int number);

    void setPlainText(const QString &text);

    void clear();

signals:

    void outputTextEdited();

private:
    void connectTextChangedSignal();
    void disconnectTextChangedSignal();
};

#endif // RULEOUTPUTWIDGET_H
