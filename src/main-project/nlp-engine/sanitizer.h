#ifndef LVK_NLP_SANITIZER_H
#define LVK_NLP_SANITIZER_H

#include <QString>
#include <QStringList>

namespace Lvk
{

namespace Nlp
{

/**
 * \brief Abstract interface for NLP Sanitizers
 */

class Sanitizer
{
public:

    /**
     * \brief Sanitizes the given string
     */
    virtual QString sanitize(const QString &str) = 0;

    /**
     * \brief Sanitizes each string of the given list
     */
    virtual QStringList sanitize(const QStringList &list)
    {
        QStringList sanitizedList;
        for (int i = 0; i < list.size(); ++i) {
            sanitizedList.append(sanitize(list[i]));
        }
        return sanitizedList;
    }

    virtual ~Sanitizer() {}
};

} // namespace Nlp

} // namespace Lvk


#endif // LVK_NLP_SANITIZER_H
