#include <gtest/gtest.h>
#include <iostream>
#include <vector>

#include <StringInput.hpp>

TEST(BasicString, SuccessOnCorrectData)
{
    std::vector<StringInput> testVector = {"test", "TEST", "Test"};

    for(auto str : testVector) {
        EXPECT_TRUE(str.isValid());
    }
}

TEST(BasicString, FailsOnWrongData)
{
    std::vector<StringInput> testVector = {
        "test...\\", "test1%", "123test$", "*test#", "tes-=#t", "!test"};

    for(auto str : testVector) {
        EXPECT_FALSE(str.isValid());
    }
}