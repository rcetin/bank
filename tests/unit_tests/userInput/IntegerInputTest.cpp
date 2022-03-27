#include <gtest/gtest.h>
#include <iostream>
#include <vector>

#include <IntegerInput.hpp>

TEST(BasicInteger, SuccessOnCorrectData)
{
    std::vector<std::pair<IntegerInput, int32_t>> testVector = {
        {"10032", 10032}, {"0", 0}, {"-12122", -12122}};

    for(auto intInput : testVector) {
        EXPECT_TRUE(intInput.first.isValid());
        EXPECT_EQ(intInput.first.data(), intInput.second);
    }
}

TEST(BasicInteger, FailsOnWrongData)
{
    std::vector<IntegerInput> testVector = {"--12", "asdas", "000", "+-21d", "Asd", "!test"};

    for(auto intInput : testVector) {
        EXPECT_FALSE(intInput.isValid());
    }
}
