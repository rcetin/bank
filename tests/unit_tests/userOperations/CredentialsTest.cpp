#include <gtest/gtest.h>
#include <string>

#include <userOperations/Credentials/Credentials.hpp>

TEST(BasicCredentials, FailUsernameOnWrongFormat)
{
    std::vector<std::string> testVector = {
        "test", "__tests", "test123_", "etwetw###", "xvxc,x.,sd=", "ramzcn__xzn"};

    for(auto username : testVector) {
        Credentials cred;
        EXPECT_FALSE(cred.setUsername(username));
        EXPECT_EQ("", cred.username());
    }
}

TEST(BasicCredentials, SuccessUsernameOnCorrectFormat)
{
    std::vector<std::string> testVector = {
        "rmzn1254", "rmzn.1231", "rmzn_cetin", "a_b_c_d", "a.s.e.f.s.a", "helloworld"};

    for(auto username : testVector) {
        Credentials cred;
        EXPECT_TRUE(cred.setUsername(username));
        EXPECT_EQ(username, cred.username());
    }
}

TEST(BasicCredentials, FailPasswordOnWrongFormat)
{
    std::vector<std::string> testVector = {
        "test...&&", "test123123", "123test.....", "*test%$#T", "tes-=t123123", "!testTTTT??#"};

    for(auto password : testVector) {
        Credentials cred;
        EXPECT_FALSE(cred.setPassword(password));
        EXPECT_EQ("", cred.password());
    }
}

TEST(BasicCredentials, SuccessPasswordOnCorrectFormat)
{
    std::vector<std::string> testVector = {"TESTs234.///.",
                                           "Test1234.",
                                           "Test&#asd12",
                                           "*test%$#Tas1d",
                                           "23423Tfsr233.",
                                           "testTES12."};

    for(auto password : testVector) {
        Credentials cred;
        EXPECT_TRUE(cred.setPassword(password));
        EXPECT_EQ(password, cred.password());
    }
}
