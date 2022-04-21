#include <cstdio>
#include <gtest/gtest.h>
#include <string>
#include <utility>

#include <userOperations/Credentials/Credentials.hpp>
#include <userOperations/Customer/Customer.hpp>
#include <userOperations/Storage/Storage.hpp>
#include <userOperations/Transaction/Transaction.hpp>

void removeDb(void)
{
    std::remove(Storage::dbFile);
}

bool createDummyCustomer(std::vector<Customer>& customerVector, int32_t size)
{
    constexpr int32_t maxArrSize = 128;
    Rand<0, INT32_MAX> random;
    int ret[3]{};

    for(int i = 0; i < size; ++i) {
        std::array<char, maxArrSize> fullnameArray{};
        std::array<char, maxArrSize> emailArray{};
        std::array<char, maxArrSize> birthdayArray{};
        ret[0] = snprintf(fullnameArray.data(), maxArrSize, "Dummy %d Customer", random());
        ret[1] = snprintf(emailArray.data(), maxArrSize, "dummy%d.customer@gmail.com", random());
        ret[2] = snprintf(birthdayArray.data(),
                          maxArrSize,
                          "%d/%d/%d",
                          random() % 31,
                          random() % 12,
                          1950 + (random() % (2020 - 1950)));

        if(ret[0] <= 0 || ret[1] <= 0 || ret[2] <= 0) {
            return false;
        }

        customerVector.push_back(
            Customer{fullnameArray.data(), emailArray.data(), "Antalya", birthdayArray.data()});
    }

    return true;
}

bool createDummyAccount(uuidType customerId, std::vector<Account>& accountVector, int32_t size)
{
    constexpr int32_t maxArrSize = 128;
    Rand<0, INT32_MAX> random;
    int ret[3]{};
    int32_t dateRange = (3600) * (24) * (365);

    for(int i = 0; i < size; ++i) {
        double balance = 100.225 + (random() % 125123);
        Datetime date{(Datetime::now() - dateRange) + (random() % dateRange)};

        accountVector.push_back(Account{customerId, balance, date});
    }

    return true;
}

bool createDummyTransaction(uuidType fromAccountId,
                            uuidType toAccountId,
                            std::vector<Transaction>& transactionVector,
                            int32_t size)
{
    constexpr int32_t maxArrSize = 128;
    Rand<0, INT32_MAX> random;
    int ret{};
    int32_t dateRange = (3600) * (24) * (365);
    std::string type = (fromAccountId && toAccountId)
                           ? "transfer"
                           : (fromAccountId ? "withdraw" : (toAccountId ? "deposit" : "invalid"));

    if(!type.compare("invalid")) {
        return false;
    }

    for(int i = 0; i < size; ++i) {
        std::array<char, maxArrSize> descArray{};
        double amount = 100.15 + (random() % 125);
        Datetime date{(Datetime::now() - dateRange) + (random() % dateRange)};
        ret = snprintf(descArray.data(), maxArrSize, "Dummy %s", type.c_str());

        if(ret <= 0) {
            return false;
        }

        transactionVector.push_back(
            Transaction{date, fromAccountId, toAccountId, descArray.data(), type, amount});
    }

    return true;
}

TEST(CustomerStorageTest, createCustomer)
{
    std::vector<Customer> customerVector;
    constexpr size_t customerCount = 10;
    Storage::CustomerMngr::customerDbEntry p;

    removeDb();

    ASSERT_TRUE(createDummyCustomer(customerVector, customerCount));
    for(int i = 0; i < customerCount; ++i) {
        ASSERT_NO_THROW(ASSERT_TRUE(Storage::CustomerMngr::insert(customerVector[i], p)));
        ASSERT_NE(p.first, 0);
        ASSERT_EQ(p.second, customerVector[i]);
    }
}

TEST(CustomerStorageTest, getCustomerByEmail)
{
    std::vector<Customer> customerVector;
    constexpr size_t customerCount = 10;

    Storage::CustomerMngr::customerDbEntry pInserted, pGot;

    ASSERT_TRUE(createDummyCustomer(customerVector, customerCount));
    for(int i = 0; i < customerCount; ++i) {
        ASSERT_NO_THROW(ASSERT_TRUE(Storage::CustomerMngr::insert(customerVector[i], pInserted)));
        ASSERT_NO_THROW(
            ASSERT_TRUE(Storage::CustomerMngr::getByEmail(customerVector[i].email(), pGot)));
        ASSERT_EQ(pGot.first, pInserted.first);
        ASSERT_EQ(pGot.second, pInserted.second);
    }
}

