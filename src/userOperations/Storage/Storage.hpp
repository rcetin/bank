#ifndef STORAGE_HPP
#define STORAGE_HPP

#include <string>

#include <userOperations/Account/Account.hpp>
#include <userOperations/Credentials/Credentials.hpp>
#include <userOperations/Customer/Customer.hpp>
#include <userOperations/Transaction/Transaction.hpp>

namespace Storage
{

#ifndef TESTING
static const char* dbFile = "/home/rcetin/workspace/repos/bank/bank.db";
#else
static const char* dbFile =
    "/home/rcetin/workspace/repos/bank/tests/unit_tests/userOperations/unit_test.db";
#endif

constexpr int32_t STORAGE_ERROR = -1;

// bool removeDb(void);

namespace CustomerMngr
{
using customerDbEntry = std::pair<uuidType, Customer>;
bool insert(const Customer&);
bool insert(const Customer&, customerDbEntry&);
bool getByEmail(const std::string&, customerDbEntry&);
bool getByUsername(const std::string&, customerDbEntry&);
bool update(uuidType, const Customer&);
bool update(uuidType, const Customer&, customerDbEntry&);

} // namespace CustomerMngr

namespace AccountMngr
{
using accountDbEntry = std::pair<uuidType, Account>;
bool insert(const Account& account, int32_t customerId);
bool insert(const Account& account, int32_t customerId, accountDbEntry&);
bool getByCustomerId(int32_t customerId, std::vector<accountDbEntry>&);
bool update(uuidType, const Account&);
bool update(uuidType, const Account&, accountDbEntry&);

} // namespace AccountMngr

namespace TransactionMngr
{
using transactionDbEntry = std::pair<uuidType, Transaction>;
bool insert(const Transaction& account);
bool insert(const Transaction& account, transactionDbEntry&);
bool getByAccountId(int32_t accountId, std::vector<transactionDbEntry>&);
bool update(uuidType, const Transaction&);
bool update(uuidType, const Transaction&, transactionDbEntry&);
} // namespace TransactionMngr

namespace CredentialsMngr
{
using credentialsDbEntry = std::pair<uuidType, Credentials>;
bool insert(const Credentials& credentials, int32_t customerId);
bool insert(const Credentials& credentials, int32_t customerId, credentialsDbEntry&);
bool getByCustomerId(int32_t customerId, credentialsDbEntry&);
bool getByUsername(const std::string& username, credentialsDbEntry&);
bool update(uuidType, const Credentials&);
bool update(uuidType, const Credentials&, credentialsDbEntry&);

} // namespace CredentialsMngr

} // namespace Storage

#endif