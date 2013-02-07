#ifndef LVK_NLP_LEMMATIZER_H
#define LVK_NLP_LEMMATIZER_H

#include "nlp-engine/word.h"

#include <QString>
#include <QStringList>

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
 * \brief The Lemmatizer class provides the interface for all lemmatizers
 */
class Lemmatizer
{
public:

    /**
     * Destroys the object.
     */
    virtual ~Lemmatizer() {}

    /**
     * Tokenizes \a input.
     */
    virtual void tokenize(const QString &input, QStringList &l) = 0;

    /**
     * Lemmatizes \a input.
     */
    virtual void lemmatize(const QString &input, WordList &l) = 0;
};

/// @}

} // namespace Nlp

/// @}

} // namespace Lvk

#endif // LVK_NLP_LEMMATIZER_H
