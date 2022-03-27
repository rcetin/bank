#include <gtest/gtest.h>
#include <iostream>
#include <vector>

#include <PasswordInput.hpp>

TEST(BasicPasswordString, SuccessOnCorrectData)
{
    std::vector<std::pair<PasswordInput, std::string>> testVector = {
        {PasswordInput{"Test1234."}, "Test1234."},
        {PasswordInput{"TESTs234.///."}, "TESTs234.///."}};

    for(auto str : testVector) {
        EXPECT_TRUE(str.first.isValid());
        EXPECT_EQ(str.first.data(), str.second);
    }
}

TEST(BasicPasswordString, FailsOnWrongData)
{
    std::vector<PasswordInput> testVector = {
        "test...&&", "test123123", "123test.....", "*test%$#T", "tes-=t123123", "!testTTTT??#"};

    for(auto str : testVector) {
        EXPECT_FALSE(str.isValid());
    }
}

TEST(BasicPasswordString, FailsOnLengthCheck)
{
    std::vector<PasswordInput> testVector = {"Test...1123121312312312123123123123", "Tt1.?"};

    for(auto str : testVector) {
        EXPECT_FALSE(str.isValid());
    }
}
