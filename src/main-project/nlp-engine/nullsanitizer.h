#ifndef LVK_NLP_NULLSANITIZER_H
#define LVK_NLP_NULLSANITIZER_H

#include "sanitizer.h"

namespace Lvk
{

namespace Nlp
{

/**
 * \brief The NullSanitizer provides a sanitizer that does nothing
 */

class NullSanitizer : public Sanitizer
{
public:
    /**
     *  \brief Returns the same string that is given
     */
    virtual QString sanitize(const QString &str) const;

};

} // namespace Nlp

} // namespace Lvk


#endif // LVK_NLP_NULLSANITIZER_H
