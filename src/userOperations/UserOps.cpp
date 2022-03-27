#include <iostream>
#include <string>
#include <unordered_map>

#include <UserOps.hpp>

static void fnCreateAccount(void);
static void fnDeleteAccount(void);
static void fnListAccounts(void);
static void fnAccountDetails(void);
static void fnChangeCustomerInfo(void);
static void fnChangePassword(void);

const std::unordered_map<std::string, void (*)(void)> UserOps::ops = {
    {"fnCreateAccount", fnCreateAccount},
    {"fnDeleteAccount", fnDeleteAccount},
    {"fnListAccounts", fnListAccounts},
    {"fnAccountDetails", fnAccountDetails},
    {"fnChangeCustomerInfo", fnChangeCustomerInfo},
    {"fnChangePassword", fnChangePassword}};

void fnCreateAccount(void)
{
    std::cout << "Call: fnCreateAccount\n";
}

void fnDeleteAccount(void)
{
    std::cout << "Call: fnDeleteAccount\n";
}

void fnListAccounts(void)
{
    std::cout << "Call: fnListAccounts\n";
}

void fnAccountDetails(void)
{
    std::cout << "Call: fnAccountDetails\n";
}

void fnChangeCustomerInfo(void)
{
    std::cout << "Call: fnChangeCustomerInfo\n";
}

void fnChangePassword(void)
{
    std::cout << "Call: fnChangePassword\n";
}