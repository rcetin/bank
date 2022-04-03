#ifndef UTILS_RAND_HPP
#define UTILS_RAND_HPP

#include <random>

template <int32_t rangeLow, int32_t rangeHigh>
class Rand
{
public:
    int32_t operator()()
    {
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist(rangeLow, rangeHigh);

        return dist(rng);
    }
};

#endif