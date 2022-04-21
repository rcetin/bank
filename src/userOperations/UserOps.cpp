#include <algorithm>
#include <cstdio>
#include <iostream>
#include <regex>
#include <string>
#include <termios.h>
#include <unistd.h>
#include <unordered_map>

#include <userInput/StringInput.hpp>
#include <userOperations/Account/Account.hpp>
#include <userOperations/Credentials/Credentials.hpp>
#include <userOperations/Customer/Customer.hpp>
#include <userOperations/Storage/Storage.hpp>
#include <userOperations/UserOps.hpp>

static void fnCreateCustomer(UserOps&);
static void fnLogin(UserOps&);
static void fnQuit(UserOps&);
static void fnCreateAccount(UserOps&);
static void fnDeleteAccount(UserOps&);
static void fnListAccounts(UserOps&);
static void fnAccountDetails(UserOps&);
static void fnChangeCustomerInfo(UserOps&);
static void fnChangePassword(UserOps&);

constexpr int32_t maxNumberOfUserTrials = 5;

const std::unordered_map<std::string, void (*)(UserOps&)> UserOps::ops = {
    {"fnCreateCustomer", fnCreateCustomer},
    {"fnLogin", fnLogin},
    {"fnQuit", fnQuit},
    {"fnCreateAccount", fnCreateAccount},
    {"fnDeleteAccount", fnDeleteAccount},
    {"fnListAccounts", fnListAccounts},
    {"fnAccountDetails", fnAccountDetails},
    {"fnChangeCustomerInfo", fnChangeCustomerInfo},
    {"fnChangePassword", fnChangePassword}};

static void quit()
{
    std::exit(EXIT_SUCCESS);
}

template <typename Type>
static bool getInput(std::istream& in,
                     std::ostream& out,
                     const std::string& promptLog,
                     Input<Type>& outUserInput)
{
    int32_t numberOfFails = 0;
    std::string input;

    while(numberOfFails++ < maxNumberOfUserTrials) {
        out << promptLog;
        outUserInput.clear();
        std::getline(in, input);
        if(in.bad()) {
            out << "Invalid input is given. Try again...\n";
            continue;
        }

        outUserInput.set(input);
        if(!outUserInput.isValid(out)) {
            continue;
        }

        return true;
    }

    return false;
}

bool createCustomer(std::istream& in, std::ostream& out, Customer& customer)
{
    StringInput fullname, email, address, birthday;
    // std::string input, fullname, email, address, birthday;
    // auto removeWhiteSpaceRegex = std::regex("^ +| +$|( ) +");

    std::cout << "Create Customer Details:\n\n";
    // std::cout << "\nFullname, e-mail, address, birthday: ";

    if(!getInput(in, out, "Fullname: ", fullname) || !getInput(in, out, "Email: ", email) ||
       !getInput(in, out, "Address: ", address) ||
       !getInput(in, out, "Birthday (dd/mm/yyyy): ", birthday)) {
        out << "Proper input is not entered, quitting...\n";
        quit();
    }
    customer.setInfo(fullname, email, address, birthday);

    return true;
}

static bool
getPasswordInput(std::ostream& out, const std::string& promptLog, PasswordInput& outPassword)
{
    int32_t numberOfFails = 0;
    struct termios old_tio, new_tio;
    char c;
    size_t currentIdx = 0;
    std::string passwordInput;

    /* get the terminal settings for stdin */
    tcgetattr(STDIN_FILENO, &old_tio);
    new_tio = old_tio;
    new_tio.c_lflag &= (~ICANON & ~ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);

    while(numberOfFails++ < maxNumberOfUserTrials) {
        out << promptLog;
        passwordInput.clear();
        do {
            c = getchar();

            if(c == 127) { // DEL
                if(currentIdx) {
                    out << "\b \b";
                    currentIdx--;
                    passwordInput.pop_back();
                }
                continue;
            }

            if(c != '\n') { // del
                out << '*';
                currentIdx++;
                passwordInput.push_back(c);
            }

        } while(c != '\n');

        out << "\n";

        outPassword.set(passwordInput);
        if(!outPassword.isValid()) {
            continue;
        }

        break;
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);

    return (numberOfFails >= maxNumberOfUserTrials) ? false : true;
}

bool createCredentials(std::istream& in, std::ostream& out, Credentials& credentials)
{
    UsernameInput username;
    PasswordInput password;

    std::cout << "Create Customer Credentials:\n\n";

    if(!getInput(in, out, "Username: ", username) ||
       !getPasswordInput(out, "Password: ", password)) {
        goto quit;
    }

    credentials.setCredentials(username.data(), password.data());
    return true;
quit:
    out << "Proper input is not entered, quitting...\n";
    quit();
    return false;
}

void fnLogin(UserOps& userOps)
{
    StringInput username;
    PasswordInput password;
    Storage::CredentialsMngr::credentialsDbEntry credEntry;
    Storage::CustomerMngr::customerDbEntry customerEntry;
    Credentials cred;
    int32_t numberOfTrials = 0;

    userOps.out() << "Login:\n\n";
    if(!getInput(userOps.in(), userOps.out(), "Username: ", username) ||
       !getPasswordInput(userOps.out(), "Password: ", password)) {
        userOps.out() << "Proper input is not entered, quitting...\n";
        quit();
    }

    if(!Storage::CredentialsMngr::getByUsername(username, credEntry)) {
        userOps.out() << "User cannot be found...\n";
        quit();
    }

    if(credEntry.second != Credentials{username, password}) {
        userOps.out() << "Username or Password is wrong...\n";
        quit();
    }

    if(!Storage::CustomerMngr::getByUsername(username, customerEntry)) {
        userOps.out() << "Error: Customer cannot be found...\n";
        quit();
    }

    userOps.user().isLoggedIn_ = true;
    userOps.user().customer_ = customerEntry;
    userOps.user().credentials_ = credEntry;

    userOps.out() << "Logged in successfully.\n";
}

void fnQuit(UserOps& userOps)
{
    userOps.out() << "Quitting the application...\nHave a nice day!\n\n";
    quit();
}

void fnCreateCustomer(UserOps& userOps)
{
    Customer c;
    Credentials cred;
    Storage::CustomerMngr::customerDbEntry insertedC;

    userOps.out() << "Open Customer Account:\n";

    if(!createCustomer(userOps.in(), userOps.out(), c)) {
        userOps.out() << "Customer Account could not be created\n";
        return;
    }

    if(!Storage::CustomerMngr::insert(c, insertedC)) {
        userOps.out() << "Customer cannot be stored in database\n";
        return;
    }

    if(!createCredentials(userOps.in(), userOps.out(), cred)) {
        return;
    }

    if(!Storage::CredentialsMngr::insert(cred, insertedC.first)) {
        return;
    }

    userOps.out() << "Customer is created successfully.\n";
    userOps.out() << "Please login to continue...\n";
}

void fnCreateAccount(UserOps& userOps) { }

void fnDeleteAccount(UserOps&)
{
    std::cout << "Call: fnDeleteAccount\n";
}

void fnListAccounts(UserOps&)
{
    std::cout << "Call: fnListAccounts\n";
}

void fnAccountDetails(UserOps&)
{
    std::cout << "Call: fnAccountDetails\n";
}

void fnChangeCustomerInfo(UserOps&)
{
    std::cout << "Call: fnChangeCustomerInfo\n";
}

void fnChangePassword(UserOps&)
{
    std::cout << "Call: fnChangePassword\n";
}