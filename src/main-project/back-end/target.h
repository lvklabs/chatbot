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
 * \brief Target class provides information about a rule target
 */

typedef RosterItem Target;


/**
 * \brief The TargetList class provides a list of targets
 */

typedef QList<Target> TargetList;

/// @}

} // namespace BE

/// @}

} // namespace Lvk


#endif // LVK_BE_TARGET_H