TEST(CustomerStorageTest, getCustomerByUsername)
{
    Credentials cred;
    std::vector<Customer> customerVector;
    constexpr size_t customerCount = 1;

    Storage::CustomerMngr::customerDbEntry pInserted, pGot;
    Storage::CredentialsMngr::credentialsDbEntry credInserted;

    ASSERT_TRUE(createDummyCustomer(customerVector, customerCount));
    ASSERT_NO_THROW(ASSERT_TRUE(Storage::CustomerMngr::insert(customerVector.back(), pInserted)));
    ASSERT_TRUE(cred.setCredentials("selen.gecgel", "GWerw1231..zxc!12312", std::cout));
    ASSERT_NO_THROW(
        ASSERT_TRUE(Storage::CredentialsMngr::insert(cred, pInserted.first, credInserted)));
    ASSERT_NO_THROW(ASSERT_TRUE(Storage::CustomerMngr::getByUsername(cred.username(), pGot)));
    ASSERT_EQ(pGot.first, pInserted.first);
    ASSERT_EQ(pGot.second, pInserted.second);
}

TEST(CustomerStorageTest, updateCustomer)
{
    std::vector<Customer> customerVector;
    constexpr size_t customerCount = 1;
    std::string updatedFullname = "Ramazan C";
    std::string updatedEmail = "ramco..1254@gmail.com";
    Storage::CustomerMngr::customerDbEntry pInserted, pGot, pUpdated;

    ASSERT_TRUE(createDummyCustomer(customerVector, customerCount));

    ASSERT_NO_THROW(ASSERT_TRUE(Storage::CustomerMngr::insert(customerVector.back(), pInserted)));
    ASSERT_NO_THROW(
        ASSERT_TRUE(Storage::CustomerMngr::update(pInserted.first,
                                                  Customer(updatedFullname,
                                                           updatedEmail,
                                                           customerVector.back().address(),
                                                           customerVector.back().birthday()),
                                                  pUpdated)));
    ASSERT_NO_THROW(ASSERT_TRUE(Storage::CustomerMngr::getByEmail(updatedEmail, pGot)));
    ASSERT_EQ(pGot.first, pUpdated.first);
    ASSERT_EQ(pGot.second, pUpdated.second);
}

TEST(CredentialsStorageTest, createCredentials)
{
    Credentials cred;
    uuidType id;
    Storage::CredentialsMngr::credentialsDbEntry credInserted;
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
    std::vector<Customer> customerVector;
    constexpr size_t customerCount = 1;
    std::string username = "customer.112";
    std::string password = "GASBcc2212..zxc!";
    Storage::CustomerMngr::customerDbEntry pInserted;
    Storage::CredentialsMngr::credentialsDbEntry credInserted, credGot;

    ASSERT_TRUE(createDummyCustomer(customerVector, customerCount));

    ASSERT_NO_THROW(ASSERT_TRUE(Storage::CustomerMngr::insert(customerVector.back(), pInserted)));
    ASSERT_TRUE(cred.setCredentials(username, password, std::cout));
    ASSERT_NO_THROW(
        ASSERT_TRUE(Storage::CredentialsMngr::insert(cred, pInserted.first, credInserted)));
    ASSERT_NO_THROW(
        ASSERT_TRUE(Storage::CredentialsMngr::getByCustomerId(pInserted.first, credGot)));
    ASSERT_EQ(credInserted.first, credGot.first);
    ASSERT_EQ(credInserted.second, credGot.second);
}

TEST(CredentialsStorageTest, getCredentialsByUsername)
{
    Credentials cred;
    std::vector<Customer> customerVector;
    constexpr size_t customerCount = 1;
    std::string username = "customer.2";
    std::string password = "GASBcc2212..zxc!";
    Storage::CustomerMngr::customerDbEntry pInserted;
    Storage::CredentialsMngr::credentialsDbEntry credInserted, credGot;

    ASSERT_TRUE(createDummyCustomer(customerVector, customerCount));

    ASSERT_NO_THROW(ASSERT_TRUE(Storage::CustomerMngr::insert(customerVector.back(), pInserted)));
    ASSERT_TRUE(cred.setCredentials(username, password, std::cout));
    ASSERT_NO_THROW(
        ASSERT_TRUE(Storage::CredentialsMngr::insert(cred, pInserted.first, credInserted)));
    ASSERT_NO_THROW(ASSERT_TRUE(Storage::CredentialsMngr::getByUsername(username, credGot)));
    ASSERT_EQ(credInserted.first, credGot.first);
    ASSERT_EQ(credInserted.second, credGot.second);
}

