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
 * \brief The NullLemmatizer class provides a Lemmatizer that does nothing.
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
     * Returns \a input without any change.
     */
    QString lemmatize(const QString &input)
    {
        return input;
    }
};


/// @}

} // namespace Nlp

/// @}

} // namespace Lvk

#endif // LVK_NLP_DEFAULTLEMMATIZER_H
