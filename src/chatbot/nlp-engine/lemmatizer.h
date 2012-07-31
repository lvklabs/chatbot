#ifndef LVK_NLP_LEMMATIZER_H
#define LVK_NLP_LEMMATIZER_H

#include <string>
#include "freeling.h"

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
 * \brief The Engine class provides a lemmatizer
 */
class Lemmatizer
{
public:
    /**
     * Constructs a Lemmatizer with the default parameters.
     */
    Lemmatizer();

    /**
     * Destroys the object.
     */
    ~Lemmatizer();

    /**
     * Lemmatizes \a input.
     */
    std::list<sentence> lemmatize(const std::string& input);

private:
    tokenizer *m_tk;
    splitter *m_sp;
    maco *m_morfo;
};


/// @}

} // namespace Nlp

/// @}

} // namespace Lvk

#endif // LVK_NLP_LEMMATIZER_H
