#ifndef LVK_CA_CHATVIRTUALUSER_H
#define LVK_CA_CHATVIRTUALUSER_H

#include <QString>
#include <QPixmap>

namespace Lvk
{

namespace CA
{

/**
 * \brief VirtualUser is used by chat clients to get a response or to show a different avatar
 */
class VirtualUser
{
public:
    ~VirtualUser() {}

    virtual QString getResponse(const QString &input) = 0;
    virtual QPixmap getAvatar() = 0;
};

} //namespace CA

} //namespace Lvk


#endif // LVK_CA_CHATVIRTUALUSER_H
