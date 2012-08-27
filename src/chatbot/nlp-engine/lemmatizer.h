#ifndef LVK_NLP_LEMMATIZER_H
#define LVK_NLP_LEMMATIZER_H

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
     * Lemmatizes \a input.
     */
    virtual QString lemmatize(const QString &input) = 0;

    /**
     * Lemmatizes each string of the list \a list and returns a new one.
     */
    virtual QStringList lemmatize(const QStringList &list)
    {
        QStringList lemList;
        for (int i = 0; i < list.size(); ++i) {
            lemList.append(lemmatize(list[i]));
        }
        return lemList;
    }
};

/// @}

} // namespace Nlp

/// @}

} // namespace Lvk

#endif // LVK_NLP_LEMMATIZER_H
