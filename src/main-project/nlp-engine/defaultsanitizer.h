#ifndef LVK_NLP_DEFAULTSANITIZER_H
#define LVK_NLP_DEFAULTSANITIZER_H

#include "sanitizer.h"

namespace Lvk
{

namespace Nlp
{

/**
 * \brief Default implementation of the Sanitizer interface
 *
 * The DefaultSanitizer class returns a string with all vowels without acute or diaeresis
 */

class DefaultSanitizer : public Sanitizer
{
public:
    virtual QString sanitize(const QString &str) const;

};

} // namespace Nlp

} // namespace Lvk


#endif // LVK_NLP_DEFAULTSANITIZER_H
