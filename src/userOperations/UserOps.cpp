#include <algorithm>
#include <cstdio>
#include <iostream>
#include <regex>
#include <string>
#include <termios.h>
#include <unistd.h>
#include <unordered_map>

#include <userInput/IntegerInput.hpp>
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
static void fnTransferMoney(UserOps& userOps);
static void fnDepositMoney(UserOps& userOps);
static void fnWithdrawMoney(UserOps& userOps);
static void fnGoBack(UserOps& userOps);
static void noOp(UserOps& userOps);

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
    {"fnChangePassword", fnChangePassword},
    {"fnTransferMoney", fnTransferMoney},
    {"fnDepositMoney", fnDepositMoney},
    {"fnWithdrawMoney", fnWithdrawMoney},
    {"fnGoBack", fnGoBack},
    {"noOp", noOp}};

static void quit()
{
    std::exit(EXIT_SUCCESS);
}

static void goBackInMenu(size_t level, UserSchema& schema)
{
    for(size_t i = 0; i < level; ++i) {
        schema.goBack();
    }
}

static void goRootInMenu(UserSchema& schema)
{
    schema.reset();
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

    std::cout << "Create Customer Details:\n\n";

    if(!getInput(in, out, "Fullname: ", fullname) || !getInput(in, out, "Email: ", email) ||
       !getInput(in, out, "Address: ", address) ||
       !getInput(in, out, "Birthday (dd/mm/yyyy): ", birthday)) {
        out << "Proper input is not entered, quitting...\n";
        quit();
    }
    customer.setInfo(fullname, email, address, birthday);

    return true;
}

static bool getPasswordInput(std::ostream& out,
                             const std::string& promptLog,
                             bool validate,
                             PasswordInput& outPassword)
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
        if(validate && !outPassword.isValid(out)) {
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
       !getPasswordInput(out, "Password: ", true, password)) {
        goto quit;
    }

    credentials.setCredentials(username.data(), password.data());
    return true;
quit:
    out << "Proper input is not entered, quitting...\n";
    quit();
    return false;
}

void fnGoBack(UserOps& userOps)
{
    goBackInMenu(2, userOps.schema());
}

