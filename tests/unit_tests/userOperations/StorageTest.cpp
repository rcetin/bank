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
    uuidType gotId;

    ASSERT_NO_THROW(ASSERT_TRUE(Storage::CustomerMngr::insert(
        Customer("Ramazan Cetin 1", "ramco1@gmail.com", "Istanbul", "23/02/1112"), id)));
    ASSERT_NO_THROW(ASSERT_TRUE(Storage::CustomerMngr::getByEmail("ramco1@gmail.com", gotId)));
    ASSERT_EQ(id, gotId);
}

TEST(CustomerStorageTest, updateCustomer)
{
    uuidType id;
    uuidType updatedId;

    ASSERT_NO_THROW(ASSERT_TRUE(Storage::CustomerMngr::getByEmail("ramco@gmail.com", id)));
    ASSERT_NO_THROW(ASSERT_TRUE(Storage::CustomerMngr::update(
        id, Customer("Ramazan Cetin", "ramco1254@gmail.com", "Amsterdam, 1095DK", "23/02/1999"))));
    ASSERT_NO_THROW(
        ASSERT_TRUE(Storage::CustomerMngr::getByEmail("ramco1254@gmail.com", updatedId)));
    ASSERT_EQ(id, updatedId);
}

TEST(CustomerStorageTest, getCustomerByUsername)
{
    Credentials cred;
    uuidType idInserted;
    uuidType idRetrieved;
    uuidType credId;

    ASSERT_NO_THROW(ASSERT_TRUE(Storage::CustomerMngr::insert(
        Customer("Selen Gecgel", "selen@gmail.com", "Amsterdam", "11/11/1111"), idInserted)));

    ASSERT_TRUE(cred.setCredentials("selen.gecgel", "GWerw1231..zxc!12312", std::cout));
    ASSERT_NO_THROW(ASSERT_TRUE(Storage::CredentialsMngr::insert(cred, idInserted, credId)));
    ASSERT_NO_THROW(
        ASSERT_TRUE(Storage::CustomerMngr::getByUsername(cred.username(), idRetrieved)));
    ASSERT_EQ(idInserted, idRetrieved);
}

TEST(CredentialsStorageTest, createCredentials)
{
    Credentials cred;
    uuidType id;

    ASSERT_TRUE(cred.setCredentials("rmzn1254", "GWerw1231..zxc!", std::cout));

    ASSERT_NO_THROW(ASSERT_TRUE(Storage::CredentialsMngr::insert(cred, 2131234, id)));
    ASSERT_NE(0, id);
}

TEST(CredentialsStorageTest, getCredentialsByCustomerId)
{
    uuidType customerId;
    uuidType credId;
    uuidType outId;
    Credentials cred;

    ASSERT_NO_THROW(ASSERT_TRUE(Storage::CustomerMngr::insert(
        Customer("Test Customer", "customer@gmail.com", "Turkey", "22/22/1121"), customerId)));
    ASSERT_TRUE(cred.setCredentials("customer.112", "GASBcc2212..zxc!", std::cout));
    ASSERT_NO_THROW(ASSERT_TRUE(Storage::CredentialsMngr::insert(cred, customerId, outId)));
    ASSERT_NO_THROW(ASSERT_TRUE(Storage::CredentialsMngr::getByCustomerId(customerId, credId)));
    ASSERT_EQ(outId, credId);
}

TEST(CredentialsStorageTest, getCredentialsByUsername)
{
    uuidType customerId;
    uuidType credId;
    uuidType outId;
    Credentials cred;
    std::string username = "customer.2";

    ASSERT_NO_THROW(ASSERT_TRUE(Storage::CustomerMngr::insert(
        Customer("Test Customer2", "customer2@gmail.com", "Turkey", "12/12/4534"), customerId)));
    ASSERT_TRUE(cred.setCredentials(username, "GASBcc2212..zxc!", std::cout));
    ASSERT_NO_THROW(ASSERT_TRUE(Storage::CredentialsMngr::insert(cred, customerId, outId)));
    ASSERT_NO_THROW(ASSERT_TRUE(Storage::CredentialsMngr::getByUsername(username, credId)));
    ASSERT_EQ(outId, credId);
}

TEST(CredentialsStorageTest, updateCustomer)
{
    uuidType customerId;
    uuidType credId;
    uuidType outId;
    Credentials cred;
    std::string updatedUsername = "customer.3.updated";

    ASSERT_NO_THROW(ASSERT_TRUE(Storage::CustomerMngr::insert(
        Customer("Test Customer3", "customer3@gmail.com", "Turkey", "12/12/4534"), customerId)));
    ASSERT_TRUE(cred.setCredentials("customer.3", "GASBcc2212..zxc!", std::cout));
    ASSERT_NO_THROW(ASSERT_TRUE(Storage::CredentialsMngr::insert(cred, customerId, outId)));
    ASSERT_NO_THROW(ASSERT_TRUE(
        Storage::CredentialsMngr::update(outId, Credentials{updatedUsername, "UpdatdPass12!a"})));
    ASSERT_NO_THROW(ASSERT_TRUE(Storage::CredentialsMngr::getByUsername(updatedUsername, credId)));
    ASSERT_EQ(outId, credId);
}
