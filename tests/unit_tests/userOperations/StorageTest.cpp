#include <cstdio>
#include <gtest/gtest.h>
#include <string>
#include <utility>

#include <userOperations/Credentials/Credentials.hpp>
#include <userOperations/Customer/Customer.hpp>
#include <userOperations/Storage/Storage.hpp>

void removeDb(void)
{
    std::remove(Storage::dbFile);
}

TEST(CustomerStorageTest, createCustomer)
{
    std::string fullname = "Ramazan Cetin";
    std::string email = "ramco@gmail.com";
    std::string address = "Amsterdam";
    std::string bday = "23/02/1111";
    std::pair<uuidType, Customer> p;

    removeDb();
    ASSERT_NO_THROW(
        ASSERT_TRUE(Storage::CustomerMngr::insert(Customer(fullname, email, address, bday), p)));
    ASSERT_NE(0, p.first);
    ASSERT_EQ(p.second.fullname(), fullname);
    ASSERT_EQ(p.second.email(), email);
    ASSERT_EQ(p.second.address(), address);
    ASSERT_EQ(p.second.birthday(), bday);
}

TEST(CustomerStorageTest, getCustomerByEmail)
{
    std::string fullname = "Ramazan Cetin 1";
    std::string email = "ramco1@gmail.com";
    std::string address = "Istanbul";
    std::string bday = "23/02/1112";
    std::pair<uuidType, Customer> pInserted, pGot;

    ASSERT_NO_THROW(ASSERT_TRUE(
        Storage::CustomerMngr::insert(Customer(fullname, email, address, bday), pInserted)));
    ASSERT_NO_THROW(ASSERT_TRUE(Storage::CustomerMngr::getByEmail(email, pGot)));
    ASSERT_EQ(pGot.first, pInserted.first);
    ASSERT_EQ(pGot.second.fullname(), pInserted.second.fullname());
    ASSERT_EQ(pGot.second.email(), pInserted.second.email());
    ASSERT_EQ(pGot.second.address(), pInserted.second.address());
    ASSERT_EQ(pGot.second.birthday(), pInserted.second.birthday());
}

TEST(CustomerStorageTest, getCustomerByUsername)
{
    Credentials cred;
    std::string fullname = "Selen Gecgel";
    std::string email = "selen@gmail.com";
    std::string address = "Amsterdam";
    std::string bday = "11/11/1111";
    std::pair<uuidType, Customer> pInserted, pGot;
    std::pair<uuidType, Credentials> credInserted;

    ASSERT_NO_THROW(ASSERT_TRUE(
        Storage::CustomerMngr::insert(Customer(fullname, email, address, bday), pInserted)));
    ASSERT_TRUE(cred.setCredentials("selen.gecgel", "GWerw1231..zxc!12312", std::cout));
    ASSERT_NO_THROW(
        ASSERT_TRUE(Storage::CredentialsMngr::insert(cred, pInserted.first, credInserted)));
    ASSERT_NO_THROW(ASSERT_TRUE(Storage::CustomerMngr::getByUsername(cred.username(), pGot)));
    ASSERT_EQ(pGot.first, pInserted.first);
    ASSERT_EQ(pGot.second.fullname(), pInserted.second.fullname());
    ASSERT_EQ(pGot.second.email(), pInserted.second.email());
    ASSERT_EQ(pGot.second.address(), pInserted.second.address());
    ASSERT_EQ(pGot.second.birthday(), pInserted.second.birthday());
}

