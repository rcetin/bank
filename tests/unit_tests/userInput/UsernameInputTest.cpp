#include <gtest/gtest.h>
#include <iostream>
#include <vector>

#include <UsernameInput.hpp>

TEST(BasicUsernameString, SuccessOnCorrectData)
{
    std::vector<std::pair<UsernameInput, std::string>> testVector = {
        {UsernameInput{"ramazan"}, "ramazan"},
        {UsernameInput{"ramazan1cetin"}, "ramazan1cetin"},
        {UsernameInput{"ramazan_cetin"}, "ramazan_cetin"},
        {UsernameInput{"rmzn1254"}, "rmzn1254"},
        {UsernameInput{"rmzn.cetin"}, "rmzn.cetin"},
        {UsernameInput{"r1r2r4rr5"}, "r1r2r4rr5"}};

    for(auto str : testVector) {
        std::cout << "Check usename: " << str.second << "\n";
        EXPECT_TRUE(str.first.isValid());
        EXPECT_EQ(str.first.data(), str.second);
    }
}

TEST(BasicUsernameString, FailsOnWrongData)
{
    std::vector<UsernameInput> testVector = {
        "rmzn...&&", "_rmzn123123", ".123test.....", "*test%$#T", "tes-=t123123", "!testTTTT??#"};

    for(auto str : testVector) {
        EXPECT_FALSE(str.isValid());
    }
}

TEST(BasicUsernameString, FailsOnLengthCheck)
{
    std::vector<UsernameInput> testVector = {"Test...1123121312312312123123123123", "Tt1.?"};

    for(auto str : testVector) {
        EXPECT_FALSE(str.isValid());
    }
}
