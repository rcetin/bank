#ifndef ACCOUNT_HPP
#define ACCOUNT_HPP

#include <string>

#include <userOperations/Customer/Customer.hpp>
#include <utils/Datetime.hpp>
#include <utils/Rand.hpp>

class Account
{
public:
    Account() = default;
    uuidType id();
    std::string owner();
    int32_t balance();
    std::string openDate();
    std::string dumpDetails();
    std::string dumpTxHistory();
    bool deposit(int32_t);
    bool withdraw(int32_t);

    using AccountList = std::vector<Account>;

    static bool find(uuidType customerId, AccountList& outAccounts);
    static bool create(uuidType customerId, Account& outAccount);

private:
    uuidType id_;
    int32_t balance_;
    Customer owner_;
    uuidType ownerId_;
    Datetime openDate_;
    bool isBlocked_;
};

#endif
