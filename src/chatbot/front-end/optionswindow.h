#ifndef LVK_FE_OPTIONSWINDOW_H
#define LVK_FE_OPTIONSWINDOW_H

#include <QDialog>

namespace Ui {
    class OptionsWindow;
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
 * \brief The OptionsWindow class provides a window to configure the application options.
 *
 * Currently only two NLP engine options are available through this window. All options are
 * set/get using the OptionsWindow::Options structure.
 *
 */
class OptionsWindow : public QDialog
{
    Q_OBJECT

public:

    /**
     * The Options struct provides a simple structure with all the application options
     */
    struct Options
    {
        Options() : lemmatizeSentence(false), ignoreDupChars(false), exactMatchSupport(false) { }

        bool lemmatizeSentence; ///< NLP engine lemmatizes sentences
        bool ignoreDupChars;    ///< NLP engine ignores duplicated characters
        bool exactMatchSupport; ///< NLP engine exact match support enabled
    };

    /**
     * Constructs an OptionsWindow object which is a child of \a parent.
     */
    explicit OptionsWindow(QWidget *parent = 0);

    /**
     * Constructs an OptionsWindow object which is a child of \a parent and with initial options
     * \a opt
     */
    explicit OptionsWindow(const Options &opt, QWidget *parent = 0);

    /**
     * Destroys the object.
     */
    ~OptionsWindow();

    /**
     * Initilize the window with options \a opt
     */
    void setOptions(const Options &opt);

    /**
     * Returns the options set in the window
     */
    Options options() const;

private:
    Ui::OptionsWindow *ui;

    void connectSignals();
};

/// @}

} // namespace FE

/// @}

} // namespace Lvk

#endif // LVK_FE_OPTIONSWINDOW_H
