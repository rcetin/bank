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
    Account(uuidType ownerId, double balance, const Datetime& openDate);

    void setAttributes(uuidType ownerId, double balance, const Datetime& openDate);
    void setOwnerId(uuidType);
    void setBalance(double);
    uuidType ownerId() const;
    double balance() const;
    Datetime openDate() const;
    std::string dumpDetails() const;
    std::string dumpTxHistory() const;
    bool deposit(double);
    bool withdraw(double);

    friend bool operator==(const Account&, const Account&);
    friend bool operator!=(const Account&, const Account&);
    friend std::ostream& operator<<(std::ostream&, const Account&);

    using AccountList = std::vector<Account>;

private:
    uuidType id_;
    double balance_ = 0;
    // Customer owner_;
    uuidType ownerId_;
    Datetime openDate_;
    bool isBlocked_;
};

#endif
