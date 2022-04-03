#include <gtest/gtest.h>
#include <iostream>

#include <Rand.hpp>

TEST(BasicRand, Randomness)
{
    Rand<1, 1000000> random;

    std::cout << random() << std::endl;
}