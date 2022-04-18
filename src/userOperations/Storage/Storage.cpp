#include <array>
#include <cstdio>
#include <iostream>
#include <string>

#include <SQLiteCpp/SQLiteCpp.h>
#include <userOperations/Storage/Storage.hpp>

static constexpr size_t maxSqlCommandLen = 512;

static const char* createCustomerTableCmd = "CREATE TABLE IF NOT EXISTS [Customers]"
                                            "("
                                            "[customerId] INTEGER PRIMARY KEY NOT NULL,"
                                            "[fullname] TEXT NOT NULL,"
                                            "[email] TEXT NOT NULL UNIQUE,"
                                            "[address] TEXT NOT NULL,"
                                            "[birthday] TEXT"
                                            ")";

static const char* createCredentialsTableCmd =
    "CREATE TABLE IF NOT EXISTS [Credentials]"
    "("
    "[credentialId] INTEGER PRIMARY KEY NOT NULL,"
    "[customerId] INTEGER NOT NULL,"
    "[username] TEXT NOT NULL UNIQUE,"
    "[password] TEXT NOT NULL,"
    "FOREIGN KEY([customerId]) REFERENCES[Customers]([customerId])"
    "ON DELETE NO ACTION ON UPDATE NO ACTION"
    ")";

static const char* createAccountTableCmd =
    "CREATE TABLE IF NOT EXISTS [Accounts]"
    "("
    "[accountId] INTEGER PRIMARY KEY NOT NULL,"
    "[customerId] INTEGER NOT NULL,"
    "[balance] REAL NOT NULL,"
    "[openDate] INTEGER NOT NULL,"
    "FOREIGN KEY ([customerId]) REFERENCES [Customers] ([customerId])"
    "ON DELETE NO ACTION ON UPDATE NO ACTION"
    ")";

static const char* createTransactionTableCmd =
    "CREATE TABLE IF NOT EXISTS [Transactions]"
    "("
    "[transactionId] INTEGER PRIMARY KEY NOT NULL,"
    "[type] TEXT NOT NULL,"
    "[fromAcc] INTEGER,"
    "[toAcc] INTEGER,"
    "[description] TEXT,"
    "[amount] REAL NOT NULL,"
    "[date] INTEGER NOT NULL,"
    "FOREIGN KEY([fromAcc])"
    "REFERENCES[Accounts]([accountId]) ON DELETE NO ACTION ON UPDATE NO ACTION,"
    "FOREIGN KEY([toAcc])"
    "REFERENCES[Accounts]([accountId]) ON DELETE NO ACTION ON UPDATE NO ACTION"
    ")";

