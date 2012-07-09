#ifndef LVK_BE_TARGET_H
#define LVK_BE_TARGET_H

#include "roster.h"

#include <QList>

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace BE
{

/// \ingroup Lvk
/// \addtogroup BE
/// @{


/**
 * \brief Target class provides information about a rule target.
 *
 * By default, rules apply to any user. Use rule targets to create rules that apply only
 * to a list of contacts.
 */
typedef RosterItem Target;


/**
 * \brief The TargetList class provides a list of targets.
 *
 * \see Target
 */
typedef QList<Target> TargetList;


/// @}

} // namespace BE

/// @}

} // namespace Lvk


#endif // LVK_BE_TARGET_H
