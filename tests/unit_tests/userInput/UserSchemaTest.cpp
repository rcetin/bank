#include <gtest/gtest.h>
#include <ios>

#include <UserSchema.hpp>

TEST(SchemaOpen, ExceptionOnWrongFile)
{
    EXPECT_THROW(
        {
            try {
                UserSchema schema("NonExistFile.txt");
            }
            catch(const std::exception& e) {
                EXPECT_STREQ("Schema File could not be opened...", e.what());
                throw;
            }
        },
        UserSchema::SchemaError);
}

TEST(SchemaFormat, ExceptionOnWrongFormat)
{
    UserSchema schema{
        "/home/rcetin/workspace/repos/bank_management_system/support/options_schema.txt"};
}

// TEST(UserInput, FailOnWrongOption)
// {
//     FAIL() << "Complete Implementation\n";
// }

// TEST(UserInput, SuccessOnCorrectOption)
// {
//     FAIL() << "Complete Implementation\n";
// }

// TEST(UserInput, CheckCurrentMenu)
// {
//     FAIL() << "Complete Implementation\n";
// }
