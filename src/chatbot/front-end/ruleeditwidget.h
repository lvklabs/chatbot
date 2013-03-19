#ifndef LVK_FE_RULEEDITWIDGET_H
#define LVK_FE_RULEEDITWIDGET_H

#include <QWidget>
#include <QStringList>

#include "back-end/target.h"
#include "back-end/rule.h"

namespace Ui {
    class RuleEditWidget;
}

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace FE
{

/// \ingroup Lvk
/// \addtogroup FE
/// @{

/**
 * \brief The RuleEditWidget class provides a widget to edit or display NLP rules
 *
 * The RuleInputWidget is composed by a RuleInputWidget, a RuleOutputWidget and a
 * AutocompleteTextEdit to edit targets.
 */
class RuleEditWidget : public QWidget
{
    Q_OBJECT

public:

    /**
     * Constructs a RuleEditWidget widget which is a child of \a parent.
     */
    explicit RuleEditWidget(QWidget *parent = 0);

    /**
     * Destroys the object
     */
    ~RuleEditWidget();

    /**
     * Sets \a rule as the current rule to edit
     */
    void setRule(const BE::Rule *rule);

    /**
     * Returns the rule name.
     */
    QString name() const;

    /**
     * Returns the list of targets in the rule.
     */
    BE::TargetList targets() const;

    /**
     * Returns the list of input strings in the rule.
     */
    QStringList input() const;

    /**
     * Returns the list of output strings in the rule.
     */
    QStringList output() const;

    /**
     * Returns the next category attribute in the rule
     */
    QString nextCategory() const;

    /**
     * Clears the widget with exception of the roster. To clear the roster use clearRoster()
     */
    void clear();

    /**
     * Sets the roster to enable autocompletion
     */
    void setRoster(const BE::Roster &roster);

    /**
     * Clears the roster set with setRoster();
     */
    void clearRoster();

    /**
     * Clears the widget, including the roster.
     */
    void clearAll();

    /**
     * Sets the focus on the input widget
     */
    void setFocusOnInput();

    /**
     * Sets the focus on the output widget
     */
    void setFocusOnOutput();

    /**
     * Sets teach and undo buttons as \a enabled
     */
    void setButtonsEnabled(bool enabled);

    /**
     * Sets the combo box with the next category as \a visible
     */
    void setNextCategoryVisible(bool visible);

signals:
    void teachRule();

    void undoRule();

    void ruleInputEdited(const QString &input);

    void ruleEdited();

private slots:
    void onRuleEdited();
    void onRuleInputEdited(const QString &input);
    void onRuleTargetEdited(const QString &ruleInput);
    void onNextCategoryEdited(int index);
    void onTeachButtonPressed();
    void onUndoButtonPressed();

private:
    Ui::RuleEditWidget *ui;
    int                 m_type;
    BE::Rule            m_ruleBackup;
    bool                m_nextCatVisible;

    void connectSignals();
    void setUiMode(BE::Rule::Type type);
    void backupRule();
    void setNextCategory(const QString &category);
    void initCategoriesList(const BE::Rule *rule);
};

/// @}

} // namespace FE

/// @}

} // namespace Lvk

#endif // LVK_FE_RULEEDITWIDGET_H
