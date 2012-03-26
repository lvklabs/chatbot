#ifndef LVK_NLP_DEFAULTSANITIZER_H
#define LVK_NLP_DEFAULTSANITIZER_H

#include "sanitizer.h"

namespace Lvk
{

namespace Nlp
{

/**
 * \brief Default implementation of the Sanitizer interface
 */

class DefaultSanitizer : public Sanitizer
{
public:
    virtual QString sanitize(const QString &str);

};

} // namespace Nlp

} // namespace Lvk


#endif // LVK_NLP_DEFAULTSANITIZER_H