namespace Storage::CustomerMngr
{

bool insert(const Customer& customer)
{
    std::array<char, maxSqlCommandLen> commandArray{};
    int32_t ret;
    Customer dummyCustomer;

    SQLite::Database db(dbFile, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
    db.exec(createCustomerTableCmd);

    ret = std::snprintf(commandArray.data(),
                        maxSqlCommandLen,
                        "INSERT INTO Customers"
                        "("
                        "fullname, email, address, birthday"
                        ")"
                        "VALUES"
                        "("
                        "\"%s\", \"%s\", \"%s\", \"%s\""
                        ")",
                        customer.fullname().c_str(),
                        customer.email().c_str(),
                        customer.address().c_str(),
                        customer.birthday().c_str());

    if(ret <= 0) {
        return false;
    }

    std::cout << "Executed command: " << commandArray.data() << "\n";
    db.exec(commandArray.data());

    return true;
}

bool insert(const Customer& customer, std::pair<uuidType, Customer>& outCustomerPair)
{
    if(!insert(customer)) {
        return false;
    }

    return getByEmail(customer.email(), outCustomerPair);
}

bool getByEmail(const std::string& email, std::pair<uuidType, Customer>& outCustomerPair)
{
    std::array<char, maxSqlCommandLen> commandArray{};
    int32_t ret;

    SQLite::Database db(dbFile, SQLite::OPEN_READWRITE);

    ret = std::snprintf(commandArray.data(),
                        maxSqlCommandLen,
                        "SELECT * FROM Customers"
                        " WHERE email = \"%s\"",
                        email.c_str());

    if(ret <= 0) {
        return false;
    }

    std::cout << "Executed command: " << commandArray.data() << "\n";
    SQLite::Statement query(db, commandArray.data());

    if(query.executeStep()) {
        // std::cout << "row =" << query.getColumn(0).getName()
        //           << " val=" << query.getColumn(0).getInt() << "\n";
        outCustomerPair.first = query.getColumn(0).getInt();
        std::string fullname, email, address, birthday;
        fullname = query.getColumn(1).getString();
        email = query.getColumn(2).getString();
        address = query.getColumn(3).getString();
        birthday = query.getColumn(4).getString();

        outCustomerPair.second.setInfo(fullname, email, address, birthday);
        return true;
    }

    return false;
}

bool getByUsername(const std::string& username, std::pair<uuidType, Customer>& outCustomerPair)
{
    std::array<char, maxSqlCommandLen> commandArray{};
    int32_t ret;

    SQLite::Database db(dbFile, SQLite::OPEN_READWRITE);

    ret = std::snprintf(
        commandArray.data(),
        maxSqlCommandLen,
        "SELECT Customers.customerId, fullname, email, address, birthday FROM Customers"
        " INNER JOIN Credentials"
        " ON Customers.customerId = Credentials.customerId"
        " WHERE Credentials.username = \"%s\"",
        username.c_str());

    if(ret <= 0) {
        return false;
    }

    std::cout << "Executed command: " << commandArray.data() << "\n";
    SQLite::Statement query(db, commandArray.data());

    if(query.executeStep()) {
        outCustomerPair.first = query.getColumn(0).getInt();
        std::string fullname, email, address, birthday;
        fullname = query.getColumn(1).getString();
        email = query.getColumn(2).getString();
        address = query.getColumn(3).getString();
        birthday = query.getColumn(4).getString();

        outCustomerPair.second.setInfo(fullname, email, address, birthday);
        return true;
    }

    return false;
}

bool update(uuidType customerId, const Customer& customer)
{
    std::array<char, maxSqlCommandLen> commandArray{};
    int32_t ret;

    SQLite::Database db(dbFile, SQLite::OPEN_READWRITE);

    ret = std::snprintf(commandArray.data(),
                        maxSqlCommandLen,
                        "UPDATE Customers"
                        " SET fullname = \"%s\","
                        "email = \"%s\","
                        "address = \"%s\","
                        "birthday = \"%s\""
                        " WHERE customerId = %d",
                        customer.fullname().c_str(),
                        customer.email().c_str(),
                        customer.address().c_str(),
                        customer.birthday().c_str(),
                        customerId);

    if(ret <= 0) {
        return false;
    }

    std::cout << "Executed command: " << commandArray.data() << "\n";
    db.exec(commandArray.data());
    return true;
}

bool update(uuidType customerId,
            const Customer& customer,
            std::pair<uuidType, Customer>& outCustomerPair)
{
    if(!update(customerId, customer)) {
        return false;
    }

    return getByEmail(customer.email(), outCustomerPair);
}

} // namespace Storage::CustomerMngr

namespace Storage::CredentialsMngr
{

bool insert(const Credentials& credentials, int32_t customerId)
{
    std::array<char, maxSqlCommandLen> commandArray{};
    int32_t ret;

    SQLite::Database db(dbFile, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
    db.exec(createCredentialsTableCmd);

    ret = std::snprintf(commandArray.data(),
                        maxSqlCommandLen,
                        "INSERT INTO [Credentials]"
                        "("
                        "[customerId], [username], [password]"
                        ")"
                        "VALUES"
                        "("
                        "\"%d\", \"%s\", \"%s\""
                        ")",
                        customerId,
                        credentials.username().c_str(),
                        credentials.password().c_str());

    if(ret <= 0) {
        return false;
    }

    std::cout << "Executed command: " << commandArray.data() << "\n";
    db.exec(commandArray.data());

    return true;
}

bool insert(const Credentials& credentials,
            int32_t customerId,
            std::pair<uuidType, Credentials>& outCredentialPair)
{
    if(!insert(credentials, customerId)) {
        return false;
    }

    return getByCustomerId(customerId, outCredentialPair);
}

bool getByCustomerId(int32_t customerId, std::pair<uuidType, Credentials>& outCredentialPair)
{
    std::array<char, maxSqlCommandLen> commandArray{};
    int32_t ret;

    SQLite::Database db(dbFile, SQLite::OPEN_READWRITE);

    ret = std::snprintf(commandArray.data(),
                        maxSqlCommandLen,
                        "SELECT credentialId, username, password FROM Credentials"
                        " WHERE customerId = %d",
                        customerId);

    if(ret <= 0) {
        return false;
    }

    std::cout << "Executed command: " << commandArray.data() << "\n";
    SQLite::Statement query(db, commandArray.data());

    if(query.executeStep()) {
        outCredentialPair.first = query.getColumn(0).getInt();
        std::string username, password;
        username = query.getColumn(1).getString();
        password = query.getColumn(2).getString();

        outCredentialPair.second.setCredentialsWithHashedPassword(username, password);
        return true;
    }

    return false;
}

bool getByUsername(const std::string& username, std::pair<uuidType, Credentials>& outCredentialPair)
{
    std::array<char, maxSqlCommandLen> commandArray{};
    int32_t ret;

    SQLite::Database db(dbFile, SQLite::OPEN_READWRITE);

    ret = std::snprintf(commandArray.data(),
                        maxSqlCommandLen,
                        "SELECT credentialId, username, password FROM Credentials"
                        " WHERE username = \"%s\"",
                        username.c_str());

    if(ret <= 0) {
        return false;
    }

    std::cout << "Executed command: " << commandArray.data() << "\n";
    SQLite::Statement query(db, commandArray.data());

    if(query.executeStep()) {
        std::string username, password;

        outCredentialPair.first = query.getColumn(0).getInt();
        username = query.getColumn(1).getString();
        password = query.getColumn(2).getString();

        outCredentialPair.second.setCredentialsWithHashedPassword(username, password);
        return true;
    }

    return false;
}

bool update(uuidType id, const Credentials& credentials)
{
    std::array<char, maxSqlCommandLen> commandArray{};
    int32_t ret;

    SQLite::Database db(dbFile, SQLite::OPEN_READWRITE);

    ret = std::snprintf(commandArray.data(),
                        maxSqlCommandLen,
                        "UPDATE [Credentials]"
                        "SET [username] = \"%s\","
                        "[password] = \"%s\""
                        "WHERE credentialId = %d",
                        credentials.username().c_str(),
                        credentials.password().c_str(),
                        id);

    if(ret <= 0) {
        return false;
    }

    std::cout << "Executed update command: " << commandArray.data() << "\n";
    db.exec(commandArray.data());

    return true;
}

bool update(uuidType id,
            const Credentials& credentials,
            std::pair<uuidType, Credentials>& outCredentialPair)
{
    if(!update(id, credentials)) {
        return false;
    }

    return getByUsername(credentials.username(), outCredentialPair);
}

} // namespace Storage::CredentialsMngr

namespace Storage::AccountMngr
{
bool insert(const Account& account, int32_t customerId)
{
    std::array<char, maxSqlCommandLen> commandArray{};
    int32_t ret;

    SQLite::Database db(dbFile, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
    std::cout << "SQLite database file '" << db.getFilename().c_str() << "' opened successfully\n";
    db.exec(createAccountTableCmd);

    ret = std::snprintf(commandArray.data(),
                        maxSqlCommandLen,
                        "INSERT INTO [Accounts]"
                        "("
                        "[customerId], [balance], [openDate]"
                        ")"
                        "VALUES"
                        "("
                        "\"%d\", \"%lf\", \"%ld\""
                        ")",
                        customerId,
                        account.balance(),
                        account.openDate().epoch());

    if(ret <= 0) {
        return false;
    }

    std::cout << "Insert command executed: " << commandArray.data() << "\n";
    db.exec(commandArray.data());

    return true;
}

bool insert(const Account& account, int32_t customerId, std::pair<uuidType, Account>& outAccount)
{
    std::vector<std::pair<uuidType, Account>> accountVector;

    if(!insert(account, customerId)) {
        return false;
    }

    if(!getByCustomerId(customerId, accountVector)) {
        return false;
    }

    outAccount = accountVector.back();

    return true;
}

bool getByCustomerId(int32_t customerId,
                     std::vector<std::pair<uuidType, Account>>& outAccountVector)
{
    std::array<char, maxSqlCommandLen> commandArray{};
    int32_t ret;

    SQLite::Database db(dbFile, SQLite::OPEN_READWRITE);

    ret = std::snprintf(commandArray.data(),
                        maxSqlCommandLen,
                        "SELECT accountId, customerId, balance, openDate FROM Accounts"
                        " WHERE customerId = %d",
                        customerId);

    if(ret <= 0) {
        return false;
    }

    std::cout << "Executed command: " << commandArray.data() << "\n";
    SQLite::Statement query(db, commandArray.data());

    while(query.executeStep()) {
        std::pair<uuidType, Account> p;
        uuidType ownerId = query.getColumn(1).getInt();
        double balance = query.getColumn(2).getDouble();
        time_t openDate = query.getColumn(3).getInt();

        p.first = query.getColumn(0).getInt();
        p.second = Account(ownerId, balance, Datetime(openDate));

        outAccountVector.push_back(p);
    }

    return (!outAccountVector.size()) ? false : true;
}

bool update(uuidType id, const Account& account)
{
    std::array<char, maxSqlCommandLen> commandArray{};
    int32_t ret;

    SQLite::Database db(dbFile, SQLite::OPEN_READWRITE);

    ret = std::snprintf(commandArray.data(),
                        maxSqlCommandLen,
                        "UPDATE [Accounts]"
                        "SET customerId = \"%d\","
                        "balance = \"%lf\","
                        "openDate = \"%ld\""
                        "WHERE accountId = %d",
                        account.ownerId(),
                        account.balance(),
                        account.openDate().epoch(),
                        id);

    if(ret <= 0) {
        return false;
    }

    std::cout << "Executed update command: " << commandArray.data() << "\n";
    db.exec(commandArray.data());

    return true;
}

bool update(uuidType id, const Account& account, std::pair<uuidType, Account>& outAccountPair)
{
    std::vector<std::pair<uuidType, Account>> accountVector;

    if(!update(id, account)) {
        return false;
    }

    if(!getByCustomerId(account.ownerId(), accountVector)) {
        return false;
    }

    outAccountPair = accountVector.back();

    return true;
}

} // namespace Storage::AccountMngr

namespace Storage::TransactionMngr
{
bool insert(const Transaction& transaction)
{
    std::array<char, maxSqlCommandLen> commandArray{};
    int32_t ret;
    uuidType relatedAccount = transaction.from() ? transaction.from() : transaction.to();

    if(!relatedAccount) {
        return false;
    }

    SQLite::Database db(dbFile, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
    std::cout << "SQLite database file '" << db.getFilename().c_str() << "' opened successfully\n";
    db.exec(createTransactionTableCmd);

    ret = std::snprintf(commandArray.data(),
                        maxSqlCommandLen,
                        "INSERT INTO [Transactions]"
                        "("
                        "[type], [fromAcc], [toAcc], [description], [amount], [date]"
                        ")"
                        "VALUES"
                        "("
                        "\"%s\", %d, %d, \"%s\", %lf, %ld"
                        ")",
                        transaction.type().c_str(),
                        transaction.from(),
                        transaction.to(),
                        transaction.description().c_str(),
                        transaction.amount(),
                        transaction.date().epoch());

    if(ret <= 0) {
        return false;
    }

    std::cout << "Insert command executed: " << commandArray.data() << "\n";
    db.exec(commandArray.data());

    return true;
}

bool insert(const Transaction& transaction, std::pair<uuidType, Transaction>& outTransaction)
{
    std::vector<std::pair<uuidType, Transaction>> transactionVector;
    uuidType relatedAccount = transaction.from() ? transaction.from() : transaction.to();

    if(!relatedAccount) {
        return false;
    }

    if(!insert(transaction)) {
        return false;
    }

    if(!getByAccountId(relatedAccount, transactionVector)) {
        return false;
    }

    outTransaction = transactionVector.back();

    return true;
}

bool getByAccountId(int32_t accountId,
                    std::vector<std::pair<uuidType, Transaction>>& outTransactionVector)
{
    std::array<char, maxSqlCommandLen> commandArray{};
    int32_t ret;

    SQLite::Database db(dbFile, SQLite::OPEN_READWRITE);

    /*
    "[transactionId] INTEGER PRIMARY KEY NOT NULL,"
    "[type] TEXT NOT NULL,"
    "[from] INTEGER,"
    "[to] INTEGER,"
    "[description] TEXT,"
    "[Amount] INTEGER NOT NULL,"
    */

    ret = std::snprintf(
        commandArray.data(),
        maxSqlCommandLen,
        "SELECT transactionId, type, fromAcc, toAcc, description, amount, date FROM Transactions"
        " WHERE fromAcc = %d OR toAcc = %d",
        accountId,
        accountId);

    if(ret <= 0) {
        return false;
    }

    std::cout << "Executed command: " << commandArray.data() << "\n";
    SQLite::Statement query(db, commandArray.data());

    while(query.executeStep()) {
        std::pair<uuidType, Transaction> p;
        std::string type = query.getColumn(1).getString();
        uuidType from = query.getColumn(2).getInt();
        uuidType to = query.getColumn(3).getInt();
        std::string description = query.getColumn(4).getString();
        double amount = query.getColumn(5).getDouble();
        Datetime date = query.getColumn(6).getInt();

        p.first = query.getColumn(0).getInt();
        p.second = Transaction(date, from, to, description, type, amount);

        outTransactionVector.push_back(p);
    }

    return (!outTransactionVector.size()) ? false : true;
}

bool update(uuidType transactionId, const Transaction& transaction)
{
    std::array<char, maxSqlCommandLen> commandArray{};
    int32_t ret;
    std::vector<std::pair<uuidType, Transaction>> transactionVector;
    uuidType relatedAccount = transaction.from() ? transaction.from() : transaction.to();

    if(!relatedAccount) {
        return false;
    }

    SQLite::Database db(dbFile, SQLite::OPEN_READWRITE);

    //"[type], [fromAcc], [toAcc], [description], [amount], [date]"
    //"\"%s\", %d, %d, \"%s\", %lf, %ld"

    ret = std::snprintf(commandArray.data(),
                        maxSqlCommandLen,
                        "UPDATE [Transactions]"
                        "SET type = \"%s\","
                        "fromAcc = \"%d\","
                        "toAcc = \"%d\""
                        "description = \"%s\""
                        "amount = \"%lf\""
                        "date = \"%ld\""
                        "WHERE transactionId = %d",
                        transaction.type().c_str(),
                        transaction.from(),
                        transaction.to(),
                        transaction.description().c_str(),
                        transaction.amount(),
                        transaction.date().epoch(),
                        transactionId);

    if(ret <= 0) {
        return false;
    }

    std::cout << "Executed update command: " << commandArray.data() << "\n";
    db.exec(commandArray.data());

    return true;
}

bool update(uuidType transactionId,
            const Transaction& transaction,
            std::pair<uuidType, Transaction>& outTransaction)
{

    std::vector<std::pair<uuidType, Transaction>> transactionVector;
    uuidType relatedAccount = transaction.from() ? transaction.from() : transaction.to();

    if(!relatedAccount) {
        return false;
    }

    if(!update(transactionId, transaction)) {
        return false;
    }

    if(!getByAccountId(relatedAccount, transactionVector)) {
        return false;
    }

    outTransaction = transactionVector.back();

    return true;
}

} // namespace Storage::TransactionMngr