TEST(CustomerStorageTest, updateCustomer)
{
    std::string fullname = "Ramazan Cetin";
    std::string email = "ramco1254@gmail.com";
    std::string address = "Amsterdam, 1095DK";
    std::string bday = "23/02/1999";
    std::string updatedFullname = "Ramazan C";
    std::string updatedEmail = "ramco..1254@gmail.com";
    std::pair<uuidType, Customer> pInserted, pGot, pUpdated;

    ASSERT_NO_THROW(ASSERT_TRUE(
        Storage::CustomerMngr::insert(Customer(fullname, email, address, bday), pInserted)));
    ASSERT_NO_THROW(ASSERT_TRUE(Storage::CustomerMngr::update(
        pInserted.first, Customer(updatedFullname, updatedEmail, address, bday), pUpdated)));
    ASSERT_NO_THROW(ASSERT_TRUE(Storage::CustomerMngr::getByEmail(updatedEmail, pGot)));
    ASSERT_EQ(pGot.first, pUpdated.first);
    ASSERT_EQ(pGot.second.fullname(), pUpdated.second.fullname());
    ASSERT_EQ(pGot.second.email(), pUpdated.second.email());
    ASSERT_EQ(pGot.second.address(), pUpdated.second.address());
    ASSERT_EQ(pGot.second.birthday(), pUpdated.second.birthday());
}

TEST(CredentialsStorageTest, createCredentials)
{
    Credentials cred;
    uuidType id;
    std::pair<uuidType, Credentials> credInserted;
    std::string username = "rmzn1254";
    std::string password = "GWerw1231..zxc!";

    ASSERT_TRUE(cred.setCredentials(username, password, std::cout));
    ASSERT_NO_THROW(ASSERT_TRUE(Storage::CredentialsMngr::insert(cred, 2131234, credInserted)));
    ASSERT_NE(credInserted.first, 0);
    ASSERT_EQ(credInserted.second.username(), username);
}

TEST(CredentialsStorageTest, getCredentialsByCustomerId)
{
    Credentials cred;
    std::string fullname = "Test Customer";
    std::string email = "customer@gmail.com";
    std::string address = "Turkey";
    std::string bday = "22/22/1121";
    std::string username = "customer.112";
    std::string password = "GASBcc2212..zxc!";
    std::pair<uuidType, Customer> pInserted;
    std::pair<uuidType, Credentials> credInserted, credGot;

    ASSERT_NO_THROW(ASSERT_TRUE(
        Storage::CustomerMngr::insert(Customer(fullname, email, address, bday), pInserted)));
    ASSERT_TRUE(cred.setCredentials(username, password, std::cout));
    ASSERT_NO_THROW(
        ASSERT_TRUE(Storage::CredentialsMngr::insert(cred, pInserted.first, credInserted)));
    ASSERT_NO_THROW(
        ASSERT_TRUE(Storage::CredentialsMngr::getByCustomerId(pInserted.first, credGot)));
    ASSERT_EQ(credInserted.first, credGot.first);
    ASSERT_EQ(credInserted.second.username(), credGot.second.username());
    ASSERT_EQ(credInserted.second.password(), credGot.second.password());
}

TEST(CredentialsStorageTest, getCredentialsByUsername)
{
    uuidType outId;
    Credentials cred;
    Credentials gotCred;
    std::string fullname = "Test Customer2";
    std::string email = "customer2@gmail.com";
    std::string address = "Turkey";
    std::string bday = "12/12/4534";
    std::string username = "customer.2";
    std::string password = "GASBcc2212..zxc!";
    std::pair<uuidType, Customer> pInserted;
    std::pair<uuidType, Credentials> credInserted, credGot;

    ASSERT_NO_THROW(ASSERT_TRUE(
        Storage::CustomerMngr::insert(Customer(fullname, email, address, bday), pInserted)));
    ASSERT_TRUE(cred.setCredentials(username, password, std::cout));
    ASSERT_NO_THROW(
        ASSERT_TRUE(Storage::CredentialsMngr::insert(cred, pInserted.first, credInserted)));
    ASSERT_NO_THROW(ASSERT_TRUE(Storage::CredentialsMngr::getByUsername(username, credGot)));
    ASSERT_EQ(credInserted.first, credGot.first);
    ASSERT_EQ(credInserted.second.username(), credGot.second.username());
    ASSERT_EQ(credInserted.second.password(), credGot.second.password());
}

