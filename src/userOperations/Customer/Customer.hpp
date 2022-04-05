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
    struct CustomerInfo
    {
        std::string fullname;
        std::string email;
        std::string address;
        std::string birthday;
    };

    Customer() = default;
    Customer(const CustomerInfo&);
    uuidType id(void);
    std::string fullname(void);
    std::string email(void);
    std::string address(void);
    std::string birthday(void);
    void setInfo(const CustomerInfo&);
    bool store(void);

    static bool find(uuidType customerId,
                     Customer& outCustomer); // id will be returned from credential verification.
    static bool edit(uuidType, const CustomerInfo&);
    static bool create(const CustomerInfo&, Customer&);

private:
    uuidType id_;
    CustomerInfo info_;
    Credentials loginCred_;
    AccountList accounts_;
    Datetime registerDate_;
};

#endif