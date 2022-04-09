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
    Account(uuidType ownerId);

    void setOwnerId(uuidType);
    uuidType id() const;
    std::string owner() const;
    int32_t balance() const;
    std::string openDate() const;
    std::string dumpDetails() const;
    std::string dumpTxHistory() const;
    bool deposit(int32_t);
    bool withdraw(int32_t);

    using AccountList = std::vector<Account>;

    static bool find(uuidType customerId, AccountList& outAccounts);
    static bool create(uuidType customerId, Account& outAccount);
    static bool del(uuidType accountId);

private:
    uuidType id_;
    int32_t balance_ = 0;
    // Customer owner_;
    uuidType ownerId_;
    Datetime openDate_;
    bool isBlocked_;
};

#endif
