#pragma once

#ifndef TWO_PI
#define TWO_PI 6.2831853071795864769252866
#endif

#include <stdlib.h> 
#include <math.h>

namespace Random
{
    inline double Rnd()
    {
        return (double)rand() / RAND_MAX;
    }

    inline float Rndf()
    {
        return (float)rand() / RAND_MAX;
    }

    inline int Rnd(int max)
    {
        return (int)(Rnd() * max);
    }

    inline int Rnd(int min, int max)
    {
        return (int)((max - min) * Rnd()) + min;
    }

    // Use variance of 1 for standard normal distribution
    double generateGaussianNoise(const double &variance);
}