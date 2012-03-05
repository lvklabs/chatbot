#ifndef RULEOUTPUTWIDGET_H
#define RULEOUTPUTWIDGET_H

#include <QTextEdit>
#include <QStringList>

/**
 * \brief Custom widget to edit or display NLP rule ouputs
 */

class RuleOutputWidget : public QTextEdit
{
    Q_OBJECT
public:
    explicit RuleOutputWidget(QWidget *parent = 0);

    QStringList outputList();

    void setOutputList(const QStringList &outputList);

signals:

public slots:

};

#endif // RULEOUTPUTWIDGET_H
