#ifndef LVK_NLP_FREELINGLEMMATIZER_H
#define LVK_NLP_FREELINGLEMMATIZER_H

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
 * \brief The FreelingLemmatizer class provides the default implementation of the Lemmatizer
 *        interface.
 *
 * The FreelingLemmatizer class uses Freeling to tokenize and lemmatize sentences.
 */
class FreelingLemmatizer : public Lemmatizer
{
public:
    /**
     * Constructs a FreelingLemmatizer with the default parameters.
     */
    FreelingLemmatizer();

    /**
     * \copydoc Lemmatizer::~Lemmatizer()
     */
    ~FreelingLemmatizer();

    /**
     * \copydoc Lemmatizer::tokenize(const QString &input, QStringList &l)
     */
    virtual void tokenize(const QString &input, QStringList &l);

    /**
     * \copydoc Lemmatizer::lemmatize(const QString &input, WordList &l)
     */
    virtual void lemmatize(const QString &input, WordList &l);


private:
    FreelingLemmatizer(const FreelingLemmatizer&);
    FreelingLemmatizer & operator=(const FreelingLemmatizer&);

    bool m_flInit;
    tokenizer *m_tk;
    splitter *m_sp;
    maco *m_morpho;
};

/// @}

} // namespace Nlp

/// @}

} // namespace Lvk

#endif // LVK_NLP_FREELINGLEMMATIZER_H
