#ifndef LVK_CA_CHATVIRTUALUSER_H
#define LVK_CA_CHATVIRTUALUSER_H

#include <QString>
#include <QPixmap>
#include "contactinfo.h"

namespace Lvk
{

namespace CA
{

/**
 * \brief VirtualUser class provides information to the chatbot to behave like a human being
 *
 * VirtualUser class provides two methods: one to get a response and another one
 * to show a custom avatar
 */
class VirtualUser
{
public:
    /** Class destructor */
    virtual ~VirtualUser() {}

    /** Get a response for the given input and chat contact */
    virtual QString getResponse(const QString &input, const ContactInfo &contact) = 0;

    /** Virtual user custom avatar */
    virtual QPixmap getAvatar() = 0;
};

} //namespace CA

} //namespace Lvk


#endif // LVK_CA_CHATVIRTUALUSER_H
