#ifndef LVK_STAT_TIMEINTERVAL_H
#define LVK_STAT_TIMEINTERVAL_H

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace Stats
{

/// \ingroup Lvk
/// \addtogroup Stats
/// @{

/**
 * \brief The TimeInterval type identifies an stastitic time interval.
 *
 * Statistics are stored in time intervals. Time intervals don't represent a fixed amount
 * of time such us 1 hour, 12 hours or 1 day. Instead, every time StatsManager::newInterval()
 * is invoked a new time interval starts.
 *
 * 1 is the first time interval, 2 is the second time interval, and so on. 0 is the null time
 * interval.
 *
 */
typedef unsigned TimeInterval;

/// @}

} // namespace Stats

/// @}

} // namespace Lvk



#endif // LVK_STAT_TIMEINTERVAL_H
