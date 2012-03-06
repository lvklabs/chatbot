#include "random.h"

#include <QDateTime>
#include <cstdlib>

int Lvk::Common::Random::getInt(int min, int max)
{
    static unsigned int seed = 0;

    if (!seed) {
        seed = QDateTime::currentDateTime().toTime_t();
        srand(seed);
    }

    return min + (int)(rand()/(double)(RAND_MAX - 1)*(max - min + 1));
}
