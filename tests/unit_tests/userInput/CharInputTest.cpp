#include <gtest/gtest.h>

#include <CharInput.hpp>

TEST(BasicChar, SuccessOnCorrectData)
{
    std::vector<std::pair<CharInput, char>> testVector = {{"a", 'a'}, {"Z", 'Z'}, {"b", 'b'}};

    for(auto charInput : testVector) {
        EXPECT_TRUE(charInput.first.isValid());
        EXPECT_EQ(charInput.first.data(), charInput.second);
    }
}

TEST(BasicChar, FailsOnWrongData)
{
    std::vector<CharInput> testVector = {"--12", "asdas", "1", "+-21d", "Asd", "!test"};

    for(auto charInput : testVector) {
        EXPECT_FALSE(charInput.isValid());
    }
}
