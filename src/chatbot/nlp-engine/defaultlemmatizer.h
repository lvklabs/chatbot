#ifndef LVK_NLP_DEFAULTLEMMATIZER_H
#define LVK_NLP_DEFAULTLEMMATIZER_H

#include "nlp-engine/lemmatizer.h"

class tokenizer;
class splitter;
class maco;

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
 * \brief The DefaultLemmatizer class provides the default implementation of the Lemmatizer
 *        interface.
 *
 * The DefaultLemmatizer class uses Freeling to tokenize and lemmatize sentences.
 */
class DefaultLemmatizer : public Lemmatizer
{
public:
    /**
     * Constructs a DefaultLemmatizer with the default parameters.
     */
    DefaultLemmatizer();

    /**
     * \copydoc Lemmatizer::~Lemmatizer()
     */
    ~DefaultLemmatizer();

    /**
     * \copydoc Lemmatizer::lemmatize(const QString &)
     */
    QString lemmatize(const QString &input);

private:
    bool m_flInit;
    tokenizer *m_tk;
    splitter *m_sp;
    maco *m_morpho;
};


/// @}

} // namespace Nlp

/// @}

} // namespace Lvk

#endif // LVK_NLP_DEFAULTLEMMATIZER_H
