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
bool insert(const Customer&, uuidType&);
bool getByEmail(const std::string&, uuidType&);
bool getByUsername(const std::string&, uuidType&);
bool update(uuidType, const Customer&);
} // namespace CustomerMngr

namespace AccountMngr
{
bool insert(const Account& account, int32_t customerId);
}

namespace TransactionMngr
{
}

namespace CredentialsMngr
{
bool insert(const Credentials& credentials, int32_t customerId, uuidType&);
bool getByCustomerId(int32_t customerId, uuidType&);
bool getByUsername(const std::string& username, uuidType&);
bool update(uuidType, const Credentials&);

} // namespace CredentialsMngr

} // namespace Storage

#endif