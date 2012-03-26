#ifndef LVK_CA_CONTACTINFO_H
#define LVK_CA_CONTACTINFO_H

#include <QString>

namespace Lvk
{

namespace CA
{

/**
 * \brief ContactInfo struct provides information about a chat contact
 */
struct ContactInfo
{
    QString username; //! Username used to log in the chat service
    QString fullname; //! Full name
};


} //namespace CA

} //namespace Lvk

#endif // LVK_CA_CONTACTINFO_H
