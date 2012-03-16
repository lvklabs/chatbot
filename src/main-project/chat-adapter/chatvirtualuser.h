#ifndef LVK_CA_CHATVIRTUALUSER_H
#define LVK_CA_CHATVIRTUALUSER_H

#include <QString>
#include <QPixmap>

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
    ~VirtualUser() {}

    virtual QString getResponse(const QString &input, const QString &from) = 0;
    virtual QPixmap getAvatar() = 0;
};

} //namespace CA

} //namespace Lvk


#endif // LVK_CA_CHATVIRTUALUSER_H
