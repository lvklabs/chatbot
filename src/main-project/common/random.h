#ifndef LVK_COMMON_RANDOM_H
#define LVK_COMMON_RANDOM_H

namespace Lvk
{

namespace Common
{

/**
 * Handy class to generate random numbers
 */

class Random
{
public:
    static int getInt(int min, int max);

private:
    Random();
    Random(Random&);
};

} // namespace Common

} // namespace Lvk


#endif // LVK_COMMON_RANDOM_H
