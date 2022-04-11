#ifndef STORAGE_HPP
#define STORAGE_HPP

#include <string>

#include <userOperations/Account/Account.hpp>
#include <userOperations/Credentials/Credentials.hpp>
#include <userOperations/Customer/Customer.hpp>

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
bool insert(const Customer&, std::pair<uuidType, Customer>&);
bool getByEmail(const std::string&, std::pair<uuidType, Customer>&);
bool getByUsername(const std::string&, std::pair<uuidType, Customer>&);
bool update(uuidType, const Customer&, std::pair<uuidType, Customer>&);
} // namespace CustomerMngr

namespace AccountMngr
{
bool insert(const Account& account, int32_t customerId, std::pair<uuidType, Account>&);
bool getByCustomerId(int32_t customerId, std::vector<std::pair<uuidType, Account>>&);

} // namespace AccountMngr

namespace TransactionMngr
{
}

namespace CredentialsMngr
{
bool insert(const Credentials& credentials, int32_t customerId, std::pair<uuidType, Credentials>&);
bool getByCustomerId(int32_t customerId, std::pair<uuidType, Credentials>&);
bool getByUsername(const std::string& username, std::pair<uuidType, Credentials>&);
bool update(uuidType, const Credentials&, std::pair<uuidType, Credentials>&);

} // namespace CredentialsMngr

} // namespace Storage

#endif