#ifndef LVK_NLP_NULLLEMMATIZER_H
#define LVK_NLP_NULLLEMMATIZER_H

#include "nlp-engine/lemmatizer.h"

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace Nlp
{

/// \ingroup Lvk
/// \addtogroup Nlp
/// @{

/**
 * \brief The NullLemmatizer class provides a Lemmatizer that does nothing, only tokenizes
 */
class NullLemmatizer : public Lemmatizer
{
public:
    /**
     * Constructs a NullLemamatizer with the default parameters.
     */
    NullLemmatizer() { }

    /**
     * \copydoc Lemmatizer::~Lemmatizer()
     */
    ~NullLemmatizer() { }

    /**
     *
     */
    void lemmatize(const QString &input, WordList &l)
    {
        l.clear();

        QStringList tokens;
        tokenize(input, tokens);

        foreach (QString t, tokens) {
            l.append(Nlp::Word(t, t, t));
        }
    }

    /**
     * Splits \a input using regexp "\\s+"
     */
    void tokenize(const QString &input, QStringList &l)
    {
        l = input.split(QRegExp("\\s+"), QString::SkipEmptyParts);
    }
};


/// @}

} // namespace Nlp

/// @}

} // namespace Lvk

#endif // LVK_NLP_NULLLEMMATIZER_H
