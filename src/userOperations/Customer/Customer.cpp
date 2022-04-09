#include <userOperations/Customer/Customer.hpp>

Customer::Customer(const std::string& fullname,
                   const std::string& email,
                   const std::string& address,
                   const std::string& birthday)
    : fullname_(fullname)
    , email_(email)
    , address_(address)
    , birthday_(birthday)
{ }

std::string Customer::fullname() const
{
    return fullname_;
}

std::string Customer::address() const
{
    return address_;
}

std::string Customer::email() const
{
    return email_;
}

std::string Customer::birthday() const
{
    return birthday_;
}

void Customer::setInfo(const std::string& fullname,
                       const std::string& email,
                       const std::string& address,
                       const std::string& birthday)
{
    fullname_ = fullname;
    address_ = address;
    birthday_ = birthday;
    email_ = email;
}
