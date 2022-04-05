#include <gtest/gtest.h>
#include <string>

#include <userOperations/Credentials/Credentials.hpp>

TEST(BasicCredentials, FailUsernamePasswordOnWrongFormat)
{
    std::vector<std::pair<std::string, std::string>> testVector = {{"test", "test...&&"},
                                                                   {"__tests", "test123123"},
                                                                   {"test123_", "123test....."},
                                                                   {"etwetw###", "*test%$#T"},
                                                                   {"xvxc,x.,sd=", "tes-=t123123"},
                                                                   {"ramzcn__xzn", "!testTTTT??#"}};

    for(auto credPair : testVector) {
        Credentials cred;
        EXPECT_FALSE(cred.setCredentials(credPair.first, credPair.second));
        EXPECT_EQ("", cred.username());
        EXPECT_EQ("", cred.password());
    }
}

TEST(BasicCredentials, SuccessUsernamePasswordOnCorrectFormat)
{
    std::vector<std::pair<std::string, std::string>> testVector = {{"rmzn1254", "TESTs234.///."},
                                                                   {"rmzn.1231", "Test1234."},
                                                                   {"rmzn_cetin", "Test&#asd12"},
                                                                   {"a_b_c_d", "*test%$#Tas1d"},
                                                                   {"a.s.e.f.s.a", "23423Tfsr233."},
                                                                   {"helloworld", "testTES12."}};

    for(auto credPair : testVector) {
        Credentials cred;
        EXPECT_TRUE(cred.setCredentials(credPair.first, credPair.second));
        EXPECT_EQ(credPair.first, cred.username());
        EXPECT_EQ(credPair.second, cred.password());
    }
}
