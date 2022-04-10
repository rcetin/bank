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
    std::string fullname = "Ramazan Cetin";
    std::string email = "ramco@gmail.com";
    std::string address = "Amsterdam";
    std::string bday = "23/02/1111";

    removeDb();
    ASSERT_NO_THROW(
        ASSERT_TRUE(Storage::CustomerMngr::insert(Customer(fullname, email, address, bday), id)));
    ASSERT_NE(0, id);
}

TEST(CustomerStorageTest, getCustomerByEmail)
{
    uuidType id;
    uuidType gotId;
    Customer c;
    std::string fullname = "Ramazan Cetin 1";
    std::string email = "ramco1@gmail.com";
    std::string address = "Istanbul";
    std::string bday = "23/02/1112";

    ASSERT_NO_THROW(
        ASSERT_TRUE(Storage::CustomerMngr::insert(Customer(fullname, email, address, bday), id)));
    ASSERT_NO_THROW(ASSERT_TRUE(Storage::CustomerMngr::getByEmail("ramco1@gmail.com", gotId, c)));
    ASSERT_EQ(id, gotId);
    ASSERT_EQ(c.fullname(), fullname);
    ASSERT_EQ(c.email(), email);
    ASSERT_EQ(c.address(), address);
    ASSERT_EQ(c.birthday(), bday);
}

TEST(CustomerStorageTest, getCustomerByUsername)
{
    Credentials cred;
    uuidType idInserted;
    uuidType idRetrieved;
    uuidType credId;
    Customer c;
    std::string fullname = "Selen Gecgel";
    std::string email = "selen@gmail.com";
    std::string address = "Amsterdam";
    std::string bday = "11/11/1111";

    ASSERT_NO_THROW(ASSERT_TRUE(
        Storage::CustomerMngr::insert(Customer(fullname, email, address, bday), idInserted)));
    ASSERT_TRUE(cred.setCredentials("selen.gecgel", "GWerw1231..zxc!12312", std::cout));
    ASSERT_NO_THROW(ASSERT_TRUE(Storage::CredentialsMngr::insert(cred, idInserted, credId)));
    ASSERT_NO_THROW(
        ASSERT_TRUE(Storage::CustomerMngr::getByUsername(cred.username(), idRetrieved, c)));
    ASSERT_EQ(idInserted, idRetrieved);
    ASSERT_EQ(c.fullname(), fullname);
    ASSERT_EQ(c.email(), email);
    ASSERT_EQ(c.address(), address);
    ASSERT_EQ(c.birthday(), bday);
}

TEST(CustomerStorageTest, updateCustomer)
{
    uuidType id;
    uuidType updatedId;
    Customer c;
    std::string fullname = "Ramazan Cetin";
    std::string email = "ramco1254@gmail.com";
    std::string address = "Amsterdam, 1095DK";
    std::string bday = "23/02/1999";
    std::string updatedFullname = "Ramazan C";
    std::string updatedEmail = "ramco..1254@gmail.com";

    ASSERT_NO_THROW(
        ASSERT_TRUE(Storage::CustomerMngr::insert(Customer(fullname, email, address, bday), id)));
    ASSERT_NO_THROW(ASSERT_TRUE(
        Storage::CustomerMngr::update(id, Customer(updatedFullname, updatedEmail, address, bday))));
    ASSERT_NO_THROW(ASSERT_TRUE(Storage::CustomerMngr::getByEmail(updatedEmail, updatedId, c)));
    ASSERT_EQ(id, updatedId);
    ASSERT_EQ(c.fullname(), updatedFullname);
    ASSERT_EQ(c.email(), updatedEmail);
    ASSERT_EQ(c.address(), address);
    ASSERT_EQ(c.birthday(), bday);
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
    Credentials gotCred;
    Customer c;
    std::string fullname = "Test Customer";
    std::string email = "customer@gmail.com";
    std::string address = "Turkey";
    std::string bday = "22/22/1121";
    std::string username = "customer.112";
    std::string password = "GASBcc2212..zxc!";

    ASSERT_NO_THROW(ASSERT_TRUE(
        Storage::CustomerMngr::insert(Customer(fullname, email, address, bday), customerId)));
    ASSERT_TRUE(cred.setCredentials(username, password, std::cout));
    ASSERT_NO_THROW(ASSERT_TRUE(Storage::CredentialsMngr::insert(cred, customerId, outId)));
    ASSERT_NO_THROW(
        ASSERT_TRUE(Storage::CredentialsMngr::getByCustomerId(customerId, credId, gotCred)));
    ASSERT_EQ(outId, credId);
    ASSERT_EQ(gotCred.username(), cred.username());
    ASSERT_EQ(gotCred.password(), cred.password());
}

TEST(CredentialsStorageTest, getCredentialsByUsername)
{
    uuidType customerId;
    uuidType credId;
    uuidType outId;
    Credentials cred;
    Credentials gotCred;
    std::string fullname = "Test Customer2";
    std::string email = "customer2@gmail.com";
    std::string address = "Turkey";
    std::string bday = "12/12/4534";
    std::string username = "customer.2";
    std::string password = "GASBcc2212..zxc!";

    ASSERT_NO_THROW(ASSERT_TRUE(
        Storage::CustomerMngr::insert(Customer(fullname, email, address, bday), customerId)));
    ASSERT_TRUE(cred.setCredentials(username, password, std::cout));
    ASSERT_NO_THROW(ASSERT_TRUE(Storage::CredentialsMngr::insert(cred, customerId, outId)));
    ASSERT_NO_THROW(
        ASSERT_TRUE(Storage::CredentialsMngr::getByUsername(username, credId, gotCred)));
    ASSERT_EQ(outId, credId);
    ASSERT_EQ(gotCred.username(), cred.username());
    ASSERT_EQ(gotCred.password(), cred.password());
}

TEST(CredentialsStorageTest, updateCustomer)
{
    uuidType customerId;
    uuidType credId;
    uuidType outId;
    Credentials cred;
    Credentials gotCred;
    std::string fullname = "Test Customer3";
    std::string email = "customer3@gmail.com";
    std::string address = "Turkey";
    std::string bday = "12/12/4534";
    std::string username = "customer.3";
    std::string password = "GASBcc2212..zxc!";
    std::string updatedUsername = "customer.3.updated";
    std::string updatedPassword = "UpdatdPass12!a";
    Credentials updatedCred(updatedUsername, updatedPassword);

    ASSERT_TRUE(updatedCred.isValid(std::cout));
    ASSERT_NO_THROW(ASSERT_TRUE(
        Storage::CustomerMngr::insert(Customer(fullname, email, address, bday), customerId)));
    ASSERT_TRUE(cred.setCredentials(username, password, std::cout));
    ASSERT_NO_THROW(ASSERT_TRUE(Storage::CredentialsMngr::insert(cred, customerId, outId)));
    ASSERT_NO_THROW(ASSERT_TRUE(Storage::CredentialsMngr::update(outId, updatedCred)));
    ASSERT_NO_THROW(
        ASSERT_TRUE(Storage::CredentialsMngr::getByUsername(updatedUsername, credId, gotCred)));
    ASSERT_EQ(outId, credId);
    ASSERT_EQ(gotCred.username(), updatedCred.username());
    ASSERT_EQ(gotCred.password(), updatedCred.password());
}
