#include <gtest/gtest.h>
#include <iostream>

#include <userOperations/Credentials/Credentials.hpp>
#include <userOperations/Customer/Customer.hpp>
#include <userOperations/UserOps.hpp>

TEST(BasicUserOps, createCustomer)
{
    Customer c;
    ASSERT_TRUE(createCustomer(std::cin, std::cout, c));
}

// TEST(BasicUserOps, createCredentials)
// {
//     Credentials c;
//     ASSERT_TRUE(createCredentials(std::cin, std::cout, c));
// }