TEST(CredentialsStorageTest, updateCustomer)
{
    uuidType credId;
    Credentials cred;
    std::vector<Customer> customerVector;
    constexpr size_t customerCount = 1;
    std::string username = "customer.3";
    std::string password = "GASBcc2212..zxc!";
    std::string updatedUsername = "customer.3.updated";
    std::string updatedPassword = "UpdatdPass12!a";
    Credentials updatedCred(updatedUsername, updatedPassword);
    Storage::CustomerMngr::customerDbEntry pInserted;
    Storage::CredentialsMngr::credentialsDbEntry credInserted, credGot, credUpdated;

    ASSERT_TRUE(createDummyCustomer(customerVector, customerCount));
    ASSERT_TRUE(updatedCred.isValid(std::cout));
    ASSERT_NO_THROW(ASSERT_TRUE(Storage::CustomerMngr::insert(customerVector.back(), pInserted)));
    ASSERT_TRUE(cred.setCredentials(username, password, std::cout));
    ASSERT_NO_THROW(
        ASSERT_TRUE(Storage::CredentialsMngr::insert(cred, pInserted.first, credInserted)));
    ASSERT_NO_THROW(ASSERT_TRUE(
        Storage::CredentialsMngr::update(credInserted.first, updatedCred, credUpdated)));
    ASSERT_NO_THROW(ASSERT_TRUE(Storage::CredentialsMngr::getByUsername(updatedUsername, credGot)));
    ASSERT_EQ(credUpdated.first, credGot.first);
    ASSERT_EQ(credUpdated.second, credGot.second);
}

TEST(AccountStorageTest, createSingleAccount)
{
    std::vector<Customer> customerVector;
    std::vector<Account> accountVector;
    constexpr size_t accountCount = 1;
    constexpr size_t customerCount = 1;
    Storage::CustomerMngr::customerDbEntry cInserted;
    Storage::AccountMngr::accountDbEntry aInserted;

    ASSERT_TRUE(createDummyCustomer(customerVector, customerCount));
    ASSERT_NO_THROW(ASSERT_TRUE(Storage::CustomerMngr::insert(customerVector.back(), cInserted)));

    ASSERT_TRUE(createDummyAccount(cInserted.first, accountVector, accountCount));

    ASSERT_NO_THROW(ASSERT_TRUE(
        Storage::AccountMngr::insert(accountVector.back(), cInserted.first, aInserted)));
    ASSERT_NE(aInserted.first, 0);
    ASSERT_EQ(aInserted.second, accountVector.back());
}

TEST(AccountStorageTest, getMultipleAccount)
{
    std::vector<Customer> customerVector;
    std::vector<Account> accountVector;
    constexpr size_t accountCount = 10;
    constexpr size_t customerCount = 1;

    Storage::CustomerMngr::customerDbEntry cInserted;
    std::array<Storage::AccountMngr::accountDbEntry, accountCount> accountDbVector;
    std::vector<Storage::AccountMngr::accountDbEntry> aGotVector;

    ASSERT_TRUE(createDummyCustomer(customerVector, customerCount));
    ASSERT_NO_THROW(ASSERT_TRUE(Storage::CustomerMngr::insert(customerVector.back(), cInserted)));

    ASSERT_TRUE(createDummyAccount(cInserted.first, accountVector, accountCount));
    for(size_t i = 0; i < accountCount; ++i) {
        ASSERT_NO_THROW(ASSERT_TRUE(
            Storage::AccountMngr::insert(accountVector[i], cInserted.first, accountDbVector[i])));
    }

    ASSERT_NO_THROW(
        ASSERT_TRUE(Storage::AccountMngr::getByCustomerId(cInserted.first, aGotVector)));
    for(size_t i = 0; i < accountCount; ++i) {
        ASSERT_EQ(aGotVector[i].first, accountDbVector[i].first);
        ASSERT_EQ(aGotVector[i].second, accountDbVector[i].second);
    }
}

TEST(AccountStorageTest, updateAccount)
{
    std::vector<Customer> customerVector;
    std::vector<Account> accountVector;
    constexpr size_t accountCount = 1;
    constexpr size_t customerCount = 1;
    Account updated;

    Storage::CustomerMngr::customerDbEntry cInserted;
    Storage::AccountMngr::accountDbEntry aInserted, aUpdated;
    std::vector<Storage::AccountMngr::accountDbEntry> aGotVector;

    ASSERT_TRUE(createDummyCustomer(customerVector, customerCount));
    ASSERT_NO_THROW(ASSERT_TRUE(Storage::CustomerMngr::insert(customerVector.back(), cInserted)));

    ASSERT_TRUE(createDummyAccount(cInserted.first, accountVector, accountCount));
    ASSERT_NO_THROW(ASSERT_TRUE(
        Storage::AccountMngr::insert(accountVector.back(), cInserted.first, aInserted)));
    updated.setAttributes(cInserted.first, 23.34, Datetime::now());
    ASSERT_NO_THROW(ASSERT_TRUE(Storage::AccountMngr::update(aInserted.first, updated, aUpdated)));

    ASSERT_EQ(aInserted.first, aUpdated.first);
    ASSERT_EQ(updated, aUpdated.second);
}

