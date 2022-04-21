#ifndef USER_OPS_HPP
#define USER_OPS_HPP

#include <iostream>
#include <string>
#include <unordered_map>

#include <userOperations/Customer/Customer.hpp>
#include <userOperations/Storage/Storage.hpp>

bool createCustomer(std::istream& in, std::ostream& out, Customer& customer);
bool createCredentials(std::istream& in, std::ostream& out, Credentials& credentials);

struct User
{
    bool isLoggedIn_ = false;
    Storage::CustomerMngr::customerDbEntry customer_;
    Storage::CredentialsMngr::credentialsDbEntry credentials_;
    std::vector<Storage::AccountMngr::accountDbEntry> accounts_;
    std::vector<Storage::TransactionMngr::transactionDbEntry> transactions_;
};

class UserOps
{
public:
    UserOps(std::istream& in, std::ostream& out)
        : in_(in)
        , out_(out)
    { }
    bool run(const std::string& op)
    {
        if(op == "noOp") {
            return false;
        }

        ops.at(op)(*this);
        return true;
    }

    std::istream& in()
    {
        return in_;
    }

    std::ostream& out()
    {
        return out_;
    }

    User& user()
    {
        return user_;
    }

private:
    static const std::unordered_map<std::string, void (*)(UserOps&)> ops;
    std::istream& in_;
    std::ostream& out_;
    User user_;
};

#endif