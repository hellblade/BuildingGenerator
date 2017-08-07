#include "Random.h"

namespace Random
{
   
    // From here: http://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform    
    double generateGaussianNoise(const double &variance)
    {
        static bool haveSpare = false;
        static double rand1, rand2;

        if (haveSpare)
        {
            haveSpare = false;
            return sqrt(variance * rand1) * sin(rand2);
        }

        haveSpare = true;

        rand1 = Rnd();
        if (rand1 < 1e-100) rand1 = 1e-100;
        rand1 = -2 * log(rand1);
        rand2 = Rnd() * TWO_PI;

        return sqrt(variance * rand1) * cos(rand2);
    }
}