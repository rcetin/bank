#include <array>
#include <cstdio>

#include <userOperations/Account/Account.hpp>

constexpr int32_t maxDumpArraySize = 512;

Account::Account(uuidType ownerId)
{
    ownerId_ = ownerId;
    balance_ = 0;
    openDate_ = Datetime::now();
}

Account::Account(uuidType ownerId, double balance, const Datetime& openDate)
{
    ownerId_ = ownerId;
    balance_ = balance;
    openDate_ = openDate;
}

void Account::setAttributes(uuidType ownerId, double balance, const Datetime& openDate)
{
    ownerId_ = ownerId;
    balance_ = balance;
    openDate_ = openDate;
}

void Account::setOwnerId(uuidType ownerId)
{
    ownerId_ = ownerId;
}

double Account::balance() const
{
    return balance_;
}

Datetime Account::openDate() const
{
    return openDate_;
}

uuidType Account::ownerId() const
{
    return ownerId_;
}

std::string Account::dumpDetails() const
{
    std::array<char, maxDumpArraySize> details{};
    int ret;

    ret = std::snprintf(details.data(),
                        maxDumpArraySize,
                        "OwnerId = %d, Balance = %lf, Open Date = %s",
                        ownerId_,
                        balance_,
                        openDate_.dump().c_str());

    if(ret <= 0) {
        return "";
    }

    return details.data();
}

bool operator==(const Account& lhs, const Account& rhs)
{
    return (lhs.balance() == rhs.balance()) && (lhs.ownerId() == rhs.ownerId()) &&
           (lhs.openDate() == rhs.openDate());
}

bool operator!=(const Account& lhs, const Account& rhs)
{
    return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& out, const Account& account)
{
    return out << account.dumpDetails();
}