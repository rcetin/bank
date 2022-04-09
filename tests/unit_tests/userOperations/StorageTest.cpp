#include <cstdio>
#include <gtest/gtest.h>
#include <string>

#include <userOperations/Credentials/Credentials.hpp>
#include <userOperations/Customer/Customer.hpp>
#include <userOperations/Storage/Storage.hpp>

void removeDb(void)
{
    std::remove(Storage::dbFile);
}

TEST(CustomerStorageTest, createCustomer)
{
    uuidType id;
    removeDb();
    ASSERT_NO_THROW(ASSERT_TRUE(Storage::CustomerMngr::insert(
        Customer("Ramazan Cetin", "ramco@gmail.com", "Amsterdam", "23/02/1111"), id)));
    ASSERT_NE(0, id);
}

TEST(CustomerStorageTest, getCustomerByEmail)
{
    uuidType id;
    ASSERT_NO_THROW(ASSERT_TRUE(Storage::CustomerMngr::getByEmail("ramco@gmail.com", id)));
}

TEST(CustomerStorageTest, updateCustomer)
{
    uuidType id;
    ASSERT_NO_THROW(ASSERT_TRUE(Storage::CustomerMngr::getByEmail("ramco@gmail.com", id)));
    ASSERT_NO_THROW(ASSERT_TRUE(Storage::CustomerMngr::update(
        id, Customer("Ramazan Cetin", "ramco1254@gmail.com", "Amsterdam, 1095DK", "23/02/1999"))));
}

TEST(CustomerStorageTest, getCustomerByUsername)
{
    Credentials cred;
    uuidType idInserted;
    uuidType idRetrieved;

    ASSERT_NO_THROW(ASSERT_TRUE(Storage::CustomerMngr::insert(
        Customer("Selen Gecgel", "selen@gmail.com", "Amsterdam", "11/11/1111"), idInserted)));

    ASSERT_TRUE(cred.setCredentials("selen.gecgel", "GWerw1231..zxc!12312", std::cout));
    ASSERT_NO_THROW(ASSERT_TRUE(Storage::CredentialsMngr::insert(cred, idInserted)));
    ASSERT_NO_THROW(
        ASSERT_TRUE(Storage::CustomerMngr::getByUsername(cred.username(), idRetrieved)));
    ASSERT_EQ(idInserted, idRetrieved);
}

TEST(CredentialsStorageTest, createCredentials)
{
    Credentials cred;
    ASSERT_TRUE(cred.setCredentials("rmzn1254", "GWerw1231..zxc!", std::cout));

    ASSERT_NO_THROW(ASSERT_TRUE(Storage::CredentialsMngr::insert(cred, 2131234)));
}
