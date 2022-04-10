#ifndef CUSTOMER_HPP
#define CUSTOMER_HPP

#include <string>

#include <userOperations/Account/Account.hpp>
#include <userOperations/Credentials/Credentials.hpp>
#include <utils/Datetime.hpp>
#include <utils/Rand.hpp>

class Customer
{
public:
    Customer() = default;
    Customer(const std::string& fullname,
             const std::string& email,
             const std::string& address,
             const std::string& birthday);
    // uuidType id(void);
    std::string fullname(void) const;
    std::string email(void) const;
    std::string address(void) const;
    std::string birthday(void) const;
    void setInfo(const std::string& fullname,
                 const std::string& email,
                 const std::string& address,
                 const std::string& birthday);

private:
    // uuidType id_;
    std::string fullname_;
    std::string address_;
    std::string email_;
    std::string birthday_;
    // Account::AccountList accounts_;
    Datetime registerDate_;
};

#endif