void fnLogin(UserOps& userOps)
{
    StringInput username;
    PasswordInput password;
    Storage::CredentialsMngr::credentialsDbEntry credEntry;
    Storage::CustomerMngr::customerDbEntry customerEntry;
    std::vector<Storage::AccountMngr::accountDbEntry> accounts;
    Credentials cred;
    int32_t numberOfTrials = 0;

    userOps.out() << "Login:\n\n";
    if(!getInput(userOps.in(), userOps.out(), "Username: ", username) ||
       !getPasswordInput(userOps.out(), "Password: ", false, password)) {
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

    if(Storage::AccountMngr::getByCustomerId(customerEntry.first, accounts)) {
        userOps.user().accounts_ = accounts;
    }

    userOps.out() << "Logged in successfully.\n";
    userOps.schema().goBack();
}

void fnQuit(UserOps& userOps)
{
    userOps.out() << "Quitting the application...\nHave a nice day!\n\n";
    quit();
}

void noOp(UserOps& userOps)
{
    return;
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
    goRootInMenu(userOps.schema());
}

void fnCreateAccount(UserOps& userOps)
{
    Account userAccount;

    if(!userOps.user().isLoggedIn_) {
        userOps.out() << "User is not logged in!\n";
        quit();
    }

    userAccount.setAttributes(userOps.user().customer_.first, 0, Datetime::now());
    if(!Storage::AccountMngr::insert(userAccount, userOps.user().customer_.first)) {
        userOps.out() << "User account cannot be created.\n";
        quit();
    }

    userOps.out() << "User account is created successfully.\n";
    goBackInMenu(1, userOps.schema());
}

void fnDeleteAccount(UserOps& userOps)
{
    IntegerInput accountIdx;
    StringInput input;
    std::vector<Storage::AccountMngr::accountDbEntry> accountList;
    size_t idx = 0;

    if(!userOps.user().isLoggedIn_) {
        userOps.out() << "User is not logged in!\n";
        quit();
    }

    if(!Storage::AccountMngr::getByCustomerId(userOps.user().customer_.first, accountList)) {
        userOps.out() << "Database operation error.\n";
        quit();
    }

    userOps.out() << "      Balance       Open Date\n";
    for(auto& accountEntry : accountList) {
        userOps.out() << "[" << idx++ << "]     " << accountEntry.second.balance() << "         "
                      << accountEntry.second.openDate().dump() << "\n";
    }

    if(!getInput(userOps.in(), userOps.out(), "Select account to delete: ", input)) {
        userOps.out() << "Proper input is not entered, quitting...\n";
        quit();
    }

    accountIdx.set(input.data());
    if(!Storage::AccountMngr::del(accountList[accountIdx].first)) {
        userOps.out() << "Database operation error.\n";
        quit();
    }

    userOps.out() << "Account is deleted successfully\n";
    goBackInMenu(1, userOps.schema());
}

void fnListAccounts(UserOps& userOps)
{
    std::vector<Storage::AccountMngr::accountDbEntry> accountList;

    if(!userOps.user().isLoggedIn_) {
        userOps.out() << "User is not logged in!\n";
        quit();
    }

    if(!Storage::AccountMngr::getByCustomerId(userOps.user().customer_.first, accountList)) {
        userOps.out() << "Database operation error.\n";
        quit();
    }

    userOps.out() << "Account Id      Balance       Open Date\n";
    for(auto& accountEntry : accountList) {
        userOps.out() << "[" << accountEntry.first << "]             "
                      << accountEntry.second.balance() << "         "
                      << accountEntry.second.openDate().dump() << "\n";
    }
    goBackInMenu(1, userOps.schema());
}

void fnAccountDetails(UserOps& userOps)
{
    IntegerInput accountIdx;
    std::vector<Storage::AccountMngr::accountDbEntry> accountList;
    std::vector<Storage::TransactionMngr::transactionDbEntry> transactionList;
    size_t idx = 0;

    if(!userOps.user().isLoggedIn_) {
        userOps.out() << "User is not logged in!\n";
        quit();
    }

    if(!Storage::AccountMngr::getByCustomerId(userOps.user().customer_.first, accountList)) {
        userOps.out() << "Database operation error.\n";
        quit();
    }

    userOps.out() << "      Balance       Open Date\n";
    for(auto& accountEntry : accountList) {
        userOps.out() << "[" << idx++ << "]     " << accountEntry.second.balance() << "         "
                      << accountEntry.second.openDate().dump() << "\n";
    }

    if(!getInput(userOps.in(), userOps.out(), "\nSelect account to show details: ", accountIdx) ||
       accountIdx >= idx) {
        userOps.out() << "Proper input is not entered, quitting...\n";
        quit();
    }

    userOps.out() << "Balance: " << accountList[accountIdx].second.balance()
                  << "\nOpen Date: " << accountList[accountIdx].second.openDate().dump() << "\n\n";

    if(Storage::TransactionMngr::getByAccountId(accountList[accountIdx].first, transactionList)) {
        userOps.out() << "Transactions:\n";
        idx = 0;
        for(auto& transaction : transactionList) {
            userOps.out() << "[" << idx++ << "] " << transaction.second << "\n";
        }
    }

    goBackInMenu(1, userOps.schema());
}

void fnChangeCustomerInfo(UserOps&)
{
    std::cout << "Call: fnChangeCustomerInfo\n";
}

void fnChangePassword(UserOps&)
{
    std::cout << "Call: fnChangePassword\n";
}

void fnTransferMoney(UserOps& userOps)
{
    StringInput amountInput, description;
    IntegerInput from, to;
    double amount;
    Storage::AccountMngr::accountDbEntry senderAccount, receiverAccount;

    if(!userOps.user().isLoggedIn_) {
        userOps.out() << "User is not logged in!\n";
        quit();
    }

    std::cout << "Please enter transfer details:\n\n";

    if(!getInput(userOps.in(), userOps.out(), "Sender Account Id: ", from) ||
       !getInput(userOps.in(), userOps.out(), "Receiver Account Id: ", to) ||
       !getInput(userOps.in(), userOps.out(), "Amount: ", amountInput) ||
       !getInput(userOps.in(), userOps.out(), "Description: ", description)) {
        userOps.out() << "Proper input is not entered, quitting...\n";
        quit();
    }

    amount = std::stod(amountInput);
    if(!Storage::AccountMngr::getByAccountId(from, senderAccount) ||
       !Storage::AccountMngr::getByAccountId(to, receiverAccount) ||
       senderAccount.second.ownerId() != userOps.user().customer_.first) {
        userOps.out() << "Account could not be found.\n";
        quit();
    }

    if(senderAccount.second.balance() < amount) {
        userOps.out() << "Account balance is not enough. Balance: "
                      << senderAccount.second.balance() << "\n";
        quit();
    }

    Transaction tx(Datetime::now(), from, to, description, "transfer", amount);
    if(!Storage::TransactionMngr::insert(tx)) {
        userOps.out() << "Transaction cannot be stored.\n";
        quit();
    }

    userOps.out() << "Transaction is completed successfully.\n";
    goBackInMenu(1, userOps.schema());
}

void fnDepositMoney(UserOps& userOps)
{
    StringInput amountInput, description;
    IntegerInput to;
    double amount;
    Storage::AccountMngr::accountDbEntry receiverAccount;

    if(!userOps.user().isLoggedIn_) {
        userOps.out() << "User is not logged in!\n";
        quit();
    }

    std::cout << "Please enter deposit details:\n\n";

    if(!getInput(userOps.in(), userOps.out(), "Account Id: ", to) ||
       !getInput(userOps.in(), userOps.out(), "Amount: ", amountInput) ||
       !getInput(userOps.in(), userOps.out(), "Description: ", description)) {
        userOps.out() << "Proper input is not entered, quitting...\n";
        quit();
    }

    amount = std::stod(amountInput);
    if(!Storage::AccountMngr::getByAccountId(to, receiverAccount)) {
        userOps.out() << "Account could not be found.\n";
        quit();
    }

    Transaction tx(Datetime::now(), 0, to, description, "deposit", amount);
    if(!Storage::TransactionMngr::insert(tx)) {
        userOps.out() << "Transaction cannot be stored.\n";
        quit();
    }

    userOps.out() << "Transaction is completed successfully.\n";
    goBackInMenu(1, userOps.schema());
}

void fnWithdrawMoney(UserOps& userOps)
{
    StringInput amountInput, description;
    IntegerInput from;
    double amount;
    Storage::AccountMngr::accountDbEntry senderAccount;

    if(!userOps.user().isLoggedIn_) {
        userOps.out() << "User is not logged in!\n";
        quit();
    }

    std::cout << "Please enter withdraw details:\n\n";

    if(!getInput(userOps.in(), userOps.out(), "Account Id: ", from) ||
       !getInput(userOps.in(), userOps.out(), "Amount: ", amountInput) ||
       !getInput(userOps.in(), userOps.out(), "Description: ", description)) {
        userOps.out() << "Proper input is not entered, quitting...\n";
        quit();
    }

    amount = std::stod(amountInput);
    if(!Storage::AccountMngr::getByAccountId(from, senderAccount) ||
       senderAccount.second.ownerId() != userOps.user().customer_.first) {
        userOps.out() << "Account could not be found.\n";
        quit();
    }

    if(senderAccount.second.balance() < amount) {
        userOps.out() << "Account balance is not enough. Balance: "
                      << senderAccount.second.balance() << "\n";
        quit();
    }

    Transaction tx(Datetime::now(), from, 0, description, "withdraw", amount);
    if(!Storage::TransactionMngr::insert(tx)) {
        userOps.out() << "Transaction cannot be stored.\n";
        quit();
    }

    userOps.out() << "Transaction is completed successfully.\n";
    goBackInMenu(1, userOps.schema());
}