TEST(CredentialsStorageTest, updateCustomer)
{
    uuidType credId;
    Credentials cred;
    std::string fullname = "Test Customer3";
    std::string email = "customer3@gmail.com";
    std::string address = "Turkey";
    std::string bday = "12/12/4534";
    std::string username = "customer.3";
    std::string password = "GASBcc2212..zxc!";
    std::string updatedUsername = "customer.3.updated";
    std::string updatedPassword = "UpdatdPass12!a";
    Credentials updatedCred(updatedUsername, updatedPassword);
    std::pair<uuidType, Customer> pInserted;
    std::pair<uuidType, Credentials> credInserted, credGot, credUpdated;

    ASSERT_TRUE(updatedCred.isValid(std::cout));
    ASSERT_NO_THROW(ASSERT_TRUE(
        Storage::CustomerMngr::insert(Customer(fullname, email, address, bday), pInserted)));
    ASSERT_TRUE(cred.setCredentials(username, password, std::cout));
    ASSERT_NO_THROW(
        ASSERT_TRUE(Storage::CredentialsMngr::insert(cred, pInserted.first, credInserted)));
    ASSERT_NO_THROW(ASSERT_TRUE(
        Storage::CredentialsMngr::update(credInserted.first, updatedCred, credUpdated)));
    ASSERT_NO_THROW(ASSERT_TRUE(Storage::CredentialsMngr::getByUsername(updatedUsername, credGot)));
    ASSERT_EQ(credUpdated.first, credGot.first);
    ASSERT_EQ(credUpdated.second.username(), credGot.second.username());
    ASSERT_EQ(credUpdated.second.password(), credGot.second.password());
}

TEST(AccountStorageTest, createSingleAccount)
{
    std::string fullname = "Ali Durdu";
    std::string email = "ali11@gmail.com";
    std::string address = "Kars";
    std::string bday = "12/12/2332";
    double balance1 = 10.22;
    Datetime d1 = Datetime::now();
    Account a;

    std::pair<uuidType, Customer> cInserted;
    std::pair<uuidType, Account> aInserted;

    ASSERT_NO_THROW(ASSERT_TRUE(
        Storage::CustomerMngr::insert(Customer(fullname, email, address, bday), cInserted)));
    a.setAttributes(cInserted.first, balance1, d1);
    ASSERT_NO_THROW(ASSERT_TRUE(Storage::AccountMngr::insert(a, cInserted.first, aInserted)));
    ASSERT_NE(aInserted.first, 0);
    ASSERT_EQ(aInserted.second, a);
}

TEST(AccountStorageTest, getMultipleAccount)
{
    std::string fullname = "Veli Durdu";
    std::string email = "veli11@gmail.com";
    std::string address = "Adiyaman";
    std::string bday = "12/12/2500";
    double balance1 = 2522.2412;
    Datetime d1 = Datetime::now();
    double balance2 = 534632.1112;
    Datetime d2 = Datetime::now();

    std::pair<uuidType, Customer> cInserted;
    std::pair<uuidType, Account> aInserted1, aInserted2;
    std::vector<std::pair<uuidType, Account>> aGotVector;

    ASSERT_NO_THROW(ASSERT_TRUE(
        Storage::CustomerMngr::insert(Customer(fullname, email, address, bday), cInserted)));
    ASSERT_NO_THROW(ASSERT_TRUE(Storage::AccountMngr::insert(
        Account(cInserted.first, balance1, d1), cInserted.first, aInserted1)));
    ASSERT_NO_THROW(ASSERT_TRUE(Storage::AccountMngr::insert(
        Account(cInserted.first, balance2, d2), cInserted.first, aInserted2)));
    ASSERT_NO_THROW(
        ASSERT_TRUE(Storage::AccountMngr::getByCustomerId(cInserted.first, aGotVector)));

    std::cout << aInserted1.second << "\n";
    std::cout << aInserted2.second << "\n";

    ASSERT_EQ(aGotVector[0].first, aInserted1.first);
    ASSERT_EQ(aGotVector[0].second, aInserted1.second);
    ASSERT_EQ(aGotVector[1].first, aInserted2.first);
    ASSERT_EQ(aGotVector[1].second, aInserted2.second);
}

// TODO: Implement operator== and operator<< for credentials and customer classes
// TODO: When classes cannot be created in constructors, it should throw an exception.
//      For credentials class for example.
//      For customer no input validation so no need.
//      For account no input validation so no need.

// Should we check customerId in creation of credentials and account objects????