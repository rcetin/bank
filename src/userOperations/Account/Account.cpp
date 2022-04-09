#include <userOperations/Account/Account.hpp>

Account::Account(uuidType ownerId)
{
    ownerId_ = ownerId;
    balance_ = 0;
}

void Account::setOwnerId(uuidType ownerId)
{
    ownerId_ = ownerId;
}

int32_t Account::balance() const
{
    return balance_;
}

std::string Account::openDate() const
{
    return openDate_.dump();
}