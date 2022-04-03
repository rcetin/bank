#include <ctime>
#include <gtest/gtest.h>
#include <iostream>

#include <Datetime.hpp>

TEST(BasicDatetime, CreateDatetime)
{
    Datetime currentTime(std::time(nullptr));
    ASSERT_NE(currentTime.dump(), "");
}