TEST(TransactionTest, createTransaction)
{
    std::vector<Customer> customerVector;
    std::vector<Account> accountVector;
    std::vector<Transaction> transactionVector;

    Storage::CustomerMngr::customerDbEntry cInserted;
    Storage::AccountMngr::accountDbEntry aInserted;
    Storage::TransactionMngr::transactionDbEntry txInserted;

    ASSERT_TRUE(createDummyCustomer(customerVector, 1));
    ASSERT_NO_THROW(ASSERT_TRUE(Storage::CustomerMngr::insert(customerVector.back(), cInserted)));

    ASSERT_TRUE(createDummyAccount(cInserted.first, accountVector, 1));
    ASSERT_NO_THROW(ASSERT_TRUE(
        Storage::AccountMngr::insert(accountVector.back(), cInserted.first, aInserted)));

    ASSERT_TRUE(createDummyTransaction(0, aInserted.first, transactionVector, 1));
    ASSERT_NO_THROW(
        ASSERT_TRUE(Storage::TransactionMngr::insert(transactionVector.back(), txInserted)));

    ASSERT_NE(txInserted.first, 0);
    ASSERT_EQ(transactionVector.back(), txInserted.second);
}

TEST(TransactionTest, getTransaction)
{
    constexpr size_t customerCount = 1;
    constexpr size_t accountCount = 2;
    constexpr size_t transactionCount = 20;
    std::vector<Customer> customerVector;
    std::vector<Account> accountVector;

    std::vector<Storage::AccountMngr::accountDbEntry> accountDbVector;
    std::array<std::vector<Storage::TransactionMngr::transactionDbEntry>, accountCount>
        GetQueryTransactionDbVector;
    std::vector<Transaction> tmpTransactionVector;
    std::array<std::vector<Transaction>, accountCount> transactionVector;

    Storage::CustomerMngr::customerDbEntry cInserted;

    ASSERT_TRUE(createDummyCustomer(customerVector, customerCount));
    ASSERT_NO_THROW(ASSERT_TRUE(Storage::CustomerMngr::insert(customerVector.back(), cInserted)));

    ASSERT_TRUE(createDummyAccount(cInserted.first, accountVector, accountCount));
    accountDbVector.reserve(accountCount);
    for(int i = 0; i < accountCount; ++i) {
        ASSERT_NO_THROW(ASSERT_TRUE(
            Storage::AccountMngr::insert(accountVector[i], cInserted.first, accountDbVector[i])));
    }

    /**************** Deposit ***************/
    ASSERT_TRUE(createDummyTransaction(
        0, accountDbVector[0].first, tmpTransactionVector, transactionCount));
    for(auto& elem : tmpTransactionVector) {
        transactionVector[0].push_back(elem);
    }
    for(int i = 0; i < transactionCount; ++i) {
        ASSERT_NO_THROW(ASSERT_TRUE(Storage::TransactionMngr::insert(transactionVector[0][i])));
    }
    ASSERT_NO_THROW(ASSERT_TRUE(Storage::TransactionMngr::getByAccountId(
        accountDbVector[0].first, GetQueryTransactionDbVector[0])));
    for(int i = 0; i < transactionCount; ++i) {
        ASSERT_EQ(transactionVector[0][i], GetQueryTransactionDbVector[0][i].second);
    }

    /**************** Transfer ***************/
    tmpTransactionVector.clear();
    ASSERT_TRUE(createDummyTransaction(accountDbVector[0].first,
                                       accountDbVector[1].first,
                                       tmpTransactionVector,
                                       transactionCount));

    // // copy tx elems
    for(auto& elem : tmpTransactionVector) {
        transactionVector[0].push_back(elem);
        transactionVector[1].push_back(elem);
    }

    for(auto& elem : transactionVector[1]) {
        std::cout << elem << "\n";
    }
    for(int i = 0; i < transactionCount; ++i) {
        ASSERT_NO_THROW(ASSERT_TRUE(Storage::TransactionMngr::insert(transactionVector[1][i])));
    }
    ASSERT_NO_THROW(ASSERT_TRUE(Storage::TransactionMngr::getByAccountId(
        accountDbVector[0].first, GetQueryTransactionDbVector[0])));
    for(int i = 0; i < transactionCount; ++i) {
        ASSERT_EQ(transactionVector[0][i], GetQueryTransactionDbVector[0][i].second);
    }
    ASSERT_NO_THROW(ASSERT_TRUE(Storage::TransactionMngr::getByAccountId(
        accountDbVector[1].first, GetQueryTransactionDbVector[1])));
    for(int i = 0; i < transactionCount; ++i) {
        ASSERT_EQ(transactionVector[1][i], GetQueryTransactionDbVector[1][i].second);
    }
}
