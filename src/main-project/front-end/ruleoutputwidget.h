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

};

#endif // RULEOUTPUTWIDGET_H
